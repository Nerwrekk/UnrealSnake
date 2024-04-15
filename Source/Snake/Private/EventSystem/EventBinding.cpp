// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSystem/EventBinding.h"
#include "EventSystem/EventBase.h"

UEventBinding::UEventBinding()
{
	
}

void UEventBinding::BindEvent(const TFunction<void()>& EventNoArgs)
{
	OnEventNoArgs = EventNoArgs;
}

void UEventBinding::BindEvent(const TFunction<void(UEventBase*)>& EventToBind)
{
	OnEvent = EventToBind;
}

void UEventBinding::OnEventTriggerd(UEventBase* Event)
{
	OnEvent(Event);
}
