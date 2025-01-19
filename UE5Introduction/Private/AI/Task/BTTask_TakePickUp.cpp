// Victor's Lessons, no right reserved


#include "AI/Task/BTTask_TakePickUp.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

#include "AI/EnemyController.h"
#include "AI/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameplay/GravityGunComponent.h"

EBTNodeResult::Type UBTTask_TakePickUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	// Get the values from the blackboard
	PickUp = Cast<AActor>(BlackboardComponent->GetValueAsObject(PickUpKey.SelectedKeyName));
	if (!PickUp)
	{
		return EBTNodeResult::Failed;
	}

	// Get gravity Gun
	GravityGunComp = EnemyCharacter->FindComponentByClass<UGravityGunComponent>();
	if (!GravityGunComp)
	{
		return EBTNodeResult::Failed;
	}

	// Check if we're close enough
	const FVector EnemyLocation = EnemyCharacter->GetActorLocation();
	const FVector PickUpLocation = PickUp->GetActorLocation();
	FVector EnemyToPickUp = PickUpLocation - EnemyLocation;
	const float RaycastSize = GravityGunComp->GetRaycastSize();
	if (RaycastSize < EnemyToPickUp.Size())
	{
		return Failed();
	}

	// Add some variation to the raycast
	RandomAnglePitch = FMath::FRandRange(-RandomAngleAroundRaycast, RandomAngleAroundRaycast);
	EnemyToPickUp = EnemyToPickUp.RotateAngleAxis(RandomAnglePitch, EnemyCharacter->GetActorRightVector());

	RandomAngleYaw = FMath::FRandRange(-RandomAngleAroundRaycast, RandomAngleAroundRaycast);
	EnemyToPickUp = EnemyToPickUp.RotateAngleAxis(RandomAngleYaw, EnemyCharacter->GetActorUpVector());

	// Compute check location
	EnemyToPickUp.Normalize();
	FVector CheckLocation = EnemyLocation + (EnemyToPickUp * RaycastSize);

	// Launch the ray
	if (!GravityGunComp->TryToTakePickUp(EnemyLocation, CheckLocation))
	{
		return Failed();
	}

	// We've got the pick up ! Update the blackboard Value
	BlackboardComponent->SetValueAsBool(EnemyHasPickUpKey.SelectedKeyName, true);
	return EBTNodeResult::Succeeded;
}

void UBTTask_TakePickUp::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	FString PickUpName = PickUp ? PickUp->GetName() : "NONE";
	Values.Add(FString::Printf(TEXT("Pick Up : %s - Random Angle Pitch %f - Random Angle Yaw %f"), 
		*PickUpName, RandomAnglePitch, RandomAngleYaw));

	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
}

FString UBTTask_TakePickUp::GetStaticDescription() const
{
	FString Description("invalid");
	// Check if the keys given by the user are valid
	if (CanTakePickUpKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass() &&
		PickUpKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		Description = "Pick Up : " + PickUpKey.SelectedKeyName.ToString() + " - Can Take Pick Up : " +
			CanTakePickUpKey.SelectedKeyName.ToString() + FString(TEXT(" - Pick Up Timer %f"), PickUpTimer)
			+ FString(TEXT(" - Pick Up Random Angle %f"), RandomAngleAroundRaycast);
	}

	return FString::Printf(TEXT("%s : %s"), *Super::GetStaticDescription(), *Description);
}

EBTNodeResult::Type UBTTask_TakePickUp::Failed()
{
	// Launch the timer
	EnemyController->StartCanTakePickUpTimer(PickUpTimer);
	return EBTNodeResult::Failed;
}
