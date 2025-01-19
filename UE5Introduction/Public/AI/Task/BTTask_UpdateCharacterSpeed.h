// Victor's Lessons, no right reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI/AIEnums.h"
#include "BTTask_UpdateCharacterSpeed.generated.h"


UCLASS()
class UE5INTRODUCTION_API UBTTask_UpdateCharacterSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

protected:
	TObjectPtr<class AEnemyController> EnemyController;
	TObjectPtr<class AEnemyCharacter> EnemyCharacter;

	float CharacterSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	EAISpeedType SpeedType = EAISpeedType::ReachPickUp;
};
