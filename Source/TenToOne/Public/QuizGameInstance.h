// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "QuizGameInstance.generated.h"


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

    UPROPERTY(BlueprintReadOnly)
    TArray<FQuizQuestion> Questions;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentQuestionIndex = 0;

    UFUNCTION(BlueprintCallable)
    FQuizQuestion GetCurrentQuestion() const;

    UFUNCTION(BlueprintCallable)
    bool SubmitAnswer(int32 AnswerIndex);
    virtual void Init() override;
};