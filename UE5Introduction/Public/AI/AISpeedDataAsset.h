// Victor's Lessons, no right reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AI/AIEnums.h"
#include "AISpeedDataAsset.generated.h"


UCLASS()
class UE5INTRODUCTION_API UAISpeedDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Speed Asset")
	float SpeedTypeArray[(uint8)EAISpeedType::MAX];
};
