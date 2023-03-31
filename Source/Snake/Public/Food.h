// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Food.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFoodEaten);

UCLASS()
class SNAKE_API AFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FoodMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Base Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBoxComponent;

public:	
	UPROPERTY(BlueprintAssignable)
	FFoodEaten OnFoodEaten;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
