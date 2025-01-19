// Victor's Lessons, no right reserved


#include "UI/MainMenuCommonAW.h"
#include "UI/MainCommonButtonBase.h"
#include "UI/OptionMenuCommonAW.h"

#include "Kismet/GameplayStatics.h"

#include "Controller/MainPlayerController.h"

void UMainMenuCommonAW::NativeConstruct()
{
	Super::NativeConstruct();

	// Get player controller
	PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	OpenMainMenu();

	// Bind the buttons
	if (BIND_StartButton)
	{
		BIND_StartButton->OnButtonClicked.AddUniqueDynamic(this, &UMainMenuCommonAW::OnStartClicked);
		BIND_StartButton->SetFocus(); // Set the focus on the first button for using a gamepad
	}

	if (BIND_OptionButton)
	{
		BIND_OptionButton->OnButtonClicked.AddUniqueDynamic(this, &UMainMenuCommonAW::OnOptionClicked);
	}

	if (BIND_QuitButton)
	{
		BIND_QuitButton->OnButtonClicked.AddUniqueDynamic(this, &UMainMenuCommonAW::OnQuitClicked);
	}
}

void UMainMenuCommonAW::OpenMainMenu()
{
	// Display the Widget
	SetVisibility(ESlateVisibility::Visible);

	// Update the inputs
	if (PlayerController.IsValid())
	{
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeUIOnly{});
	}
}

void UMainMenuCommonAW::OnStartClicked()
{
	// Remove the widget
	SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();

	// Give back the controls to the player
	if (PlayerController.IsValid())
	{
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly{});
	}

	// Launch the game
	UGameplayStatics::OpenLevel((UObject*)PlayerController->GetWorld(), MapName);
}

void UMainMenuCommonAW::OnOptionClicked()
{
	// Create and display the widget
	if (OptionWidget)
	{
		UOptionMenuCommonAW* CurrentOptionMenuWidget =
			Cast<UOptionMenuCommonAW>(CreateWidget<UOptionMenuCommonAW>(this, OptionWidget));
		if (CurrentOptionMenuWidget)
		{
			CurrentOptionMenuWidget->AddToViewport(0);
			CurrentOptionMenuWidget->OnNativeDestruct.AddUObject(this, &UMainMenuCommonAW::OnOptionClosed);
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMainMenuCommonAW::OnQuitClicked()
{
	if (PlayerController.IsValid())
	{
		UKismetSystemLibrary::QuitGame(this, PlayerController.Get(), EQuitPreference::Quit, true);
	}
}

void UMainMenuCommonAW::OnOptionClosed(UUserWidget* WidgetClosed)
{
	// Reset focus back to Option button when closing the Option Menu
	if (BIND_OptionButton)
	{
		BIND_OptionButton->SetFocus();
	}

	SetVisibility(ESlateVisibility::Visible);
}
