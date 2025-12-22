// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Http.h"
#include "QuizGameInstance.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestionsLoaded);

USTRUCT(BlueprintType)
struct FQuizQuestion
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Question;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Answers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CorrectIndex = 0;
};

UCLASS()
class TENTOONE_API UQuizGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void FetchQuestionsFromGroq(const FString& Topic, int32 Count = 10);

    UPROPERTY(BlueprintReadOnly)
    TArray<FQuizQuestion> Questions;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentQuestionIndex = 0;

    UFUNCTION(BlueprintCallable)
    FQuizQuestion GetCurrentQuestion() const;

    UFUNCTION(BlueprintCallable)
    bool SubmitAnswer(int32 AnswerIndex);

    UFUNCTION(BlueprintCallable)
    void NextQuestion();

    // currently there is hacky way of 5 seconds delay before showing ui to fetch questions, bind wbp bp to this instead later
    UPROPERTY(BlueprintAssignable)
    FOnQuestionsLoaded OnQuestionsLoaded;
private:
    void OnGroqResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    UPROPERTY()
    FString GroqApiKey;
    void Init() override;
};