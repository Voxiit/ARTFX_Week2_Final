// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"

#include "EnhancedActionKeyMapping.h"

#include "KeyMappingCommonAW.generated.h"


UCLASS(Abstract)
class UE5INTRODUCTION_API UKeyMappingCommonAW : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	class AMainPlayerController* PlayerController = nullptr;

// Keybindings
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class UInputKeySelector* BIND_InputKeySelector = nullptr;
	FName InputName;
	FEnhancedActionKeyMapping DisplayedKey;

	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonTextBlock* BIND_InputDisplayName = nullptr;

protected:
	UFUNCTION()
	void OnKeySelected(struct FInputChord SelectedKey);

public:
	void SetInputName(FName InName);
	void SetInputSelector(FEnhancedActionKeyMapping& InKeyMapping);
	void SetDisplayName(FText InText);

// End of Keybindings


// Reset
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class UMainCommonButtonBase* BIND_ResetButton = nullptr;

protected:
	UFUNCTION()
	void OnResetButtonClicked();

// End of Reset
};
