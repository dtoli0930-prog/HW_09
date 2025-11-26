// Fill out your copyright notice in the Description page of Project Settings.


#include "HW_09ChatInput.h"
#include "Components/EditableTextBox.h"
#include "HW_09PlayerController.h"


void UHW_09ChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHW_09ChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHW_09ChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			AHW_09PlayerController* OwningHW_09PlayerController = Cast<AHW_09PlayerController>(OwningPlayerController);
			if (IsValid(OwningHW_09PlayerController) == true)
			{
				OwningHW_09PlayerController->SetChatMessageString(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
