// Victor's Lessons, no right reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MainMenuCommonAW.generated.h"


UCLASS(Abstract)
class UE5INTRODUCTION_API UMainMenuCommonAW : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	void OpenMainMenu();

	TWeakObjectPtr<class AMainPlayerController> PlayerController = nullptr;

// Button
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class UMainCommonButtonBase* BIND_StartButton = nullptr;
	UPROPERTY(meta = (BindWidgetOptional))
	class UMainCommonButtonBase* BIND_OptionButton = nullptr;
	UPROPERTY(meta = (BindWidgetOptional))
	class UMainCommonButtonBase* BIND_QuitButton = nullptr;

protected:
	UFUNCTION()
	void OnStartClicked();
	UFUNCTION()
	void OnOptionClicked();
	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void OnOptionClosed(UUserWidget* WidgetClosed);

	UPROPERTY(EditAnywhere, Category = "Main Menu")
	TSubclassOf<UUserWidget> OptionWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Main Menu")
	FName MapName = "MainWorld";

// End of Button
};
