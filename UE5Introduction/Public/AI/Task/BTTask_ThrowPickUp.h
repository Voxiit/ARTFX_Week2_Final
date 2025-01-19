// Victor's Lessons, no right reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ThrowPickUp.generated.h"


UCLASS()
class UE5INTRODUCTION_API UBTTask_ThrowPickUp : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

protected:
	TObjectPtr<class AEnemyController> EnemyController;
	TObjectPtr<class AEnemyCharacter> EnemyCharacter;

	class UGravityGunComponent* GravityGunComp = nullptr;
	AActor* PlayerGoal = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pick Up")
	float PickUpTimer = 2.f;
	UPROPERTY(EditAnywhere, Category = "Pick Up")
	struct FBlackboardKeySelector EnemyHasPickUpKey;
	UPROPERTY(EditAnywhere, Category = "Pick Up")
	struct FBlackboardKeySelector PlayerGoalKey;
};
