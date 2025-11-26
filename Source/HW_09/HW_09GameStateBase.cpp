// Fill out your copyright notice in the Description page of Project Settings.


#include "HW_09GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "HW_09PlayerController.h"

void AHW_09GameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			AHW_09PlayerController* HW_09PC = Cast<AHW_09PlayerController>(PC);
			if(IsValid(HW_09PC) == true)
			{
				FString NotificationString = InNameString + TEXT("has joined the chat.");
				HW_09PC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
