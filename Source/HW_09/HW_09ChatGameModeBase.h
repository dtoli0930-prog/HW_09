// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HW_09ChatGameModeBase.generated.h"


class AHW_09PlayerController;
/**
 * 
 */
UCLASS()
class HW_09_API AHW_09ChatGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer)override;

	FString GenerateGuessNumber();
	
	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;
	
	void PrintChatMessageString(AHW_09PlayerController* InChattingPlayerController, const FString& InMessageChatString);

	void IncreaseGuessCount(AHW_09PlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(AHW_09PlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<AHW_09PlayerController>> AllPlayerControllers;

};
