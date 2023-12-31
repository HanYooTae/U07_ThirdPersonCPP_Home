#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CActionComponent.h"
#include "CBTTaskNode_Change.generated.h"

UCLASS()
class U07_THIRDPERSONCPP_API UCBTTaskNode_Change : public UBTTaskNode
{
	GENERATED_BODY()
	
	// ActionComp->SetOOOOMode
	//Execute, Tick

public:
	UCBTTaskNode_Change();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
		EActionType Type;
};
