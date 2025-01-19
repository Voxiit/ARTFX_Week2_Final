// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionMenuCommonAW.h"
#include "UI/MainCommonButtonBase.h"
#include "UI/KeybindingsMenuCommonAW.h"

#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "CommonTextBlock.h"
#include "Components/VerticalBox.h"


#include "Controller/MainPlayerController.h"

void UOptionMenuCommonAW::NativeConstruct()
{
	Super::NativeConstruct();

	// Get player controller
	PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if (!PlayerController.IsValid())
	{
		// If we don't have a Main Player Controller, we can't use the option menu
		if (!PlayerController.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Warning - No Player Controller Found in Option Menu"));
			return;
		}
	}

	OpenMenu();

	// Bind exit button
	if (BIND_ExitButton)
	{
		BIND_ExitButton->OnButtonClicked.AddUniqueDynamic(this, &UOptionMenuCommonAW::OnExitButtonClicked);
	}

	// Bind Sliders and display current value
	if(BIND_XSlider && PlayerController.IsValid())
	{
		// Bind to value changed
		BIND_XSlider->OnValueChanged.AddUniqueDynamic(this, &UOptionMenuCommonAW::RecieveXSliderValue);

		// Display the current sensitivity
		float CurrentSensitivityX = PlayerController->GetMouseSentivityX();
		BIND_XSlider->SetValue(CurrentSensitivityX);
	}

	if (BIND_YSlider && PlayerController.IsValid())
	{
		// Bind to value changed
		BIND_YSlider->OnValueChanged.AddUniqueDynamic(this, &UOptionMenuCommonAW::RecieveYSliderValue);

		// Display the current sensitivity
		float CurrentSensitivityY = PlayerController->GetMouseSentivityY();
		BIND_YSlider->SetValue(CurrentSensitivityY);
	}

	// Bind to the Keybindings Button
	if (BIND_KeybindingsMenuButton)
	{
		BIND_KeybindingsMenuButton->OnButtonClicked.AddUniqueDynamic(this, &UOptionMenuCommonAW::OnKeybindingsMenuButtonClicked);
		BIND_KeybindingsMenuButton->SetFocus(); // Set the focus on the first button for using a gamepad
	}
}

void UOptionMenuCommonAW::OpenMenu()
{
	// Display the Widget
	SetVisibility(ESlateVisibility::Visible);
}

void UOptionMenuCommonAW::OnExitButtonClicked()
{
	// Display the Widget
	SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();
}

void UOptionMenuCommonAW::RecieveXSliderValue(float InFloat)
{
	// Update and display the new sensitivity
	if (PlayerController.IsValid())
	{
		PlayerController->SetMouseSentivityX(InFloat);
		DisplaySensitivityX(InFloat);
	}
}

void UOptionMenuCommonAW::RecieveYSliderValue(float InFloat)
{
	// Update and display the new sensitivity
	if (PlayerController.IsValid())
	{
		PlayerController->SetMouseSentivityY(InFloat);
		DisplaySensitivityY(InFloat);
	}
}

void UOptionMenuCommonAW::DisplaySensitivityX(float InSensitivity)
{
	if (BIND_XDisplay)
	{
		// We need to "sanitize" the number aka reduce to 2 digits
		float SensitivityToDisplay = InSensitivity * 100.f; // 2.1325 -> 213.25
		SensitivityToDisplay = FMath::RoundToInt(SensitivityToDisplay) * 0.01f; // 213.25 -> 2.13000
		FString SensitivityString = FString::SanitizeFloat(SensitivityToDisplay); // 2.13000 -> 2.13
		FText SensitivyText = FText::FromString(SensitivityString);

		// Display it
		BIND_XDisplay->SetText(SensitivyText);
	}
}

void UOptionMenuCommonAW::DisplaySensitivityY(float InSensitivity)
{
	if (BIND_YDisplay)
	{
		// We need to "sanitize" the number aka reduce to 2 digits
		float SensitivityToDisplay = InSensitivity * 100.f; // 2.1325 -> 213.25
		SensitivityToDisplay = FMath::RoundToInt(SensitivityToDisplay) * 0.01f; // 213.25 -> 2.13000
		FString SensitivityString = FString::SanitizeFloat(SensitivityToDisplay); // 2.13000 -> 2.13
		FText SensitivyText = FText::FromString(SensitivityString);

		// Display it
		BIND_YDisplay->SetText(SensitivyText);
	}
}

void UOptionMenuCommonAW::OnKeybindingsMenuButtonClicked()
{
	// Create and display the menu. Also bind yourself to the destruct event
	if (KeybindingsMenuWidget)
	{
		UKeybindingsMenuCommonAW* NewKeybindingMenu = Cast<UKeybindingsMenuCommonAW>(CreateWidget<UKeybindingsMenuCommonAW>(this, KeybindingsMenuWidget));
		if (NewKeybindingMenu)
		{
			NewKeybindingMenu->AddToViewport(0);
			NewKeybindingMenu->OnNativeDestruct.AddUObject(this, &UOptionMenuCommonAW::OnKeybindingsMenuClosed);
		}
	}
}

void UOptionMenuCommonAW::OnKeybindingsMenuClosed(UUserWidget* ClosedWidget)
{
	// Reset focus en keybindings button
	if (BIND_KeybindingsMenuButton)
	{
		BIND_KeybindingsMenuButton->SetFocus();
	}
}