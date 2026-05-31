#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QuizSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FHighScoreEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Score")
    FString PlayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Score")
    int32 Score = 0;
};

UCLASS()
class TENTOONE_API UQuizSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "High Score")
    TArray<FHighScoreEntry> HighScores;
};