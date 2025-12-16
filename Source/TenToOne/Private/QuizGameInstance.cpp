// Fill out your copyright notice in the Description page of Project Settings.


#include "QuizGameInstance.h"
FQuizQuestion UQuizGameInstance::GetCurrentQuestion() const
{
    if (Questions.IsValidIndex(CurrentQuestionIndex))
    {
        return Questions[CurrentQuestionIndex];
    }

    return FQuizQuestion();
}

bool UQuizGameInstance::SubmitAnswer(int32 AnswerIndex)
{
    if (!Questions.IsValidIndex(CurrentQuestionIndex))
    {
        return false;
    }

    bool bCorrect = Questions[CurrentQuestionIndex].CorrectIndex == AnswerIndex;

    // przechodzimy do nastêpnego pytania
    CurrentQuestionIndex++;

    return bCorrect;
}

void UQuizGameInstance::Init()
{
    Super::Init();

    FQuizQuestion Q1;
    Q1.Question = "What is 2 + 2?";
    Q1.Answers = { "3", "4", "5", "22" };
    Q1.CorrectIndex = 1;

    FQuizQuestion Q2;
    Q2.Question = "Unreal Engine language?";
    Q2.Answers = { "Python", "C++", "Lua", "Java" };
    Q2.CorrectIndex = 1;

    Questions = { Q1, Q2 };
}
