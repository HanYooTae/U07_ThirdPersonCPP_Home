#include "CAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Todo. (1)RunBT, (2)PerceptionComp
	CHelpers::CreateActorComponent(this, &Blackboard, "Blackboard");
	// -> PostInitialize().. => 생성자 이후에 호출되는 함수
	// BlackBoard가 Null일 경우 어떤 시도를 함. 그 시도를 차단하기 위해 만드는 함수
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
