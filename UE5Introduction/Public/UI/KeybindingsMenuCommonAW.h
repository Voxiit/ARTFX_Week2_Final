// Victor's Lessons, no right reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "KeybindingsMenuCommonAW.generated.h"



UCLASS(Abstract)
class UE5INTRODUCTION_API UKeybindingsMenuCommonAW : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	void OpenMenu();

protected:
	TWeakObjectPtr<class AMainPlayerController> PlayerController = nullptr;


// Button
protected:
	UFUNCTION()
	void OnExitMenuButtonClicked();

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class UMainCommonButtonBase* BIND_ExitButton;

// End of Button


// Keybindings
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class UVerticalBox* BIND_MovementKeyVerticalBox;
	UPROPERTY(meta = (BindWidgetOptional))
	class UVerticalBox* BIND_ActionKeyVerticalBox;

	UPROPERTY(EditAnywhere, Category = "Keybinding User Widget")
	TSubclassOf<class UKeyMappingCommonAW> KeybindingsWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "Keybinding User Widget")
	FString MovementCategoryName = FString("Movement");
	UPROPERTY(EditAnywhere, Category = "Keybinding User Widget")
	FString ActionCategoryName = FString("Action");

protected:
	void DisplayMappableKeys();

// End of Keybindings


// Navigation
protected:
	void UpdateMenuNavigation();

public:
	UFUNCTION()
	void OnKeyButtonPressed();

protected:
	TArray<class UKeyMappingCommonAW*> MovementKeyMapping;
	TArray<class UKeyMappingCommonAW*> ActionKeyMapping;

// End of Navigation
};
