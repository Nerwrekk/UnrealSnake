// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeController.h"
#include "Food.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SnakeHead.h"
#include "Snake/SnakeGameModeBase.h"

void ASnakeController::InitializeSnake()
{
}

void ASnakeController::GrowSnake()
{
}

void ASnakeController::InitPlayerState()
{
	Super::InitPlayerState();
}

bool ASnakeController::GetIsDead() const
{
	return bIsDead;
}

void ASnakeController::SetIsDead(const bool bValue)
{
	bIsDead = bValue;

	if (bIsDead)
	{
		Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(this))->PawnDied(this->GetPawn());
	}
}

void ASnakeController::BeginPlayingState()
{
	Super::BeginPlayingState();

	MySnakeHead = GetPawn<ASnakeHead>();
	
	CurrentFood = Cast<AFood>(UGameplayStatics::GetActorOfClass(this, AFood::StaticClass()));
	if (CurrentFood)
		CurrentFood->OnFoodEaten.AddDynamic(this, &ASnakeController::IncreaseScore);

	auto SnakeGameMode = Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (SnakeGameMode)
	{
		auto CurrentSavedGame = SnakeGameMode->GetCurrentSavedGame();
		HighScore = CurrentSavedGame->HighScore;
		UpdateScore();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load game"));
	}
}

void ASnakeController::IncreaseScore()
{
	Score++;
	Tails++;

	const auto SnakeGameMode = Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (SnakeGameMode)
	{
		auto CurrentSavedGame = SnakeGameMode->GetCurrentSavedGame();
		if (CurrentSavedGame->HighScore < Score)
		{
			CurrentSavedGame->HighScore = Score;
			UE_LOG(LogTemp, Log, TEXT("HighScore: %i"), CurrentSavedGame->HighScore);
		}
		SnakeGameMode->SaveGame();
	}
	
	if (HighScore < Score)
	{
		HighScore = Score;
	}
	
	UpdateScore();
	
	UE_LOG(LogTemp, Warning, TEXT("TESTING"))
}
