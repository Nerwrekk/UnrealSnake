// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "Components/BoxComponent.h"
#include "../Interface/ITriggerable.h"
#include "EventSystem/EventBus.h"
#include "EventSystem/FoodEatenEvent.h"

//This is how you implement a static event in a class in unreal
AFood::FEeatenFood FoodEaten; 

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = CollisionBoxComponent;

	FoodMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Food Mesh"));
	FoodMeshComponent->SetupAttachment(RootComponent);
}

AFood::FEeatenFood& AFood::OnFoodEaten()
{
	return FoodEaten;
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
	if (CollisionBoxComponent)
	{
		CollisionBoxComponent->OnComponentHit.AddDynamic(this, &AFood::OnHit);
		CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFood::OnOverlapBegin);
	}

	UE_LOG(LogTemp, Warning, TEXT("Hello"));
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFood::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit"));


	//auto SnakeHead = Cast<ASnakeHead>(OtherActor);

	//if (SnakeHead)
	//	SnakeHead->GrowSnake();

	////Destroy
	//Destroy();
}

void AFood::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Good way of checking for specific class
	// if (!OtherActor->IsA(ASnakeHead::StaticClass())) return;

	// if (OtherActor->GetClass()->ImplementsInterface(UITriggerable::StaticClass())) //keeping for learning
	if (OtherActor->Implements<UITriggerable>()) //keeping for learning
		UE_LOG(LogTemp, Warning, TEXT("Trigger"));

	if (IITriggerable* Triggerable = Cast<IITriggerable>(OtherActor))
	{
		Triggerable->Trigger();

		FoodEaten.Broadcast();

		if (UEventBus* EventBus = GetWorld()->GetGameInstance()->GetSubsystem<UEventBus>())
		{
			UFoodEatenEvent* EatenEvent = NewObject<UFoodEatenEvent>();
			EatenEvent->FoodActor = this;
			EventBus->Publish(EEventType::FoodConsumed, EatenEvent);
		}
		
		Destroy();
	}
	
}

