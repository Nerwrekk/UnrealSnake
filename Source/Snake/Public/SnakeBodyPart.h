// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeBodyPart.generated.h"

UCLASS()
class SNAKE_API ASnakeBodyPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeBodyPart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Base Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BodyMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetFollowTarget(AActor* ActorToFollow);
	AActor* GetFollowTarget() const;

	void Stop();
private:

	UPROPERTY(EditAnywhere)
	bool ShouldStop;
	
	UPROPERTY(VisibleAnywhere)
	float TimeElapsed = 0;
	
	UPROPERTY(EditAnywhere)
	float LerpDuration = 0;
	
	UPROPERTY(VisibleAnywhere)
	AActor* FollowTarget;

	UPROPERTY(VisibleAnywhere)
	FVector CurrentDestination;

	UPROPERTY(VisibleAnywhere)
	FVector CurrentPosition;

	UPROPERTY(EditAnywhere)
	float BodyOffset = 50.f;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	float Interp = .2f;
};
