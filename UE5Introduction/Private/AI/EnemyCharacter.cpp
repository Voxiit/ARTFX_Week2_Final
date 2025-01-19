// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyCharacter.h"
#include "Gameplay/Goal.h"

AEnemyCharacter::AEnemyCharacter()
{
 	PrimaryActorTick.bCanEverTick = false;
}

AGoal* AEnemyCharacter::GetPlayerGoal() const
{
    return PlayerGoal;
}

AGoal* AEnemyCharacter::GetEnemyGoal() const
{
    return EnemyGoal;
}

