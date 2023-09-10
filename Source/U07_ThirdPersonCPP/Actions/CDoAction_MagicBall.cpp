#include "CDoAction_MagicBall.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "CAim.h"
#include "CMagicBall.h"

void ACDoAction_MagicBall::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);
}


void ACDoAction_MagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim->Tick(DeltaTime);
}

void ACDoAction_MagicBall::DoAction()
{
	CheckFalse(Datas.Num() > 0);

	// Player 전용(Zoom을 하지 않으면 MagicBall사용 불가)
	if (Aim->IsAvailable())
		CheckFalse(Aim->IsZooming());

	CheckFalse(StateComp->IsIdleMode());
	StateComp->SetActionMode();

	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
}

void ACDoAction_MagicBall::Begin_DoAction()
{
	// Spawn Projectile
	CheckNull(Datas[0].ProjectileClass);

	FVector handSocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation("hand_r");
	//OwnerCharacter->GetController()->GetPlayerViewPoint()
	
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(handSocketLocation);

	transform.SetRotation(FQuat(OwnerCharacter->GetControlRotation()));		// == OwnerCharacter->GetActorForwardVector()->Rotation()

	ACMagicBall* magicBall = GetWorld()->SpawnActorDeferred<ACMagicBall>
		(
			Datas[0].ProjectileClass,
			transform,
			OwnerCharacter,
			OwnerCharacter,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	// Todo. 뭔가 넣을 예정

	magicBall->FinishSpawning(transform);
}

void ACDoAction_MagicBall::End_DoAction()
{
	StateComp->SetIdleMode();
	StatusComp->SetMove();
}

void ACDoAction_MagicBall::OnAim()
{
	CheckNull(Aim);
	Aim->On();
}

void ACDoAction_MagicBall::OffAim()
{
	CheckNull(Aim);
	Aim->Off();
}
