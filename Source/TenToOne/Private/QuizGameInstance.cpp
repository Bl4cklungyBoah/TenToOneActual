#include "QuizGameInstance.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Misc/ConfigCacheIni.h"
static const FString GroqUrl = TEXT("https://api.groq.com/openai/v1/chat/completions");
////////static const FString GroqApiKey = TEXT("CHANGE");

void UQuizGameInstance::FetchQuestionsFromGroq(const FString& Topic, int32 Count)
{
    const FString SystemMsg = TEXT("You are a JSON generator. Output only valid JSON. No markdown. No extra text.");

    FString UserMsg = FString::Printf(
        TEXT("Generate %d multiple-choice quiz questions about %s. Each must have 4 answers and correctIndex 0-3. ")
        TEXT("Return JSON exactly in this schema: {\"items\":[{\"question\":\"...\",\"answers\":[\"...\",\"...\",\"...\",\"...\"],\"correctIndex\":0}]}."),
        Count, *Topic
    );

    TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();
    Root->SetStringField(TEXT("model"), TEXT("llama-3.3-70b-versatile"));

    TArray<TSharedPtr<FJsonValue>> Messages;

    auto MakeMsg = [](const FString& Role, const FString& Content)
        {
            TSharedPtr<FJsonObject> Obj = MakeShared<FJsonObject>();
            Obj->SetStringField(TEXT("role"), Role);
            Obj->SetStringField(TEXT("content"), Content);
            return MakeShared<FJsonValueObject>(Obj);
        };

    Messages.Add(MakeMsg(TEXT("system"), SystemMsg));
    Messages.Add(MakeMsg(TEXT("user"), UserMsg));
    Root->SetArrayField(TEXT("messages"), Messages);

    FString Body;
    {
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Body);
        FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);
    }

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Req = FHttpModule::Get().CreateRequest();
    Req->SetURL("https://api.groq.com/openai/v1/chat/completions");
    Req->SetVerb("POST");
    Req->SetHeader("Content-Type", "application/json");
    Req->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *GroqApiKey));
    Req->SetContentAsString(Body);
    Req->OnProcessRequestComplete().BindUObject(this, &UQuizGameInstance::OnGroqResponse);
    bool reqStatus = Req->ProcessRequest();
    if (!reqStatus)
        UE_LOG(LogTemp, Error, TEXT("Request status error"));
}

void UQuizGameInstance::OnGroqResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    UE_LOG(LogTemp, Error, TEXT("Groq request response"));
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Groq request failed"));
        return;
    }
    TSharedPtr<FJsonObject> Root;
    {
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
        if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to parse Groq outer JSON"));
            return;
        }
    }

    const TArray<TSharedPtr<FJsonValue>>* Choices;
    if (!Root->TryGetArrayField(TEXT("choices"), Choices) || Choices->Num() == 0)
        return;

    TSharedPtr<FJsonObject> Choice0 = (*Choices)[0]->AsObject();
    TSharedPtr<FJsonObject> MessageObj = Choice0->GetObjectField(TEXT("message"));
    FString Content = MessageObj->GetStringField(TEXT("content"));

    TSharedPtr<FJsonObject> QuizRoot;
    {
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
        if (!FJsonSerializer::Deserialize(Reader, QuizRoot) || !QuizRoot.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("Model did not return pure JSON. Content:\n%s"), *Content);
            return;
        }
    }

    const TArray<TSharedPtr<FJsonValue>>* Items;
    if (!QuizRoot->TryGetArrayField(TEXT("items"), Items))
        return;

    Questions.Empty();
    for (auto& V : *Items)
    {
        TSharedPtr<FJsonObject> Obj = V->AsObject();
        if (!Obj) continue;

        FQuizQuestion Q;
        Q.Question = Obj->GetStringField(TEXT("question"));

        const TArray<TSharedPtr<FJsonValue>>* Answers;
        if (Obj->TryGetArrayField(TEXT("answers"), Answers))
        {
            for (auto& A : *Answers)
                Q.Answers.Add(A->AsString());
        }

        Q.CorrectIndex = Obj->GetIntegerField(TEXT("correctIndex"));
        Questions.Add(Q);
    }

    CurrentQuestionIndex = 0;

    UE_LOG(LogTemp, Log, TEXT("Loaded %d quiz questions from Groq"), Questions.Num());
    OnQuestionsLoaded.Broadcast();
}

FQuizQuestion UQuizGameInstance::GetCurrentQuestion() const
{
    if (Questions.IsValidIndex(CurrentQuestionIndex))
        return Questions[CurrentQuestionIndex];

    return FQuizQuestion{};
}

bool UQuizGameInstance::SubmitAnswer(int32 AnswerIndex)
{
    if (!Questions.IsValidIndex(CurrentQuestionIndex))
        return false;

    return Questions[CurrentQuestionIndex].CorrectIndex == AnswerIndex;
}

void UQuizGameInstance::NextQuestion()
{
    if (Questions.Num() == 0) return;
    CurrentQuestionIndex = (CurrentQuestionIndex + 1) % Questions.Num();
}




void UQuizGameInstance::Init()
{
    Super::Init();

    if (!GConfig->GetString(
        TEXT("Groq"),
        TEXT("ApiKey"),
        GroqApiKey,
        GGameIni
    ))
    {
        UE_LOG(LogTemp, Error, TEXT("Groq API key not found in DefaultGame.ini"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Groq API key loaded"));
    }
}
