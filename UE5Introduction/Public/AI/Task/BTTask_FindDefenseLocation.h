// Victor's Lessons, no right reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindDefenseLocation.generated.h"


UCLASS()
class UE5INTRODUCTION_API UBTTask_FindDefenseLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

protected:
	bool FindDefenseLocation(float InMaxDistFromGoal, FVector& OutDefenseLocation);

protected:
	TObjectPtr<class AEnemyController> EnemyController;
	TObjectPtr<class AEnemyCharacter> EnemyCharacter;

	AActor* EnemyGoal = nullptr;
	FVector DefenseLocation = FVector::ZeroVector;
	float DefenseDistFromGoal = -1.f;

	UPROPERTY(EditAnywhere, Category = "Defense")
	struct FBlackboardKeySelector EnemyGoalKey;
	UPROPERTY(EditAnywhere, Category = "Defense")
	struct FBlackboardKeySelector DefenseLocationGoalKey;
};
