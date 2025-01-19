// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS(Abstract)
class UE5INTRODUCTION_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();


// Goal
protected:
	UPROPERTY(EditInstanceOnly, Category = "Enemy Character|Goal")
	class AGoal* PlayerGoal = nullptr;
	UPROPERTY(EditInstanceOnly, Category = "Enemy Character|Goal")
	class AGoal* EnemyGoal = nullptr;

public:
	class AGoal* GetPlayerGoal() const;
	class AGoal* GetEnemyGoal() const;

// End of Goal
};
