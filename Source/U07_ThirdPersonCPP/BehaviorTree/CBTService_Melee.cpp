#include "CBTService_Melee.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CPatrolComponent.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Global.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";
}

// �÷��̾ �����Ǿ��� ��, �ȵǾ��� ��
// ������ �Ǿ��� �� �Ÿ��� ���� BehaviorComp->SetOOOOMode
void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// BehaviorTree�� Owner�� Controller
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behaviorComp);

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(enemy);

	UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(stateComp);

	UCPatrolComponent* patrolComp = CHelpers::GetComponent<UCPatrolComponent>(enemy);
	CheckNull(patrolComp);

	if (stateComp->IsHittedMode())
	{
		behaviorComp->SetHittedMode();
		return;
	}
	
	// Get Player from BB
	ACPlayer* player = behaviorComp->GetPlayerKey();

	// No Perceived Player
	if (player == nullptr)
	{
		if (!!patrolComp->IsPathValid())
		{
			behaviorComp->SetPatrolMode();
			return;
		}

		behaviorComp->SetWaitMode();
		return;
	}

	// Perceived Player
	UCStateComponent* playerStateComp = CHelpers::GetComponent<UCStateComponent>(player);
	if (!!playerStateComp)
	{
		// Player is Dead
		if (playerStateComp->IsDeadMode())
		{
			if (!!patrolComp->IsPathValid())
			{
				behaviorComp->SetPatrolMode();
				return;
			}

			behaviorComp->SetWaitMode();
			return;
		}
	}

	// -> Get Distance to Player
	float distance = enemy->GetDistanceTo(player);

	//-> Is in Attack Range (150)
	if (distance < controller->GetBehaviorRange())
	{
		behaviorComp->SetActionMode();
		return;
	}

	//-> Is in Sight Range (600)
	if (distance < controller->GetSightRadius())
	{
		behaviorComp->SetApproachMode();
		return;
	}
}