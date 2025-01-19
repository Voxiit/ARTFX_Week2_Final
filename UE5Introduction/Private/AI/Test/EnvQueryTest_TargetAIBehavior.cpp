// Victor's Lessons, no right reserved


#include "AI/Test/EnvQueryTest_TargetAIBehavior.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "AI/AITargetPoint.h"

// We need to create the constructor in the cpp
UEnvQueryTest_TargetAIBehavior::UEnvQueryTest_TargetAIBehavior(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Indicate the test's cost and return item type
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();

	// Indicate if we're going to use float values
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_TargetAIBehavior::RunTest(FEnvQueryInstance& QueryInstance) const
{
	// Check if we've got an owner
	UObject* QuerierOwner = QueryInstance.Owner.Get();
	if (!QuerierOwner)
	{
		return;
	}

	// We need to prepare this value so we can use the user's info
	BoolValue.BindData(QuerierOwner, QueryInstance.QueryID);
	bool bWantsValid = BoolValue.GetValue();

	// Try to cast each instance as a Target Point
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		if (const AAITargetPoint* TargetPoint = Cast<AAITargetPoint>(GetItemActor(QueryInstance, It.GetIndex())))
		{
			// Check if we have the correct Behavior Type
			if (TargetPoint->GetTargetBehaviorType() == TargetBehaviorType)
			{
				// Correct one, it passed
				It.SetScore(TestPurpose, FilterType, true, bWantsValid);
			}
			else
			{
				// Not correct one, it failed
				It.ForceItemState(EEnvItemStatus::Failed);
			}
		}
	}
}

FText UEnvQueryTest_TargetAIBehavior::GetDescriptionTitle() const
{
	// Get the current description
	FString BehaviorString;
	switch (TargetBehaviorType)
	{
	case EAIBehaviorType::None:
		BehaviorString = "Nothing is selected";
		break;
	
	case EAIBehaviorType::Attack:
		BehaviorString = "Attack Behavior";
		break;
	
	case EAIBehaviorType::Defense:
		BehaviorString = "Defense Behavior";
		break;

	default:
		BehaviorString = "Nothing is selected";
		break;
	}
	
	// Add the parent's one with it
	return FText::FromString(FString::Printf(TEXT("%s : %s"), 
		*Super::GetDescriptionTitle().ToString(), *BehaviorString));
}

FText UEnvQueryTest_TargetAIBehavior::GetDescriptionDetails() const
{
	return DescribeBoolTestParams("behavior");
}
