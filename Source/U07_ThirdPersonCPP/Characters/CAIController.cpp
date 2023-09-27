#include "CAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CBehaviorComponent.h"
#include "CEnemy_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "CPlayer.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// (0)Use Blackboard, (1)RunBT, (2)PerceptionComp
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

	SetGenericTeamId(PossessedEnemy->GetTeamID());

	RunBehaviorTree(PossessedEnemy->GetBehaviorTree());

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear();
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(bDrawDebug);

	FVector center = PossessedEnemy->GetActorLocation();

	DrawDebugSphere(GetWorld(), center, Sight->SightRadius, 30, FColor::Green);
	DrawDebugSphere(GetWorld(), center, BehaviorRange, 30, FColor::Red);
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

// ������ Ǯ�� Actor�鵵 �������� ������ �Ķ���� ��������
// ������ �ǰ� �ִ� Actor�鸸 ����Ϸ��� ��
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	// ���� ������ �ǰ� �ִ� Actor�� �����ϴ� �Լ�
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	ACPlayer* player = nullptr;
	for (const auto& perceivedactor : actors)
	{
		player = Cast<ACPlayer>(perceivedactor);

		if (!!player)
			break;
	}

	Blackboard->SetValueAsObject("PlayerKey", player);
}
