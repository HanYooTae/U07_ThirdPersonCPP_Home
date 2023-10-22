#include "CDoAction_Storm.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/BoxComponent.h"
#include "CAttachment.h"

void ACDoAction_Storm::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* child : OwnerCharacter->Children)
	{
		// IsA() : 상속받았으면 true, 아니면 false
		if (child->IsA<ACAttachment>() && child->GetActorLabel().Contains("Storm"))
		{
			Box = CHelpers::GetComponent<UBoxComponent>(child);
			break;
		}
	}
}

void ACDoAction_Storm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentYaw += ArroundSpeed * DeltaTime;

	if (FMath::IsNearlyEqual(CurrentYaw, 360.f))
		CurrentYaw = 0.f;

	// Damage를 줄 때
	CheckFalse(bActivating);

	FVector location = OwnerCharacter->GetActorLocation();
	FVector awayFrom = FVector(Radius, 0, 0).RotateAngleAxis(CurrentYaw, OwnerCharacter->GetActorUpVector());

	location += awayFrom;
	Box->SetWorldLocation(location);
}

void ACDoAction_Storm::DoAction()
{
	Super::DoAction();

	CheckFalse(StateComp->IsIdleMode());

	// Action 중일 때 다시 DoAction을 사용할 수 없게 함
	CheckTrue(bActivating);
	bActivating = true;

	StateComp->SetActionMode();
	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
}

void ACDoAction_Storm::Begin_DoAction()
{
	Super::Begin_DoAction();

	// 항상 OwnerCharacter의 forward vector의 yaw값으로 시작
	CurrentYaw = OwnerCharacter->GetActorForwardVector().Rotation().Yaw;

	// Spawn Particle
	if (!!Datas[0].Effect)
	{
		Particle = UGameplayStatics::SpawnEmitterAttached(Datas[0].Effect, Box, NAME_None);
		Particle->SetRelativeLocation(Datas[0].EffectTransform.GetLocation());
		Particle->SetRelativeScale3D(Datas[0].EffectTransform.GetScale3D());

		// On Collision
		Attachment = Cast<ACAttachment>(Box->GetOwner());
		if (!!Attachment)
			Attachment->OnCollisions();

		// DamagePerTime(0.1s) Timer
		UKismetSystemLibrary::K2_SetTimer(this, "TickDamage", 0.1f, true);
	}
}

void ACDoAction_Storm::End_DoAction()
{
	Super::End_DoAction();

	StatusComp->SetMove();
	StateComp->SetIdleMode();

	FTimerDynamicDelegate onEndActiveTime;
	onEndActiveTime.BindUFunction(this, "Finish");
	UKismetSystemLibrary::K2_SetTimerDelegate(onEndActiveTime, ActiveTime, false);
}

void ACDoAction_Storm::Abort()
{
	Super::Abort();
	
	Finish();
}

void ACDoAction_Storm::OnBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	// 배열에 등록한 후, 등록된 애들한테만 데미지는 준다.
	HittedCharacters.AddUnique(InOtherCharacter);
}

void ACDoAction_Storm::OnEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	// EndOverlap이 되면 배열에서 삭제
	HittedCharacters.Remove(InOtherCharacter);
}

void ACDoAction_Storm::TickDamage()
{
	FDamageEvent damageEvent;

	for (int32 i = 0; i < HittedCharacters.Num(); i++)
	{
		UCStateComponent* stateComp = CHelpers::GetComponent<UCStateComponent>(HittedCharacters[i]);

		bool bIgnore = false;
		bIgnore |= (stateComp->IsDeadMode());
		bIgnore |= (HittedCharacters[i] == nullptr);
		bIgnore |= (HittedCharacters[i]->IsPendingKill());

		// bIgnore가 true라면 이후 코드를 전부 건너뜀.
		if (bIgnore) continue;

		HittedCharacters[i]->TakeDamage
		(
			Datas[0].Power,
			damageEvent,
			OwnerCharacter->GetController(),
			this
		);
	}
}

void ACDoAction_Storm::Finish()
{
	// Destroy Particle
	if (!!Particle)
		Particle->DestroyComponent();

	//  Off Collision
	if (!!Attachment)
		Attachment->OffCollisions();

	// Clear TickDamage()
	// TickDamage 함수를 찾아주는 과정이 포함되어있기 때문에 조금 느리다.
	// Handler가 포함되어있는 함수를 사용하면 좀더 빠르다.
	UKismetSystemLibrary::K2_ClearTimer(this, "TickDamage");

	// Reset bActivating;
	bActivating = false;
}