// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HW_09.h"
#include "HW_09PlayerController.generated.h"

class UHW_09ChatInput;
class UUserWidget;

/**
 * 
 */
UCLASS()
class HW_09_API AHW_09PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AHW_09PlayerController();

	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);
	
	virtual void GetLifetimeReplicatedProps(TArray<class  FLifetimeProperty>& OutLifetimeProps) const override;
	

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHW_09ChatInput> ChatInputWidegetClass;
	UPROPERTY()
	TObjectPtr<UHW_09ChatInput> ChatInputWidgetInstance;
	
	FString ChatMessageString;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
