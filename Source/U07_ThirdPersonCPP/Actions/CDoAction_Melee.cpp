#include "CDoAction_Melee.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Global.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();

	CheckFalse(Datas.Num() > 0);
	CheckFalse(StateComp->IsIdleMode());

	StateComp->SetActionMode();
	
	// ù ��° �׼�(����) ��Ÿ�� ���
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	// ��Ƽ���� ���(�޺� : �ؽ�Ʈ �޺�, ������ : ���� ������Ÿ��..)
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	// ��Ƽ���̿���(���̵� ����, ĵ����)
	StateComp->SetIdleMode();
	StatusComp->SetMove();
}

void ACDoAction_Melee::OnBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnBeginOverlap(InAttacker, InCauser, InOtherCharacter);

	FDamageEvent damageEvent;
	InOtherCharacter->TakeDamage(Datas[ComboCount].Power, damageEvent, InAttacker->GetController(), InCauser);
}

void ACDoAction_Melee::OnEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnEndOverlap(InAttacker, InCauser, InOtherCharacter);
}
