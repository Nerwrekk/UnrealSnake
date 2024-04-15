// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSystem/EventBus.h"

#include "EventSystem/EventBase.h"

UEventBus::UEventBus()
{
}

void UEventBus::Subscribe(EEventType EventType, UEventBinding* EventFunction)
{
	EventBindings.Add(EventType, EventFunction);
}

void UEventBus::Unsubscribe(EEventType EventType, UEventBinding* EventFunction)
{
}

void UEventBus::Publish(EEventType EventType)
{
}

void UEventBus::Publish(EEventType EventType, UEventBase* Event)
{
	for (auto& Binding : EventBindings)
	{
		if (Binding.Key == EventType)
		{
			Binding.Value->OnEventTriggerd(Event);
		}
	}
}
