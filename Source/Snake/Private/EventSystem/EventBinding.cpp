// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSystem/EventBinding.h"
#include "EventSystem/EventBase.h"
#include "EventSystem/EventData/EventData.h"

UEventBinding::UEventBinding()
{
	
}

void UEventBinding::BindEvent(const TFunction<void()>& EventNoArgs)
{
	OnEventNoArgs = EventNoArgs;
}

void UEventBinding::BindEvent(const TFunction<void(EventData*)>& EventToBind)
{
	OnEvent = EventToBind;
}

void UEventBinding::OnEventTriggerd(EventData* Event)
{
	OnEvent(Event);
}
