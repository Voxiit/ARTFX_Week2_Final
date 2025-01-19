// Victor's Lessons, no right reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "AI/AIEnums.h"
#include "EnvQueryContext_Goal.generated.h"


UCLASS(Abstract, Blueprintable, MinimalAPI)
class UEnvQueryContext_Goal : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const;

	UPROPERTY(EditAnywhere, Category = "Goal")
	TSubclassOf<AActor> GoalBP = nullptr;
	UPROPERTY(EditAnywhere, Category = "Goal")
	EAIBehaviorType GoalBehaviorType = EAIBehaviorType::None;
};
