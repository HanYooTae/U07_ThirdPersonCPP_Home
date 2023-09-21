#include "CBTService_Melee.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Global.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 플레이어가 감지되었을 때, 안되었을 때
	// 감지가 되었을 때 거리에 따라서 BehaviorComp->SetOOOOMode

	// BehaviorTree의 Owner는 Controller
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behaviorComp);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(enemy);

	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(stateComp);

	if (stateComp->IsHittedMode())
	{
		behaviorComp->SetHittedMode();
		return;
	}
	
	// Get Player from BB
	ACPlayer* player = behaviorComp->GetPlayerKey();

	// No Player
	if (player == nullptr)
	{
		behaviorComp->SetWaitMode();
		return;
	}

	// Perceived Player
	float distance = enemy->GetDistanceTo(player);
	behaviorComp->SetApproachMode();
	behaviorComp->SetActionMode();
}