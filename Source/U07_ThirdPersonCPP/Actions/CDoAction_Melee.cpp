#include "CDoAction_Melee.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Global.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();

	// �׼�(����) ��Ÿ�� ���
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
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
}
