// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KeyMappingCommonAW.h"
#include "UI/MainCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/InputKeySelector.h"
#include "Framework/Commands/InputChord.h"

#include "Controller/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"


void UKeyMappingCommonAW::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	// Bind on update key event
	if (BIND_InputKeySelector)
	{
		BIND_InputKeySelector->OnKeySelected.AddUniqueDynamic(this, &UKeyMappingCommonAW::OnKeySelected);
	}

	// Bind Reset button
	if (BIND_ResetButton)
	{
		BIND_ResetButton->OnButtonClicked.AddUniqueDynamic(this, &UKeyMappingCommonAW::OnResetButtonClicked);
	}
}

void UKeyMappingCommonAW::OnKeySelected(FInputChord SelectedKey)
{
	// Get the controller
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning - No Player Controller Found in Option Menu"));
	}

	// Update key
	PlayerController->OnUpdateBindedKey(InputName, SelectedKey.Key);
}

void UKeyMappingCommonAW::SetInputName(FName InName)
{
	InputName = InName;
}

void UKeyMappingCommonAW::SetInputSelector(FEnhancedActionKeyMapping& InKeyMapping)
{
	if (BIND_InputKeySelector)
	{
		DisplayedKey = InKeyMapping;
		BIND_InputKeySelector->SetSelectedKey(DisplayedKey.Key);
	}
}

void UKeyMappingCommonAW::SetDisplayName(FText InText)
{
	if (BIND_InputDisplayName)
	{
		BIND_InputDisplayName->SetText(InText);
	}
}

void UKeyMappingCommonAW::OnResetButtonClicked()
{
	if (!PlayerController)
	{
		return;
	}

	PlayerController->OnResetBindedKey( InputName, DisplayedKey, this);
}
