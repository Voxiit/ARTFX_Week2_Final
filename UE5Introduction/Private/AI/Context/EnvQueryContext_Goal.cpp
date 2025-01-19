// Victor's Lessons, no right reserved


#include "AI/Context/EnvQueryContext_Goal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Goal.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_Goal::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	// Check if we've got a querier
	UObject* QuerierObject = QueryInstance.Owner.Get();
	if (!QuerierObject)
	{
		return;
	}

	// Do we have the goal BP
	if (!GoalBP)
	{
		return;
	}

	// Try to get the goal with the correct behavior
	TArray<AActor*> GoalArray, GoalAIBehaviorArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GoalBP, GoalArray);
	if (GoalArray.IsEmpty())
	{
		return;
	}

	// Look for our goals
	for (AActor* Goal : GoalArray)
	{
		if (AGoal* GoalCasted = Cast<AGoal>(Goal))
		{
			if (GoalCasted->GetBehaviorType() == GoalBehaviorType)
			{
				GoalAIBehaviorArray.Add(GoalCasted);
			}
		}
	}

	// Send the goal we found
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, GoalAIBehaviorArray);
}
