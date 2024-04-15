// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EventBinding.generated.h"

class UEventBase;
/**
 * 
 */
UCLASS()
class SNAKE_API UEventBinding : public UObject
{
	GENERATED_BODY()

public:
	UEventBinding();
	
	void BindEvent(const TFunction<void()>& EventNoArgs);
	void BindEvent(const TFunction<void(UEventBase*)>& EventToBind);

	void OnEventTriggerd(UEventBase* Event);
protected:
	TFunction<void()> OnEventNoArgs;
	TFunction<void(UEventBase*)> OnEvent;
};
