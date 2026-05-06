#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "QuizGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundExpired);

UCLASS()
class TENTOONE_API AQuizGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AQuizGameMode();

    UPROPERTY(BlueprintReadOnly, Category = "Quiz|Score")
    int32 Score;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz|Timer")
    float RoundTime;

    UPROPERTY(BlueprintReadOnly, Category = "Quiz|Timer")
    float TimeLeft;

    UPROPERTY(BlueprintAssignable, Category = "Quiz|Timer")
    FOnRoundExpired OnRoundExpired;

    UFUNCTION(BlueprintCallable, Category = "Quiz|Round")
    void StartRound();

    UFUNCTION(BlueprintCallable, Category = "Quiz|Round")
    void SubmitAnswer(bool bCorrect);

    UFUNCTION(BlueprintCallable, Category = "Quiz|Score")
    void ResetScore();

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle RoundTimerHandle;

    void TickRoundTimer();
    void OnTimeExpired();
};