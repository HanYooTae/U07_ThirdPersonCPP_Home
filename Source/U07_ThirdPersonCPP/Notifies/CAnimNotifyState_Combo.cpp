#include "CAnimNotifyState_Combo.h"
#include "GameFramework/Character.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction_Melee.h"
#include "Global.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	// 현재 장착 중인 ActionComp->DA[Type]->EquipmentObj->Begin_Equip();
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionData_Spawned* currentData = actionComp->GetCurrentData();
	CheckNull(currentData);

	ACDoAction_Melee* doAction_melee = Cast<ACDoAction_Melee>(currentData->GetDoAction());
	CheckNull(doAction_melee);
	doAction_melee->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	// 현재 장착 중인 ActionComp->DA[Type]->EquipmentObj->Begin_Equip();
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionData_Spawned* currentData = actionComp->GetCurrentData();
	CheckNull(currentData);

	ACDoAction_Melee* doAction_melee = Cast<ACDoAction_Melee>(currentData->GetDoAction());
	CheckNull(doAction_melee);
	doAction_melee->DisableCombo();
}