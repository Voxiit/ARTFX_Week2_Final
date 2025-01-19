// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenuCommonAW.h"
#include "UI/MainCommonButtonBase.h"
#include "UI/OptionMenuCommonAW.h"
#include "UI/MainMenuCommonAW.h"

#include "Kismet/GameplayStatics.h"

#include "Controller/MainPlayerController.h"

void UPauseMenuCommonAW::NativeConstruct()
{
	Super::NativeConstruct();

	// Get player controller
	PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	OpenMenu();

	// Bind the buttons
	if (BIND_ResumeButton)
	{
		BIND_ResumeButton->OnButtonClicked.AddUniqueDynamic(this, &UPauseMenuCommonAW::OnResumeClicked);
		BIND_ResumeButton->SetFocus(); // Set the focus on the first button for using a gamepad
	}

	if (BIND_RestartButton)
	{
		BIND_RestartButton->OnButtonClicked.AddUniqueDynamic(this, &UPauseMenuCommonAW::OnRestartClicked);
	}

	if (BIND_QuitButton)
	{
		BIND_QuitButton->OnButtonClicked.AddUniqueDynamic(this, &UPauseMenuCommonAW::OnQuitClicked);
	}

	if (BIND_OptionButton)
	{
		BIND_OptionButton->OnButtonClicked.AddUniqueDynamic(this, &UPauseMenuCommonAW::OnOptionClicked);
	}

	if (BIND_MainMenuButton)
	{
		BIND_MainMenuButton->OnButtonClicked.AddUniqueDynamic(this, &UPauseMenuCommonAW::OnMainMenuClicked);
	}
}

void UPauseMenuCommonAW::OpenMenu()
{
	// Display the Widget
	SetVisibility(ESlateVisibility::Visible);

	// Update the inputs
	if (PlayerController.IsValid())
	{
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeUIOnly{});
	}

	// Pause the Game
	UGameplayStatics::SetGamePaused(this, true);
}

void UPauseMenuCommonAW::OnResumeClicked()
{
	// Display the Widget
	SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();

	// Update the inputs
	if (PlayerController.IsValid())
	{
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly{});
	}

	// Pause the Game
	UGameplayStatics::SetGamePaused(this, false);
}

void UPauseMenuCommonAW::OnRestartClicked()
{
	// Get Map Name
	FName LevelName = FName(*UGameplayStatics::GetCurrentLevelName(this));
	// Start the Map
	UGameplayStatics::OpenLevel(this, LevelName);

	// Give back the control to the player
	if (!PlayerController.IsValid())
	{
		PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	}

	PlayerController->SetInputMode(FInputModeGameOnly{});
	PlayerController->SetShowMouseCursor(false);

	// Destroy the widget
	RemoveFromParent();
}

void UPauseMenuCommonAW::OnQuitClicked()
{
	if (PlayerController.IsValid())
	{
		UKismetSystemLibrary::QuitGame(this, PlayerController.Get(), EQuitPreference::Quit, true);
	}
}

void UPauseMenuCommonAW::OnOptionClicked()
{
	// Create and display the widget
	if (OptionMenuWidget)
	{
		UOptionMenuCommonAW* CurrentOptionMenuWidget =
			Cast<UOptionMenuCommonAW>(CreateWidget<UOptionMenuCommonAW>(this, OptionMenuWidget));
		if (CurrentOptionMenuWidget)
		{
			CurrentOptionMenuWidget->AddToViewport(0);
			CurrentOptionMenuWidget->OnNativeDestruct.AddUObject(this, &UPauseMenuCommonAW::OnOptionMenuClosed);
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UPauseMenuCommonAW::OnOptionMenuClosed(UUserWidget* ClosedWidget)
{
	// Reset focus back to Option button when closing the Option Menu
	if (BIND_OptionButton)
	{
		BIND_OptionButton->SetFocus();
	}

	SetVisibility(ESlateVisibility::Visible);
}

void UPauseMenuCommonAW::OnMainMenuClicked()
{
	UGameplayStatics::OpenLevel((UObject*)PlayerController->GetWorld(), MainMenuMapName);
}
