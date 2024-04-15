// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodSpawner.h"
#include "Food.h"
#include "EventSystem/EventBase.h"
#include "EventSystem/EventBus.h"
#include "EventSystem/FoodEatenEvent.h"
#include "EventSystem/EventData/FoodEatenEventData.h"

// Sets default values
AFoodSpawner::AFoodSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentFood = nullptr;
}

void AFoodSpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AFood::OnFoodEaten().AddUObject(this, &AFoodSpawner::SpawnFood);
	
}

// Called when the game starts or when spawned
void AFoodSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	LevelMeshComponent->GetActorBounds(true, Origin, LevelBox);
	UE_LOG(LogTemp, Warning, TEXT("Box: %s"), *LevelBox.ToString());

	EventBinding = NewObject<UEventBinding>();
	EventBinding->BindEvent([=](EventData* Event)
		{
			PrintFoodName(static_cast<FoodEatenEventData*>(Event));
		});
	
	if (UEventBus* EventBus = GetWorld()->GetGameInstance()->GetSubsystem<UEventBus>())
	{
		EventBus->Subscribe(EEventType::FoodConsumed, EventBinding);
	}
	
	BP_BeginPlay(); //For blueprint classes that inherit from FoodSpawner
	
}

// Called every frame
void AFoodSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodSpawner::SpawnFood()
{
	if (!ensureAlways(Food))
	{
		UE_LOG(LogTemp, Warning, TEXT("Food class not set!!"));
	}
	UE_LOG(LogTemp, Warning, TEXT("spawning food!!"));

	//Get Random location
	TArray<FHitResult> HitResults{};
	FVector RandomPos{};
	do
	{
		// const auto XPos = FMath::FRandRange(LevelBox.GetMin(), LevelBox.GetMax());
		// const auto YPos = FMath::FRandRange(LevelBox.GetMin(), LevelBox.GetMax());
		const auto XPos = FMath::FRandRange(-LevelBox.X, LevelBox.X);
		const auto YPos = FMath::FRandRange(-LevelBox.Y, LevelBox.Y);

		RandomPos.X = XPos - XPosOffset;
		RandomPos.Y = YPos - YPosOffset;
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
	// if (CurrentFood)
	// 	CurrentFood->OnFoodEaten.BindRaw(this, &AFoodSpawner::SpawnFood);
}

void AFoodSpawner::PrintFoodName(FoodEatenEventData* FoodEatenEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("OMG food has been eaten: %s"), *GetNameSafe(FoodEatenEvent->FoodActor));
}

void AFoodSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


