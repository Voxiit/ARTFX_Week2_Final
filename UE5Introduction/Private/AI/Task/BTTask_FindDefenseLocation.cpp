// Victor's Lessons, no right reserved


#include "AI/Task/BTTask_FindDefenseLocation.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

#include "AI/EnemyController.h"
#include "AI/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_FindDefenseLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	EnemyGoal = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyGoalKey.SelectedKeyName));
	if (!EnemyGoal)
	{
		return EBTNodeResult::Failed;
	}

	// Get dist from goal from controller
	DefenseDistFromGoal = EnemyController->GetMaxDistanceFromGoal();

	// Try to find a defense location
	if (!FindDefenseLocation(DefenseDistFromGoal, DefenseLocation))
	{
		return EBTNodeResult::Failed;
	}

	// Set the defense location found in the blackboard
	BlackboardComponent->SetValueAsVector(DefenseLocationGoalKey.SelectedKeyName, DefenseLocation);
	return EBTNodeResult::Succeeded;
}

void UBTTask_FindDefenseLocation::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Values.Add(FString::Printf(TEXT("Defense Location Found is %s"), *DefenseLocation.ToString()));

	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
}

FString UBTTask_FindDefenseLocation::GetStaticDescription() const
{
	FString Description("invalid");
	// Check if the keys given by the user are valid
	if (DefenseLocationGoalKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass() && 
		EnemyGoalKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		Description = "Goal : " + EnemyGoalKey.SelectedKeyName.ToString() + " - Defense Location : " +
			DefenseLocationGoalKey.SelectedKeyName.ToString();
	}
	
	return FString::Printf(TEXT("%s : %s"), *Super::GetStaticDescription(), *Description);
}

bool UBTTask_FindDefenseLocation::FindDefenseLocation(float InMaxDistFromGoal, FVector& OutDefenseLocation)
{
	// Get the Player
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter)
	{
		return false;
	}

	// Get Each Actor Location
	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const FVector GoalLocation = EnemyGoal->GetActorLocation();

	// Prepare our vectors
	const FVector GoalToPlayer = PlayerLocation - GoalLocation;
	FVector GoalToPlayerNormalized = GoalToPlayer;
	GoalToPlayerNormalized.Normalize();

	// Try to place the Enemy in the middle
	OutDefenseLocation = GoalLocation + (GoalToPlayerNormalized * (GoalToPlayer.Size() / 2.f));

	// If the Enemy is above InMaxDistFromGoal, we'll stick to this distance
	if (FVector::Distance(GoalLocation, OutDefenseLocation) > InMaxDistFromGoal)
	{
		OutDefenseLocation = GoalLocation + (GoalToPlayerNormalized * InMaxDistFromGoal);
	}

	return true;
}
