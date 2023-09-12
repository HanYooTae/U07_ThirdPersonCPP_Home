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

	ActionComp = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	ActionComp->OnActionTypeChanged.AddDynamic(this, &ACDoAction_MagicBall::AbortByTypeChanged);
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

	// 카메라의 위치, 회전
	FVector location;
	FRotator rotation;
	OwnerCharacter->GetController()->GetPlayerViewPoint(location, rotation);

	FVector handSocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation("hand_r");
	FVector cameraDirection = rotation.Vector();

	location += cameraDirection * ((handSocketLocation - location) | cameraDirection);
	
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);

	transform.SetRotation(FQuat(OwnerCharacter->GetControlRotation()));		// != OwnerCharacter->GetActorForwardVector()->Rotation()

	ACMagicBall* magicBall = GetWorld()->SpawnActorDeferred<ACMagicBall>
		(
			Datas[0].ProjectileClass,
			transform,
			OwnerCharacter,
			OwnerCharacter,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	// 충돌처리
	magicBall->OnBeginOverlap.AddDynamic(this, &ACDoAction_MagicBall::OnMagicBallBeginOverlap);

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

void ACDoAction_MagicBall::OnMagicBallBeginOverlap(FHitResult hitResult)
{
	FDamageEvent damageEvent;
	hitResult.GetActor()->TakeDamage
	(
		Datas[0].Power,
		damageEvent,
		OwnerCharacter->GetController(),
		this
	);
}

void ACDoAction_MagicBall::AbortByTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	CheckFalse(Aim->IsAvailable());
	CheckFalse(Aim->IsZooming())
	Aim->Off();
}