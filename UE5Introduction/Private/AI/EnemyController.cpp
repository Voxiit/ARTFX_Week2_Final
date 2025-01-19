// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyController.h"
#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Player/MainPlayer.h"
#include "Gameplay/GravityGunComponent.h"
#include "Gameplay/Goal.h"
#include "Components/SphereComponent.h"

#include "AI/EnemyCharacter.h"


AEnemyController::AEnemyController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Start the Enemy Behavior Tree
    if (EnemyBehaviorTree)
    {
        RunBehaviorTree(EnemyBehaviorTree);
    }
}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();

    // Get the gravity gun component from the player and bind to its event
    if (AMainPlayer* MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        if (UGravityGunComponent* GravityGunComponent = MainPlayer->FindComponentByClass<UGravityGunComponent>())
        {
            GravityGunComponent->OnPlayerHasPickUp.AddUniqueDynamic(this, &AEnemyController::OnPlayerHasPickUp);
        }
    }

    // Goal
    if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetCharacter()))
    {
        // Get goals
        AGoal* PlayerGoal = EnemyCharacter->GetPlayerGoal();
        AGoal* EnemyGoal = EnemyCharacter->GetEnemyGoal();
        
        if (PlayerGoal && EnemyGoal && Blackboard)
        {
            // Set blackboard values for goal
            Blackboard->SetValueAsObject(AttackGoalName, PlayerGoal);
            Blackboard->SetValueAsObject(DefenseGoalName, EnemyGoal);

            // Bind on goal overlap event
            PlayerGoal->OnAISphereOverlap.AddUniqueDynamic(this, &AEnemyController::OnActorOverlapAISphere);
            EnemyGoal->OnAISphereOverlap.AddUniqueDynamic(this, &AEnemyController::OnActorOverlapAISphere);
        }

        // Check if the AI is inside a Goal's Sphere at the Begin Play
        TArray<AActor*> OverlappingActors;
        USphereComponent* PlayerGoalSphere = PlayerGoal->GetCollisionSphere();
        PlayerGoalSphere->GetOverlappingActors(OverlappingActors, AEnemyCharacter::StaticClass());
        if (!OverlappingActors.IsEmpty())
        {
            Blackboard->SetValueAsBool(EnemyIsInAttackSphereName, true);
        }

        USphereComponent* EnemyGoalSphere = EnemyGoal->GetCollisionSphere();
        EnemyGoalSphere->GetOverlappingActors(OverlappingActors, AEnemyCharacter::StaticClass());
        if (!OverlappingActors.IsEmpty())
        {
            Blackboard->SetValueAsBool(EnemyIsInDefenseSphereName, true);
        }
    }

    // At the beginning, the AI can take the pick up
    Blackboard->SetValueAsBool(CanTakePickUpName, true);
}

void AEnemyController::OnPlayerHasPickUp(bool bInPlayerHasPickUp)
{
    // Update Blackboard Values
    if(Blackboard)
    {
        Blackboard->SetValueAsBool(PlayerHasPickUpName, bInPlayerHasPickUp);
    }
}

void AEnemyController::OnActorOverlapAISphere(bool bIsOverlaped, EAIBehaviorType BehaviorType, AActor* OverlapedActor)
{
    // Check if it's the enemy
    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OverlapedActor);
    if (!Enemy)
    {
        return;
    }

    // Update the correct blackboard value
    if (!Blackboard)
    {
        return;
    }
    Blackboard->SetValueAsBool(BehaviorType == EAIBehaviorType::Attack ? EnemyIsInAttackSphereName : 
        EnemyIsInDefenseSphereName, bIsOverlaped);
}

float AEnemyController::GetSpeedByType(EAISpeedType InType) const
{
    if (!SpeedDataAsset)
    {
        return 0.0f;
    }

    return SpeedDataAsset->SpeedTypeArray[(uint8)InType];
}

float AEnemyController::GetMaxDistanceFromGoal() const
{
    return MaxDistanceFromGoal;
}

void AEnemyController::OnCanTakePickUpTimerOver()
{
    // Update back the bool on the blackboard
    if (Blackboard)
    {
        Blackboard->SetValueAsBool(CanTakePickUpName, true);
    }
}

void AEnemyController::StartCanTakePickUpTimer(float InTime)
{
    if (!Blackboard)
    {
        return;
    }

    // Update its blackboard Value
    Blackboard->SetValueAsBool(CanTakePickUpName, false);

    // Launch the timer
    FTimerManager& TimerManager = GetCharacter()->GetWorldTimerManager();
    TimerManager.ClearTimer(CanTakePickUpTimerHandle);
    TimerManager.SetTimer(CanTakePickUpTimerHandle, this, &AEnemyController::OnCanTakePickUpTimerOver,
        InTime, false);
}
