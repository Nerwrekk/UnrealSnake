
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SnakeController.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_API ASnakeController : public APlayerController
{
public:
	virtual void PostInitializeComponents() override;

private:
	GENERATED_BODY()

public:
	void InitializeSnake();
	void GrowSnake();

	void IncreaseScore();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScore();

	virtual void InitPlayerState() override;

	bool GetIsDead() const;
	void SetIsDead(const bool bValue);
	
private:
	virtual void BeginPlayingState() override; 

	UPROPERTY()
	class ASnakeHead* MySnakeHead;

	UPROPERTY()
	class AFood* CurrentFood;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsDead;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int Score;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int HighScore;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int Tails;

};
