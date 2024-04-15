// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventBinding.h"
#include "UObject/Object.h"
#include "EventBus.generated.h"




UENUM()
enum EEventType
{
	FoodConsumed,
	SnakeDied
};


class UEventBase;

/**
 * 
 */
UCLASS()
class SNAKE_API UEventBus : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UEventBus();

	// UFUNCTION(BlueprintCallable)
	void Subscribe(EEventType EventType, UEventBinding* EventFunction);

	// UFUNCTION(BlueprintCallable)
	void Unsubscribe(EEventType EventType, UEventBinding* EventFunction);

	// UFUNCTION(BlueprintCallable)
	void Publish(EEventType EventType);
	void Publish(EEventType EventType, UEventBase* Event);

private:
	UPROPERTY()
	TMap<TEnumAsByte<EEventType>, UEventBinding*> EventBindings;
};



