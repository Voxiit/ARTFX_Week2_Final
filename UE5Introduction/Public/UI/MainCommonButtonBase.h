// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "MainCommonButtonBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

UCLASS(Abstract)
class UE5INTRODUCTION_API UMainCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;


// Text
protected:
	UPROPERTY(EditAnywhere, Category = "Main Button")
	FText ButtonText = FText();

	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonTextBlock* BIND_MainCommonText = nullptr;

// End of Text


// Event
public:
	FOnButtonClicked OnButtonClicked;

protected:
	virtual void NativeOnClicked() override;

// End of Event
};
