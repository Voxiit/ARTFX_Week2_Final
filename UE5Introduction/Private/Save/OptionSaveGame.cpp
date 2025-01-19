// Fill out your copyright notice in the Description page of Project Settings.


#include "Save/OptionSaveGame.h"

float UOptionSaveGame::GetMouseSensitivityX() const
{
	return MouseSensitivityX;
}

float UOptionSaveGame::GetMouseSensitivityY() const
{
	return MouseSensitivityY;
}

void UOptionSaveGame::SetMouseSensitivityX(float InSensitivity)
{
	MouseSensitivityX = InSensitivity;
}

void UOptionSaveGame::SetMouseSensitivityY(float InSensitivity)
{
	MouseSensitivityY = InSensitivity;
}
