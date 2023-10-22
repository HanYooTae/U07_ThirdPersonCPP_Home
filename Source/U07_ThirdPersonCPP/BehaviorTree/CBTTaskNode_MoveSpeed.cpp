#include "CBTTaskNode_MoveSpeed.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "AIController.h"
#include "Characters/CEnemy_AI.h"

UCBTTaskNode_MoveSpeed::UCBTTaskNode_MoveSpeed()
{
	NodeName = "MoveSpeed";
}

EBTNodeResult::Type UCBTTaskNode_MoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	AAIController* controller = OwnerComp.GetAIOwner();
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(enemy, EBTNodeResult::Failed);

	UCStatusComponent* statusComp = CHelpers::GetComponent<UCStatusComponent>(enemy);
	CheckNullResult(statusComp, EBTNodeResult::Failed);

	statusComp->ChangeMoveSpeed(Type);

	return EBTNodeResult::Succeeded;
}
