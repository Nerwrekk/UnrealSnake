// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoodSpawner.generated.h"

class UFoodEatenEvent;
class UEventBinding;

UCLASS()
class SNAKE_API AFoodSpawner : public AActor
{
private:
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AFoodSpawner();

	UPROPERTY(EditAnywhere)
	float XPosOffset = 0;

	UPROPERTY(EditAnywhere)
	float YPosOffset = 0;

	virtual void PostInitializeComponents() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_BeginPlay();

	UFUNCTION(BlueprintCallable)
	void SpawnFood();

	void PrintFoodName(UFoodEatenEvent* FoodEatenEvent);

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere)
	class AFood* CurrentFood;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFood> Food;

	UPROPERTY(EditAnywhere)
	AActor* LevelMeshComponent;

	UPROPERTY()
	UEventBinding* EventBinding;
	
	FVector Origin;
	FVector LevelBox;
};
