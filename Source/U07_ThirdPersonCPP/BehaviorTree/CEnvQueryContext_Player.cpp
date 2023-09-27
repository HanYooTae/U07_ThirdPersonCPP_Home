#include "CEnvQueryContext_Player.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"

void UCEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	// QueryInstance : EQS_Avoid Asset
	// QueryInstance.Owner = ACEnemy_AI

	// BB
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());	// Enemy -> Controller -> BehaviorComp::GetPlayerKey
	CheckNull(enemy);

	ACAIController* controller = Cast<ACAIController>(enemy->GetController());
	CheckNull(controller);

	UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behaviorComp);

	ACPlayer* player = behaviorComp->GetPlayerKey();
	CheckNull(player);

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, player);
}
