// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Interface/ITriggerable.h"
#include "Components/SplineComponent.h"
#include "Math/Vector.h"
#include "SnakeHead.generated.h"

class USplineMeshComponent;

class ASnakeBodyPart;

 FVector;

UCLASS()
class SNAKE_API ASnakeHead : public APawn, public IITriggerable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnakeHead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Base Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SnakeHeadMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Base Components", meta = (AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Components", meta = (AllowPrivateAccess = "true"))
	class USplineMeshComponent* SplineMeshComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Turn(float Value);

	void InitializeSnake();

	UFUNCTION(BlueprintCallable)
	void GrowSnake();

	void SetStartLocation(ASnakeBodyPart* SnakePart);

	UFUNCTION(BlueprintCallable)
	void StartGame();
	
	void StopSnake();

	virtual void Trigger() override;
private:

	UPROPERTY(EditAnywhere)
	bool ShouldRenderSpline;

	void MoveSnake(float DeltaTime);
	
	void RenderSpline();
	
	bool HasHitObstacle();

	void AddSplineMeshBodyPart();

	UPROPERTY(EditAnywhere)
	UStaticMesh* SnakeBodyMesh;
	
	UPROPERTY(EditAnywhere)
	int DefaultNumOfBodyParts = 3;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 5.f;

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 5.f;
	
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 20.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASnakeBodyPart> SnakeBodyPart;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> BodyParts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<USplineMeshComponent*> SplineMeshParts;

	UPROPERTY(EditAnywhere)
	float BodyCollisionOffset = 75.f;

	UPROPERTY(EditAnywhere)
	bool bGameStart = false;
};
