// Victor's Lessons, no right reserved


#include "AI/Task/BTTask_UpdateCharacterSpeed.h"
#include "AI/EnemyController.h"
#include "AI/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_UpdateCharacterSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	// Get the speed
	CharacterSpeed = EnemyController->GetSpeedByType(SpeedType);

	// Apply the speed
	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = CharacterSpeed;

	return EBTNodeResult::Succeeded;
}

void UBTTask_UpdateCharacterSpeed::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Values.Add(FString::Printf(TEXT("Character Speed is %f"), CharacterSpeed));

	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
}

FString UBTTask_UpdateCharacterSpeed::GetStaticDescription() const
{
	FString EnumString;
	UEnum::GetValueAsString<EAISpeedType>(SpeedType, EnumString);

	return FString::Printf(TEXT("%s : %s"), *Super::GetStaticDescription(), *EnumString);
}
