// Fill out your copyright notice in the Description page of Project Settings.


#include "HW_09ChatGameModeBase.h"
#include "HW_09GameStateBase.h"
#include "HW_09PlayerController.h"
#include "HW_09PlayerState.h"
#include "EngineUtils.h"

void AHW_09ChatGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AHW_09PlayerController* HW_09PlayerController = Cast<AHW_09PlayerController>(NewPlayer);
	if (IsValid(HW_09PlayerController) == true)
	{
		HW_09PlayerController->NotificationText = FText::FromString(TEXT("Connected to Game Server"));
		AllPlayerControllers.Add(HW_09PlayerController);

		AHW_09PlayerState* HWPS = HW_09PlayerController->GetPlayerState<AHW_09PlayerState>();
		if (IsValid(HWPS) == true)
		{
			HWPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		AHW_09GameStateBase* HW_09GameStateBase = GetGameState<AHW_09GameStateBase>();
		if (IsValid(HW_09GameStateBase) == true)
		{
			HW_09GameStateBase->MulticastRPCBroadcastLoginMessage(HWPS->PlayerNameString);
		}
	}
}

FString AHW_09ChatGameModeBase::GenerateGuessNumber()
{
	TArray<int32> NumberArray;
	for (int32 i = 1; i <= 9; ++i)
	{
		NumberArray.Add(i);
	}
	
	FMath::RandInit(FDateTime::Now().GetTicks());
	NumberArray = NumberArray.FilterByPredicate([](int32 Number) { return Number > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, NumberArray.Num() - 1);
		Result.Append(FString::FromInt(NumberArray[Index]));
		NumberArray.RemoveAt(Index);
	}

	return Result;
}

bool AHW_09ChatGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {
		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUniqueNumber = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUniqueNumber = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUniqueNumber == false)
		{
			break;
		}

		bCanPlay = true;
	}while (false);

	return bCanPlay;
}

FString AHW_09ChatGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if(StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	else
	{
		return FString::Printf(TEXT("%dS %dB"), StrikeCount, BallCount);
	}
}

void AHW_09ChatGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SecretNumberString = GenerateGuessNumber();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *SecretNumberString);
}

void AHW_09ChatGameModeBase::PrintChatMessageString(AHW_09PlayerController* InChattingPlayerController, const FString& InMessageChatString)
{
	int Index = InMessageChatString.Len() - 3;
	FString GuessNumberString = InMessageChatString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);

		for (TActorIterator<AHW_09PlayerController> It(GetWorld()); It; ++It)
		{
			AHW_09PlayerController* HW_09PlayerController = *It;
			if (IsValid(HW_09PlayerController) == true)
			{
				FString CombinedMessageString = InMessageChatString + TEXT(" -> ") + JudgeResultString;
				HW_09PlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<AHW_09PlayerController> It(GetWorld()); It; ++It)
		{
			AHW_09PlayerController* HW_09PlayerController = *It;
			if (IsValid(HW_09PlayerController) == true)
			{
				HW_09PlayerController->ClientRPCPrintChatMessageString(InMessageChatString);
			}
		}
	}
}

void AHW_09ChatGameModeBase::IncreaseGuessCount(AHW_09PlayerController* InChattingPlayerController)
{
	AHW_09PlayerState* HWPS = InChattingPlayerController->GetPlayerState<AHW_09PlayerState>();
	if (IsValid(HWPS) == true)
	{
		HWPS->CurrentGuessCount++;
	}
}

void AHW_09ChatGameModeBase::ResetGame()
{
	SecretNumberString = GenerateGuessNumber();

	for (const auto& AHW_09PlayerController : AllPlayerControllers)
	{
		AHW_09PlayerState* HWPS = AHW_09PlayerController->GetPlayerState<AHW_09PlayerState>();
		if(IsValid(HWPS) == true)
		{
			HWPS->CurrentGuessCount = 0;
		}
	}
}

void AHW_09ChatGameModeBase::JudgeGame(AHW_09PlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		AHW_09PlayerState* HWPS = InChattingPlayerController->GetPlayerState<AHW_09PlayerState>();
		for (const auto& HW_09PlayerController : AllPlayerControllers)
		{
			if (IsValid(HWPS) == true)
			{
				FString CombinedMessageString = HWPS->PlayerNameString + TEXT(" has won the game.");
				HW_09PlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& HW_09PlayerController : AllPlayerControllers)
		{
			AHW_09PlayerState* HWPS = HW_09PlayerController->GetPlayerState<AHW_09PlayerState>();
			if (IsValid(HWPS) == true)
			{
				if (HWPS->CurrentGuessCount < HWPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& HW_09PlayerController : AllPlayerControllers)
			{
				HW_09PlayerController->NotificationText = FText::FromString(TEXT("Draw and Restart"));

				ResetGame();
			}
		}
	}
}
