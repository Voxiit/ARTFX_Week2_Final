// Victor's Lessons, no right reserved


#include "UI/KeybindingsMenuCommonAW.h"
#include "UI/KeyMappingCommonAW.h"
#include "UI/MainCommonButtonBase.h"

#include "Kismet/GameplayStatics.h"
#include "Components/VerticalBox.h"
#include "Components/InputKeySelector.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedActionKeyMapping.h"
#include "Controller/MainPlayerController.h"


void UKeybindingsMenuCommonAW::NativeConstruct()
{
	Super::NativeConstruct();

	// Get Player Controller
	PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	OpenMenu();

	// Key Mappings
	DisplayMappableKeys();

	// Bind exit Button
	if (BIND_ExitButton)
	{
		BIND_ExitButton->OnButtonClicked.AddUniqueDynamic(this, &UKeybindingsMenuCommonAW::OnExitMenuButtonClicked);
		BIND_ExitButton->SetKeyboardFocus();
	}

	UpdateMenuNavigation();
}

void UKeybindingsMenuCommonAW::OpenMenu()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UKeybindingsMenuCommonAW::OnExitMenuButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();
}

void UKeybindingsMenuCommonAW::DisplayMappableKeys()
{
	if (!PlayerController.IsValid() || !KeybindingsWidget || !BIND_MovementKeyVerticalBox || !BIND_ActionKeyVerticalBox)
	{
		return;
	}

	// Get Enhanced Input subsystem
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!EnhancedInputSubsystem)
	{
		return;
	}

	// For each mappable key, create and display a keymapping widget
	TArray<FEnhancedActionKeyMapping> MappableKeyArray = EnhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings();
	for (FEnhancedActionKeyMapping& MappableKey : MappableKeyArray)
	{
		// Get key infos
		FName KeyName = MappableKey.GetMappingName();
		FText KeyDisplayName = MappableKey.GetDisplayName();

		// Create and display infos
		UKeyMappingCommonAW* NewKeyWidget =
			Cast<UKeyMappingCommonAW>(CreateWidget<UKeyMappingCommonAW>(this, KeybindingsWidget));
		if (NewKeyWidget)
		{
			NewKeyWidget->SetInputName(KeyName);
			NewKeyWidget->SetDisplayName(KeyDisplayName);
			NewKeyWidget->SetInputSelector(MappableKey);
		}

		// Place Widget in the right Vertical Box
		FString KeyGroup = MappableKey.GetDisplayCategory().ToString();
		if (KeyGroup == MovementCategoryName)
		{
			BIND_MovementKeyVerticalBox->AddChild(NewKeyWidget);
			MovementKeyMapping.Add(NewKeyWidget);
		}
		else if (KeyGroup == ActionCategoryName)
		{
			BIND_ActionKeyVerticalBox->AddChild(NewKeyWidget);
			ActionKeyMapping.Add(NewKeyWidget);
		}
	
		// Bind to the Key pressed event
		NewKeyWidget->OnKeyButtonPressed.AddUniqueDynamic(this, &UKeybindingsMenuCommonAW::OnKeyButtonPressed);
	}
}

void UKeybindingsMenuCommonAW::UpdateMenuNavigation()
{
	// Make sure our array aren't empty
	if (MovementKeyMapping.IsEmpty() || ActionKeyMapping.IsEmpty())
	{
		return;
	}

	// We want to link the last Movement with the first Action
	UKeyMappingCommonAW* LastMovementWidget = MovementKeyMapping.Last();
	UKeyMappingCommonAW* FirstActionWidget = ActionKeyMapping[0];
	if (!LastMovementWidget || !FirstActionWidget)
	{
		return;
	}

	// Link Input Key Selector
	UInputKeySelector* MovementKeySelector = LastMovementWidget->GetInputKeySelector();
	UInputKeySelector* ActionKeySelector = FirstActionWidget->GetInputKeySelector();
	if (!MovementKeySelector || !ActionKeySelector)
	{
		return;
	}

	LastMovementWidget->SetNavigationRuleExplicit(EUINavigation::Down, ActionKeySelector);
	FirstActionWidget->SetNavigationRuleExplicit(EUINavigation::Up, MovementKeySelector);
}

void UKeybindingsMenuCommonAW::OnKeyButtonPressed()
{
	BIND_ExitButton->SetFocus();
}
