// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerScore
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly)
	int32 Score;
};
/**
 * 
 */
UCLASS()
class SNAKE_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	//Note to self!! Don't forget to add UPROPERTY! this was the cause to why the game was not storing the highscore
	//because we did not add UPROPERTY to HighScore, it could not be serialized, and therefore not stored. :)
	UPROPERTY()
	int32 HighScore;

	UPROPERTY(BlueprintReadOnly)
	TArray<FPlayerScore> PlayerScores;
};
