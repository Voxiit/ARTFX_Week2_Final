// Victor's Lessons, no right reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TakePickUp.generated.h"


UCLASS()
class UE5INTRODUCTION_API UBTTask_TakePickUp : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

protected:
	EBTNodeResult::Type Failed();

protected:
	TObjectPtr<class AEnemyController> EnemyController;
	TObjectPtr<class AEnemyCharacter> EnemyCharacter;

	float RandomAngleYaw = 0.f;
	float RandomAnglePitch = 0.f;
	AActor* PickUp = nullptr;
	class UGravityGunComponent* GravityGunComp = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pick Up")
	float RandomAngleAroundRaycast = 8.f;
	UPROPERTY(EditAnywhere, Category = "Pick Up")
	float PickUpTimer = 2.f;
	UPROPERTY(EditAnywhere, Category = "Pick Up")
	struct FBlackboardKeySelector CanTakePickUpKey;
	UPROPERTY(EditAnywhere, Category = "Pick Up")
	struct FBlackboardKeySelector PickUpKey;
	UPROPERTY(EditAnywhere, Category = "Pick Up")
	struct FBlackboardKeySelector EnemyHasPickUpKey;
};
