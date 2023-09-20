#include "CAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CBehaviorComponent.h"
#include "CEnemy_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Todo. (0)Use Blackboard, (1)RunBT, (2)PerceptionComp
	CHelpers::CreateActorComponent(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent(this, &Perception, "Perception");
	// -> PostInitialize().. => ������ ���Ŀ� ȣ��Ǵ� �Լ�
	// BlackBoard�� Null�� ��� � �õ��� ��. �� �õ��� �����ϱ� ���� ����� �Լ�

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600.f;
	Sight->LoseSightRadius = 800.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;
	Sight->SetMaxAge(2.f);
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;


	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(Sight->GetSenseImplementation());	// == Sight->GetClass()
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedEnemy = Cast<ACEnemy_AI>(InPawn);

	// BlackBoardAsset�� BlackBoardComponent�� Pairing����
	UseBlackboard(PossessedEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	// BlackboardComponent�� BehaviorComponent�� �Ѱ���� ���� �а� �� �� ����
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(PossessedEnemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
