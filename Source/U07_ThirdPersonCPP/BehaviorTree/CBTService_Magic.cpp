#include "CBTService_Magic.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CPatrolComponent.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Global.h"

UCBTService_Magic::UCBTService_Magic()
{
	NodeName = "Magic";
}

void UCBTService_Magic::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

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
		behaviorComp->SetWaitMode();
		controller->ClearFocus(EAIFocusPriority::Gameplay);
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
	
	// yawÈ¸Àü¸¸ ½ÃÅ´
	controller->SetFocus(player);

	//-> Is in Warp(Avoid) Range (400)
	if (distance < controller->GetBehaviorRange())
	{
		behaviorComp->SetAvoidMode();
		return;
	}

	//-> Is in Action Range (1000)
	if (distance < controller->GetSightRadius())
	{
		behaviorComp->SetActionMode();
		return;
	}
}