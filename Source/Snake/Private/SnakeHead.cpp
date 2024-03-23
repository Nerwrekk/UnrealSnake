// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHead.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "MySaveGame.h"
#include "SnakeBodyPart.h"
#include "SnakeController.h"
#include "Components/SplineMeshComponent.h"
#include "Snake/SnakeGameModeBase.h"

// Sets default values
ASnakeHead::ASnakeHead()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SnakeHeadMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Snake Head"));
	RootComponent = SnakeHeadMeshComp;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(RootComponent);

	SplineMeshComponent = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMesh"));
	SplineMeshComponent->SetMobility(EComponentMobility::Movable);
	SplineMeshComponent->SetupAttachment(SplineComponent);
}

// Called when the game starts or when spawned
void ASnakeHead::BeginPlay()
{
	Super::BeginPlay();

	SnakeController = Cast<ASnakeController>(GetController());
	if (!SnakeController)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is nullptr"))
	}
	
	if (SplineComponent)
	{
		const auto Points = SplineComponent->GetNumberOfSplinePoints();
		for (int i = 0; i < Points; ++i)
		{
			SplineComponent->RemoveSplinePoint(i);
		}
	}
	// InitializeSnake();
	// UGameplayStatics::GetActorOfClass(this, AFood::StaticClass()).OnFood;
}

// Called every frame
void ASnakeHead::Tick(float DeltaTime)
{
	if (!bGameStart) return;
	if (SnakeController->GetIsDead()) return;
	
	Super::Tick(DeltaTime);

	MoveSnake(DeltaTime);

	if (ShouldRenderSpline)
	{
		RenderSpline();
	}
	
	if (HasHitObstacle())
	{
		StopSnake();
	}
}

// Called to bind functionality to input
void ASnakeHead::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASnakeHead::Turn);
}

void ASnakeHead::Turn(float Value)
{
	auto DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	
	//auto moveValue = GetInputAxisValue(TEXT("MoveForward")); //This is how we can check if InputAxis have values
	//Debugging = moveValue;
	//Checks if we are backing, if so we revert the turning controls
	//if (moveValue == -1) Value *= -1; //this changes direction
	
	//rotating with FRotator
	FRotator rotator = FRotator::ZeroRotator;
	rotator.Yaw =  Value * RotationSpeed * DeltaTime;

	//FQuat Quat(0, 0, 0, 0);
	//Quat.Z = Value * RotationSpeed * DeltaTime;
	//Quat.W = 100; //DON'T KNOW WHAT W DOES YET BUT IMPORTANT!!!
	
	// AddActorLocalRotation(rotator, true);
	AddActorWorldRotation(rotator, true);
}

void ASnakeHead::InitializeSnake()
{
	AddSplineMeshBodyPart();
	SplineComponent->AddSplinePoint(GetActorLocation(), ESplineCoordinateSpace::World);
	SplineComponent->SetSplinePointType(SplineComponent->GetSplineLength(), ESplinePointType::Linear);
	
	for (int i = 0; i < DefaultNumOfBodyParts; i++)
	{
		const FVector Headlocation = GetActorLocation() - BodyCollisionOffset;
		auto BodyPartPtr = GetWorld()->SpawnActor(SnakeBodyPart, &Headlocation);
		BodyParts.Add(BodyPartPtr);
		AddSplineMeshBodyPart();
		SplineComponent->AddSplinePoint(BodyPartPtr->GetActorLocation(), ESplineCoordinateSpace::World);
		SplineComponent->SetSplinePointType(SplineComponent->GetSplineLength(), ESplinePointType::Linear);

	}

	//Not the sexiest way of implementig this
	for (int i = 0; i < BodyParts.Num(); i++)
	{
		const auto BodyPartPtr = Cast<ASnakeBodyPart>(BodyParts[i]);
		if (i == 0) {
			BodyPartPtr->SetFollowTarget(this);
			continue;
		}

		BodyPartPtr->SetFollowTarget(BodyParts[i - 1]);
	}
}

void ASnakeHead::GrowSnake()
{
	const auto LastBodyPart = BodyParts.Last();
	const auto SpawnLocation = LastBodyPart->GetActorLocation();// - LastBodyPart->GetActorForwardVector(); // * 50;
	const auto Rotation = GetActorRotation();
	auto* BodyPartPtr = Cast<ASnakeBodyPart>(GetWorld()->SpawnActor(SnakeBodyPart, &SpawnLocation, &Rotation));
	if (!BodyPartPtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BodyPartPtr is pointing to nullptr"));
		return;
	}
	BodyPartPtr->SetFollowTarget(LastBodyPart);
	BodyParts.Add(BodyPartPtr);

	SplineComponent->AddSplinePoint(BodyPartPtr->GetActorLocation(), ESplineCoordinateSpace::World);
	AddSplineMeshBodyPart();
}

void ASnakeHead::SetStartLocation(ASnakeBodyPart* SnakePart)
{
	auto FollowTargetPtr = SnakePart->GetFollowTarget();
	FVector Dir = (FollowTargetPtr->GetActorLocation()); // - GetActorForwardVector() * 80);
	
	SnakePart->SetActorLocation(Dir);
}

void ASnakeHead::StartGame()
{
	InitializeSnake();
	bGameStart = true;
}

void ASnakeHead::StopSnake()
{
	MoveSpeed = 0;

	const auto SnakeGameMode = Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	for (auto BodyPart : BodyParts) {
		Cast<ASnakeBodyPart>(BodyPart)->Stop();
	}
	
	const auto PlayerControl = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	DisableInput(PlayerControl);
	Cast<ASnakeController>(PlayerControl)->SetIsDead(true);
}

void ASnakeHead::Trigger()
{
	GrowSnake();

	MoveSpeed = FMath::Min(MoveSpeed + 20, MaxSpeed);

	SnakeController->IncreaseScore();
}

void ASnakeHead::MoveSnake(float DeltaTime)
{
	auto ForwardVector = GetActorForwardVector();
	AddActorWorldOffset(ForwardVector * DeltaTime * MoveSpeed);
	DrawDebugLine(GetWorld(), GetActorLocation(),GetActorLocation() + GetActorForwardVector() * 500, FColor::Red);
}

void ASnakeHead::RenderSpline()
{
	const auto NumOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();
	for (int i = 0; i < NumOfSplinePoints; ++i)
	{
		//head location
		if (i == 0)
		{
			SplineComponent->SetLocationAtSplinePoint(i, GetActorLocation(), ESplineCoordinateSpace::World);
		}
		else
		{
			SplineComponent->SetLocationAtSplinePoint(i, BodyParts[i-1]->GetActorLocation(), ESplineCoordinateSpace::World);

		}
		
		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		
		
		SplineMeshParts[i]->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);
	}
}

void ASnakeHead::AddSplineMeshBodyPart()
{
	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	SplineMesh->SetStaticMesh(SnakeBodyMesh);
	SplineMesh->SetMobility(EComponentMobility::Movable);
	SplineMesh->CreationMethod = EComponentCreationMethod::Instance;
	SplineMesh->RegisterComponentWithWorld(GetWorld());
	SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	SplineMeshParts.Add(SplineMesh);
}

bool ASnakeHead::HasHitObstacle()
{
	DrawDebugSphere(GetWorld(), GetActorLocation() + GetActorForwardVector() * BodyCollisionOffset, 16, 16, FColor::Red);

	FHitResult HitResult{};
	GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation() + GetActorForwardVector() * BodyCollisionOffset, GetActorLocation() + GetActorForwardVector() * BodyCollisionOffset, 
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(16));

	const auto Actor = HitResult.GetActor();
	if (Actor && Actor->ActorHasTag(TEXT("Obstacle")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Obstacle detected!!"));
		return true;
	}
	
	return false;
}


