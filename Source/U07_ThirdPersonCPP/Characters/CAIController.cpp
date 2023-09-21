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

	// Todo. (0)Use Blackboard, (1)RunBT, (2)PerceptionComp
	CHelpers::CreateActorComponent(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent(this, &Perception, "Perception");
	// -> PostInitialize().. => 생성자 이후에 호출되는 함수
	// BlackBoard가 Null일 경우 어떤 시도를 함. 그 시도를 차단하기 위해 만드는 함수

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

	// BlackBoardAsset과 BlackBoardComponent를 Pairing해줌
	UseBlackboard(PossessedEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	// BlackboardComponent를 BehaviorComponent로 넘겨줘야 값을 읽고 쓸 수 있음
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
}

// 감지가 풀린 Actor들도 가져오기 때문에 파라미터 사용안했음
// 감지가 되고 있는 Actor들만 사용하려고 함
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	// 현재 감지가 되고 있는 Actor를 검출하는 함수
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
