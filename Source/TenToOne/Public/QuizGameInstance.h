// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "QuizGameInstance.generated.h"
/**
 * Struktura jednego pytania quizowego
 */
USTRUCT(BlueprintType)
struct FQuizQuestion
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Question;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Answers; // A, B, C, D

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CorrectIndex = 0; // 0-3
};

/**
 * GameInstance do przechowywania quizu
 */
UCLASS()
class TENTOONE_API UQuizGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    // Wszystkie pytania
    UPROPERTY(BlueprintReadOnly)
    TArray<FQuizQuestion> Questions;

    // Aktualny indeks pytania
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentQuestionIndex = 0;

    // Zwraca aktualne pytanie
    UFUNCTION(BlueprintCallable)
    FQuizQuestion GetCurrentQuestion() const;

    // Sprawdza odpowiedü i przechodzi dalej
    UFUNCTION(BlueprintCallable)
    bool SubmitAnswer(int32 AnswerIndex);
    virtual void Init() override;
};