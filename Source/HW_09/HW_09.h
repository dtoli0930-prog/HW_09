// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class HW_09FunctionLibrtary
{
public:
	static void MyString(const AActor* InWorldContextActor, const FString& InString, float InTimeToDisplay = 3.f, FColor InColor = FColor::Blue)
	{
		if (IsValid(GEngine) == true && IsValid(InWorldContextActor) == true)
		{
			if(InWorldContextActor->GetNetMode() == NM_Client || InWorldContextActor->GetNetMode() == NM_ListenServer)
			{
				GEngine->AddOnScreenDebugMessage(-1, InTimeToDisplay, InColor, InString);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
			}
		}
	}
	static FString GetNetModeString(const AActor* InWorldContextActor)
	{
		FString NetModeString = TEXT("None");
		if (IsValid(InWorldContextActor) == true)
		{
			ENetMode NetMode = InWorldContextActor->GetNetMode();
			if (NetMode == NM_Client)
			{
				NetModeString = TEXT("Client");
			}
			else
			{
				if (NetMode == NM_Standalone)
				{
					NetModeString = TEXT("Standalone");
				}
				else
				{
					NetModeString = TEXT("Server");
				}
			}
		}
		return NetModeString;
	}
};

