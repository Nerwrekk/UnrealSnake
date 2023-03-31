// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodSpawner.h"
#include "Food.h"

// Sets default values
AFoodSpawner::AFoodSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFoodSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	LevelMeshComponent->GetActorBounds(false, Origin, LevelBox);
	UE_LOG(LogTemp, Warning, TEXT("Box: %s"), *LevelBox.ToString());

	SpawnFood();
}

// Called every frame
void AFoodSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodSpawner::SpawnFood()
{
	UE_LOG(LogTemp, Warning, TEXT("spawning food!!"));

	//Get Random location
	TArray<FHitResult> HitResults{};
	FVector RandomPos{};
	do
	{
		auto XPos = FMath::FRandRange(-LevelBox.X + 300, LevelBox.X);
		auto YPos = FMath::FRandRange(-LevelBox.Y + 300, LevelBox.Y);

		RandomPos.X = XPos - 100;
		RandomPos.Y = YPos - 100;
		RandomPos.Z = 0;
		//Sweep random location
		DrawDebugSphere(GetWorld(), RandomPos, 96, 32, FColor::Red, false, 4);
		GetWorld()->SweepMultiByChannel(HitResults, RandomPos, RandomPos, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(96));

	} while(HitResults.Num() > 0);

	UE_LOG(LogTemp, Warning, TEXT("RandomPos: %s"), *RandomPos.ToString());

	//Spawn if no collision
	FActorSpawnParameters SpawnParams{};
	SpawnParams.bNoFail = true;
	CurrentFood = GetWorld()->SpawnActor<AFood>(Food, RandomPos, FQuat::Identity.Rotator(), SpawnParams);

	// Remmember this for future references
	// FScriptDelegate Delegate{};
	// Delegate.BindUFunction(this, TEXT("SpawnFood"));
	
	if (CurrentFood)
		CurrentFood->OnFoodEaten.AddDynamic(this, &AFoodSpawner::SpawnFood);
}


