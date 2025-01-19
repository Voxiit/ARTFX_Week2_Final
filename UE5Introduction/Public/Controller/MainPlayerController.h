// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"


UCLASS()
class UE5INTRODUCTION_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;

// Character
protected:
	class AMainPlayer* Character = nullptr;
	TWeakObjectPtr<class UGravityGunController> GravityGunController = nullptr;

// Inputs
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Movement")
	class UInputAction* InputActionMove;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Look")
	class UInputAction* InputActionLook;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Movement")
	class UInputAction* InputActionJump;

protected:
	void MovePlayer(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void Jump();

// End of Inputs


// Mouse Sensitivity
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sensitivity", meta = (ToolTip = "Change the X axis sensitivity.", ClampMin = "0.1", ClampMax = "3.0"))
	float MouseSensitivityX = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sensitivity", meta = (ToolTip = "Change the Y axis sensitivity.", ClampMin = "0.1", ClampMax = "3.0"))
	float MouseSensitivityY = 1.f;

public:
	virtual void AddPitchInput(float Value) override;
	virtual void AddYawInput(float Value) override;

// End of Mouse Sensitivity


// Exercice 3
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Score")
	class UInputAction* InputActionScore;

protected:
	void OnDisplayScoreInputPressed();

// End of Exercice 3


// Exercice 4
protected:
	TWeakObjectPtr<class UScoreComponent> ScoreComponent = nullptr;

// End of Exercice 4


// Noted Exercice
protected:
	TWeakObjectPtr<class USpawnerController> SpawnerController = nullptr;

// End of Noted Exercice


// Pause
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Pause")
	class UInputAction* InputActionPause;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Pause")
	TSubclassOf<class UPauseMenuCommonAW> PauseMenuWidget = nullptr;

protected:
	void OnPauseInputPressed();

// End of Pause


// Mouse Sensitivity Option Menu
public:
	float GetMouseSentivityX() const;
	float GetMouseSentivityY() const;

	void SetMouseSentivityX(float InSensitivity);
	void SetMouseSentivityY(float InSensitivity);

// Mouse Sensitivity Option Menu


// Keybindings
protected:
	class UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = nullptr;
	class UEnhancedInputUserSettings* EnhancedInputUserSettings = nullptr;

public:
	void OnUpdateBindedKey(FName InputName, FKey NewKey);
	void OnResetBindedKey(FName InputName, struct FEnhancedActionKeyMapping& ActionKeyMapping, class UKeyMappingCommonAW* InWidget);

// End of Keybindings


// Save Data
protected:
	UPROPERTY(EditAnywhere, Category = "Save")
	FString SaveOptionSlotName = "OptionSaveSlot";

public:
	void SaveOptionData();
	void LoadOptionData();

// End of Save Data
};
