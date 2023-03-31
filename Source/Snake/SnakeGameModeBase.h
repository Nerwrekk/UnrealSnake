// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGameModeBase.generated.h"

class UMySaveGame;

/**
 * 
 */

UCLASS()
class SNAKE_API ASnakeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void PawnDied(APawn* Pawn);
	
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(AController* Controller);
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadSavedGame();

	UFUNCTION(BlueprintCallable)
	void AddPlayerScore(FString PlayerName, int32 Score);
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UFUNCTION(BlueprintCallable)
	UMySaveGame* GetCurrentSavedGame() const;
	
protected:

	UPROPERTY(EditAnywhere)
	FString SlotName;
	
	UPROPERTY()
	UMySaveGame* CurrentSavedGame;

	
};
