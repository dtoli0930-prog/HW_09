// Fill out your copyright notice in the Description page of Project Settings.


#include "HW_09PlayerController.h"
#include "HW_09ChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "HW_09ChatGameModeBase.h"
#include "HW_09PlayerState.h"
#include "Net/UnrealNetwork.h"

AHW_09PlayerController::AHW_09PlayerController()
{
	bReplicates = true;
}

void AHW_09PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidegetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UHW_09ChatInput>(this, ChatInputWidegetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void AHW_09PlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		AHW_09PlayerState* HWPS = GetPlayerState<AHW_09PlayerState>();
		if (IsValid(HWPS) == true)
		{
			FString CombinedMessageString = HWPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void AHW_09PlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	HW_09FunctionLibrtary::MyString(this, InChatMessageString, 6.f);
}

void AHW_09PlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void AHW_09PlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}


void AHW_09PlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AHW_09ChatGameModeBase* HWGM = Cast<AHW_09ChatGameModeBase>(GM);
		if (IsValid(HWGM) == true)
		{
			HWGM->PrintChatMessageString(this, InChatMessageString);
		}
	}

}
