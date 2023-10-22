#include "CBTTaskNode_Action.h"
#include "Global.h"
#include "AIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* controller = OwnerComp.GetAIOwner();
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(enemy, EBTNodeResult::Failed);

	UCActionComponent* ActionComp = CHelpers::GetComponent<UCActionComponent>(enemy);
	CheckNullResult(ActionComp, EBTNodeResult::Failed);

	ActionComp->DoAction();

	RunningTime = 0;

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 상태가 Idle && n초가 경과했다면 성공 리턴
	AAIController* controller = OwnerComp.GetAIOwner();
	CheckNull(controller);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(enemy);

	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(stateComp);

	RunningTime += DeltaSeconds;

	if (stateComp->IsIdleMode() && RunningTime >= Delay)
		// 실행을 끝내는 함수
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
