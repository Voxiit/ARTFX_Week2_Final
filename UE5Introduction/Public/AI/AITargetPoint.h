// Victor's Lessons, no right reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AI/AIEnums.h"
#include "AITargetPoint.generated.h"


UCLASS(Abstract)
class UE5INTRODUCTION_API AAITargetPoint : public ATargetPoint
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI Target Point")
	EAIBehaviorType TargetBehaviorType = EAIBehaviorType::None;

public:
	EAIBehaviorType GetTargetBehaviorType() const;
};
