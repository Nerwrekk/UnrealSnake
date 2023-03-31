// Copyright Epic Games, Inc. All Rights Reserved.


#include "SnakeGameModeBase.h"

#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"

void ASnakeGameModeBase::PawnDied(APawn* Pawn)
{
	GameOver(Pawn->GetController());
}

void ASnakeGameModeBase::SaveGame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSavedGame, SlotName, 0);
}

void ASnakeGameModeBase::LoadSavedGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSavedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSavedGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load game"));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("loaded game data successfully"));
		UE_LOG(LogTemp, Log, TEXT("HighScore: %i"), CurrentSavedGame->HighScore);

		for (auto [PlayerName, Score] : CurrentSavedGame->PlayerScores)
		{
			UE_LOG(LogTemp, Log, TEXT("Player Name: %s"), *PlayerName);
			UE_LOG(LogTemp, Log, TEXT("Score: %d"), Score);
		}
	}
	else
	{
		CurrentSavedGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		//Default HighScore if save file does not exists
		CurrentSavedGame->PlayerScores = {
			{TEXT("Jenae"), 25},
			{TEXT("Clemens"), 22},
			{TEXT("Evan"), 20},
			{TEXT("Wenzel"), 19},
			{TEXT("Willy"), 17},
			{TEXT("Bobby"), 16},
			{TEXT("Kurt"), 13},
			{TEXT("Rick"), 9},
			{TEXT("Morty"), 6},
			{TEXT("ASS"), 3},
		};
		UE_LOG(LogTemp, Log, TEXT("Created new save game successfully"))
	}
}

void ASnakeGameModeBase::AddPlayerScore(FString PlayerName, int32 Score)
{
	FPlayerScore NewPlayerScore;
	NewPlayerScore.PlayerName = PlayerName;
	NewPlayerScore.Score = Score;

	auto SortScore = [&](const FPlayerScore& First, const FPlayerScore& Second)
	{
		return (First.Score > Second.Score);
	};
	
	//sort PlayerScore so first index has the highest score
	CurrentSavedGame->PlayerScores.Sort(SortScore);
	const int LowestScoreIndex = CurrentSavedGame->PlayerScores.Max()-1;
	//the new score is lower then the lowest score so just return
	if (Score < CurrentSavedGame->PlayerScores[LowestScoreIndex].Score)
	{
		return;
	}

	//Replace lowest score with the new one then sort
	CurrentSavedGame->PlayerScores.RemoveAt(LowestScoreIndex);
	CurrentSavedGame->PlayerScores.Add(NewPlayerScore);
	CurrentSavedGame->PlayerScores.Sort(SortScore);
	
	SaveGame();
}

void ASnakeGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSavedGame();
}

UMySaveGame* ASnakeGameModeBase::GetCurrentSavedGame() const
{
	return CurrentSavedGame;
}
