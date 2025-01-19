// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OptionSaveGame.generated.h"


UCLASS()
class UOptionSaveGame : public USaveGame
{
	GENERATED_BODY()
	

// Mouse Sensitivity
public:
	float GetMouseSensitivityX() const;
	float GetMouseSensitivityY() const;
	void SetMouseSensitivityX(float InSensitivity);
	void SetMouseSensitivityY(float InSensitivity);

protected:
	UPROPERTY(SaveGame)
	float MouseSensitivityX = 1.f;
	UPROPERTY(SaveGame)
	float MouseSensitivityY = 1.f;

// End of Mouse Sensitivity
};
