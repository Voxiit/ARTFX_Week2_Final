// Victor's Lessons, no right reserved


#include "AI/Task/BTTask_ThrowPickUp.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

#include "AI/EnemyController.h"
#include "AI/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameplay/GravityGunComponent.h"

EBTNodeResult::Type UBTTask_ThrowPickUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get Character and Controller
	if (EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner()))
	{
		EnemyCharacter = Cast<AEnemyCharacter>(EnemyController->GetPawn());
	}

	// Check if we've got the character
	if (!EnemyCharacter)
	{
		return EBTNodeResult::Failed;
	}

	// Get the blackboard
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return EBTNodeResult::Failed;
	}

	// Get gravity Gun
	GravityGunComp = EnemyCharacter->FindComponentByClass<UGravityGunComponent>();
	if (!GravityGunComp)
	{
		return EBTNodeResult::Failed;
	}

	// Get the values from the blackboard
	PlayerGoal = Cast<AActor>(BlackboardComponent->GetValueAsObject(PlayerGoalKey.SelectedKeyName));
	if (!PlayerGoal)
	{
		return EBTNodeResult::Failed;
	}

	// Check if there's no obstacles
	const FVector RaycastStart = EnemyCharacter->GetActorLocation();
	const FVector RaycastEnd = PlayerGoal->GetActorLocation();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(EnemyCharacter);
	Params.AddIgnoredActor(PlayerGoal);
	FHitResult RaycastHit;

	// Look for an obstacle. If ther's something, we won't shoot
	if (GetWorld()->LineTraceSingleByChannel(RaycastHit, RaycastStart, RaycastEnd, ECC_Visibility, Params))
	{
		return EBTNodeResult::Failed;
	}

	// Launch pick up and timer
	GravityGunComp->ReleasePickUp(true);
	EnemyController->StartCanTakePickUpTimer(PickUpTimer);

	// Update the bb values
	BlackboardComponent->SetValueAsBool(EnemyHasPickUpKey.SelectedKeyName, false);
	return EBTNodeResult::Succeeded;
}

FString UBTTask_ThrowPickUp::GetStaticDescription() const
{
	FString Description("invalid");
	// Check if the keys given by the user are valid
	if (EnemyHasPickUpKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass() &&
		PlayerGoalKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		Description = "Enemy Has Pick Up : " + EnemyHasPickUpKey.SelectedKeyName.ToString() + " - Player Goal : " +
			PlayerGoalKey.SelectedKeyName.ToString() + FString(TEXT(" - Pick Up Timer %f"), PickUpTimer);
	}

	return FString::Printf(TEXT("%s : %s"), *Super::GetStaticDescription(), *Description);
}
