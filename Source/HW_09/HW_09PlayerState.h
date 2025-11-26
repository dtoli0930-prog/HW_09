// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HW_09PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HW_09_API AHW_09PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AHW_09PlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class  FLifetimeProperty>& OutLifetimeProps) const override;
	
	FString GetPlayerInfoString();

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
	
};
