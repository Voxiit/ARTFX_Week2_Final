// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MainPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include "Player/MainPlayer.h"
#include "Controller/GravityGunController.h"
#include "GameFramework/GameUserSettings.h"
#include "EnhancedActionKeyMapping.h"
#include "PlayerMappableKeySettings.h"

#include "Kismet/GameplayStatics.h"
#include "Gameplay/Goal.h"
#include "Gameplay/ScoreComponent.h"
#include "Controller/SpawnerController.h"
#include "UI/PauseMenuCommonAW.h"
#include "UI/KeyMappingCommonAW.h"

#include "Save/OptionSaveGame.h"


void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	LoadOptionData();
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind to subsystem
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!EnhancedInputSubsystem)
		return;

	EnhancedInputSubsystem->ClearAllMappings();
	EnhancedInputSubsystem->AddMappingContext(InputMapping, 0);

	// Get Enhanced Input Comp
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
		return;
	EnhancedInputComponent->ClearActionBindings();

	// Bind Movements
	EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AMainPlayerController::MovePlayer);
	EnhancedInputComponent->BindAction(InputActionLook, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);
	EnhancedInputComponent->BindAction(InputActionJump, ETriggerEvent::Triggered, this, &AMainPlayerController::Jump);

	// Exercice 3
	EnhancedInputComponent->BindAction(InputActionScore, ETriggerEvent::Triggered, this, &AMainPlayerController::OnDisplayScoreInputPressed);

	// Pause
	EnhancedInputComponent->BindAction(InputActionPause, ETriggerEvent::Triggered, this, &AMainPlayerController::OnPauseInputPressed);

	// Keybindings - Tell to Unreal that we're going to update the keys in the Input Mapping Context
	EnhancedInputUserSettings = EnhancedInputSubsystem->GetUserSettings();
	if (EnhancedInputUserSettings)
	{
		EnhancedInputUserSettings->RegisterInputMappingContext(InputMapping);
	}
}

void AMainPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	// We want to bind only once to the inputs
	if (Character)
		return;

	// Get a ref to our Character
	Character = Cast<AMainPlayer>(InPawn);
	if (!Character)
		return;

	GravityGunController = GetComponentByClass<UGravityGunController>();
	if (GravityGunController.IsValid())
	{
		GravityGunController->SetupInputComponentGravityGun(InputComponent, Character);
	}

	// Exercice 4
	ScoreComponent = Character->GetComponentByClass<UScoreComponent>();

	// Noted Exercice 
	SpawnerController = GetComponentByClass<USpawnerController>();
	if (SpawnerController.IsValid())
	{
		SpawnerController->SetupInputPlayerController(InputComponent, Character);
	}
}

void AMainPlayerController::MovePlayer(const FInputActionValue& Value)
{
	if (!Character)
		return;

	// Get movement value
	const FVector2D MoveValue = Value.Get<FVector2D>();

	// Check if the player wants to move in the forward direction
	if (MoveValue.Y != 0.f)
	{
		Character->AddMovementInput(Character->GetActorForwardVector(), MoveValue.Y);
	}

	// Check if the player wants to move in the right direction
	if (MoveValue.X != 0.f)
	{
		Character->AddMovementInput(Character->GetActorRightVector(), MoveValue.X);
	}
}

void AMainPlayerController::Look(const FInputActionValue& Value)
{
	if (!Character)
		return;

	// Get movement value
	const FVector2D MoveValue = Value.Get<FVector2D>();

	if (MoveValue.X != 0.f)
	{
		Character->AddControllerYawInput(MoveValue.X);
	}

	if (MoveValue.Y != 0.f)
	{
		Character->AddControllerPitchInput(-MoveValue.Y);
	}
}

void AMainPlayerController::Jump()
{
	if (!Character)
		return;

	Character->Jump();
}

void AMainPlayerController::AddPitchInput(float Value)
{
	float MultipliedValue = Value * MouseSensitivityY;
	Super::AddPitchInput(MultipliedValue);
}

void AMainPlayerController::AddYawInput(float Value)
{
	float MultipliedValue = Value * MouseSensitivityX;
	Super::AddYawInput(MultipliedValue);
}

void AMainPlayerController::OnDisplayScoreInputPressed()
{
	if (!ScoreComponent.IsValid())
		return;

	ScoreComponent->OnDisplayScoreInputPressed();
}

void AMainPlayerController::OnPauseInputPressed()
{
	// Create and display the widget
	if (PauseMenuWidget)
	{
		UPauseMenuCommonAW* CurrentPauseMenuWidget = 
			Cast<UPauseMenuCommonAW>(CreateWidget<UPauseMenuCommonAW>(this, PauseMenuWidget));
		if (CurrentPauseMenuWidget)
		{
			CurrentPauseMenuWidget->AddToViewport(0);
		}
	}
}

float AMainPlayerController::GetMouseSentivityX() const
{
	return MouseSensitivityX;
}

float AMainPlayerController::GetMouseSentivityY() const
{
	return MouseSensitivityY;
}

void AMainPlayerController::SetMouseSentivityX(float InSensitivity)
{
	MouseSensitivityX = InSensitivity;
	SaveOptionData();
}

void AMainPlayerController::SetMouseSentivityY(float InSensitivity)
{
	MouseSensitivityY = InSensitivity;
	SaveOptionData();
}

void AMainPlayerController::OnUpdateBindedKey(FName InputName, FKey NewKey)
{
	if (!EnhancedInputUserSettings)
	{
		return;
	}

	// Prepare new key
	FMapPlayerKeyArgs InArgs;
	InArgs.NewKey = NewKey;
	InArgs.MappingName = InputName;
	InArgs.Slot = EPlayerMappableKeySlot::First;
	FGameplayTagContainer FailureReason;

	// Update the key
	EnhancedInputUserSettings->MapPlayerKey(InArgs, FailureReason);


	// Save the inputs
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	GameUserSettings->ApplySettings(true);

	EnhancedInputUserSettings->ApplySettings();
	EnhancedInputUserSettings->SaveSettings();
}

void AMainPlayerController::OnResetBindedKey(FName InputName, FEnhancedActionKeyMapping& ActionKeyMapping, UKeyMappingCommonAW* InWidget)
{
	if (!EnhancedInputUserSettings)
	{
		return;
	}

	// Prepare the current
	FName KeyName = ActionKeyMapping.GetPlayerMappableKeySettings()->Name;
	FMapPlayerKeyArgs CurrentKeyArgs;
	CurrentKeyArgs.MappingName = KeyName;
	CurrentKeyArgs.NewKey = ActionKeyMapping.Key;
	CurrentKeyArgs.Slot = EPlayerMappableKeySlot::First;

	// Get the key profile to find the default data
	UEnhancedPlayerMappableKeyProfile* KeyProfile = EnhancedInputUserSettings->GetCurrentKeyProfile();
	if(!KeyProfile)
	{
		return;
	}

	// Get the default key
	FPlayerKeyMapping* DefaultKeyMapping = KeyProfile->FindKeyMapping(CurrentKeyArgs);
	FKey DefaultKey = DefaultKeyMapping->GetDefaultKey();

	// Set back the default Key
	OnUpdateBindedKey(InputName, DefaultKey);

	// Update back the display
	// In the argument of the function, we already have the required struct. We just need to update it so we can
	// send it back.
	ActionKeyMapping.Key = DefaultKey;
	InWidget->SetInputSelector(ActionKeyMapping);
}

void AMainPlayerController::SaveOptionData()
{
	// Try to get the save
	UOptionSaveGame* OptionSaveGame = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SaveOptionSlotName, 0))
	{
		OptionSaveGame = Cast<UOptionSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveOptionSlotName, 0));
	}
	// If it doesn't exit, create it
	else
	{
		OptionSaveGame = 
			Cast<UOptionSaveGame>(UGameplayStatics::CreateSaveGameObject(UOptionSaveGame::StaticClass()));
	}

	// Update the data in the save file
	OptionSaveGame->SetMouseSensitivityX(MouseSensitivityX);
	OptionSaveGame->SetMouseSensitivityY(MouseSensitivityY);

	// Save the data
	UGameplayStatics::SaveGameToSlot(OptionSaveGame, SaveOptionSlotName, 0);
}

void AMainPlayerController::LoadOptionData()
{
	// Try to get the save
	UOptionSaveGame* OptionSaveGame = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SaveOptionSlotName, 0))
	{
		OptionSaveGame = Cast<UOptionSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveOptionSlotName, 0));
	}
	// If it doesn't exist, we have nothing to load
	else
	{
		return;
	}

	// Load the data
	MouseSensitivityX = OptionSaveGame->GetMouseSensitivityX();
	MouseSensitivityY = OptionSaveGame->GetMouseSensitivityY();
}
