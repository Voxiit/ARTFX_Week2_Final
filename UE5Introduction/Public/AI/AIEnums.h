// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIEnums.generated.h"

UENUM()
enum class EAISpeedType : uint8
{
	Attack,
	Defense,
	ReachPickUp,
	MAX UMETA(Hidden),
};

UENUM()
enum class EAIBehaviorType : uint8
{
	None,
	Attack,
	Defense,
	MAX UMETA(Hidden),
};