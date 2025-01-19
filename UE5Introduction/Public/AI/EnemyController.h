// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/AIEnums.h"
#include "AI/AISpeedDataAsset.h"
#include "EnemyController.generated.h"


UCLASS(Abstract)
class UE5INTRODUCTION_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

// Components
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller|Behavior Tree")
	class UBehaviorTree* EnemyBehaviorTree = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller|Behavior Tree|Blackboard")
	FName PlayerHasPickUpName = "PlayerHasPickUp";
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller|Behavior Tree|Blackboard")
	FName EnemyIsInDefenseSphereName = "EnemyIsInDefenseSphere";
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller|Behavior Tree|Blackboard")
	FName EnemyIsInAttackSphereName = "EnemyIsInAttackSphere";
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller|Behavior Tree|Blackboard")
	FName AttackGoalName = "AttackGoal";
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller|Behavior Tree|Blackboard")
	FName DefenseGoalName = "DefenseGoal";

protected:
	UFUNCTION()
	void OnPlayerHasPickUp(bool bInPlayerHasPickUp);

	UFUNCTION()
	void OnActorOverlapAISphere(bool bIsOverlaped, EAIBehaviorType BehaviorType, AActor* OverlapedActor);

// End of Components


// Speed
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy Controller|Speed")
	UAISpeedDataAsset* SpeedDataAsset = nullptr;

public:
	float GetSpeedByType(EAISpeedType InType) const;

// End of Speed


// AI Sphere
protected:
	UPROPERTY(EditAnywhere, Category = "Enemy Controller|Defense")
	float MaxDistanceFromGoal = 500.f;

public:
	float GetMaxDistanceFromGoal() const;


// End of AI Sphere


// Pick Up Timer
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller|Behavior Tree|Blackboard")
	FName CanTakePickUpName = "CanTakePickUp";
	FTimerHandle CanTakePickUpTimerHandle;

protected:
	void OnCanTakePickUpTimerOver();

public:
	void StartCanTakePickUpTimer(float InTime);

// End of Pick Up Timer
};
