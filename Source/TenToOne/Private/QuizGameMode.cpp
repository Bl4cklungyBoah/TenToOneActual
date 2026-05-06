#include "QuizGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"

AQuizGameMode::AQuizGameMode()
{
    Score = 0;
    RoundTime = 30.0f;
    TimeLeft = RoundTime;
}

void AQuizGameMode::BeginPlay()
{
    Super::BeginPlay();

    ResetScore();
    StartRound();
}

void AQuizGameMode::ResetScore()
{
    Score = 0;
}

void AQuizGameMode::StartRound()
{
    TimeLeft = RoundTime;

    GetWorldTimerManager().ClearTimer(RoundTimerHandle);

    GetWorldTimerManager().SetTimer(
        RoundTimerHandle,
        this,
        &AQuizGameMode::TickRoundTimer,
        1.0f,
        true
    );
}

void AQuizGameMode::TickRoundTimer()
{
    TimeLeft -= 1.0f;

    if (TimeLeft <= 0.0f)
    {
        TimeLeft = 0.0f;
        OnTimeExpired();
    }
}

void AQuizGameMode::OnTimeExpired()
{
    GetWorldTimerManager().ClearTimer(RoundTimerHandle);

    UE_LOG(LogTemp, Warning, TEXT("Time expired"));

    OnRoundExpired.Broadcast();
}

void AQuizGameMode::SubmitAnswer(bool bCorrect)
{
    GetWorldTimerManager().ClearTimer(RoundTimerHandle);

    if (bCorrect)
    {
        Score++;
    }
    UE_LOG(LogTemp, Warning, TEXT("SubmitAnswer | Correct: %s | Score: %d"),
        bCorrect ? TEXT("true") : TEXT("false"),
        Score
    );
}