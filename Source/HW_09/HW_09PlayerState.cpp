// Fill out your copyright notice in the Description page of Project Settings.


#include "HW_09PlayerState.h"
#include "Net/UnrealNetwork.h"

AHW_09PlayerState::AHW_09PlayerState() 
	: PlayerNameString(TEXT("None"))
	, CurrentGuessCount(0)
	, MaxGuessCount(3)
{
	bReplicates = true;
}

void AHW_09PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString AHW_09PlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT(" (") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}
