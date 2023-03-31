// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBodyPart.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASnakeBodyPart::ASnakeBodyPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Part"));
	RootComponent = BodyMesh;
}

// Called when the game starts or when spawned
void ASnakeBodyPart::BeginPlay()
{
	Super::BeginPlay();
	CurrentPosition = GetActorLocation();
}

// Called every frame
void ASnakeBodyPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldStop)
	{
		return;
	}

	if (!FollowTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("FollowTarget is pointing to nullptr"));
		return;
	}
	
	auto Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentDestination);
	// SetActorRotation(Rotator);

	if (TimeElapsed < LerpDuration) {
		//Current possition was not initialized causing the current position to be 0,0,0
		SetActorLocation(FMath::Lerp(CurrentPosition, CurrentDestination, TimeElapsed / LerpDuration));
		SetActorRotation(Rotator);
		TimeElapsed += DeltaTime;
	}
	else {
		TimeElapsed = 0;
		SetActorLocation(CurrentDestination);
		CurrentPosition = GetActorLocation();
		CurrentDestination = FollowTarget->GetActorLocation();
	}
}

void ASnakeBodyPart::SetFollowTarget(AActor* ActorToFollow)
{
	this->FollowTarget = ActorToFollow;
	CurrentDestination = ActorToFollow->GetActorLocation();
}

AActor* ASnakeBodyPart::GetFollowTarget() const
{
	return this->FollowTarget;
}

void ASnakeBodyPart::Stop()
{
	ShouldStop = true;
}

