// Victor's Lessons, no right reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "AI/AIEnums.h"
#include "EnvQueryTest_TargetAIBehavior.generated.h"


UCLASS(MinimalAPI)
class UEnvQueryTest_TargetAIBehavior : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Target")
	EAIBehaviorType TargetBehaviorType = EAIBehaviorType::None;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;

};
