// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventBase.h"
#include "UObject/Object.h"
#include "FoodEatenEvent.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_API UFoodEatenEvent : public UEventBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AActor* FoodActor;
};
