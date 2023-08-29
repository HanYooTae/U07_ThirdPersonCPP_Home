#include "CAnimNotifyState_Equip.h"
#include "GameFramework/Character.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CEquipment.h"
#include "Global.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	// 현재 장착 중인 ActionComp->DA[Type]->EquipmentObj->Begin_Equip();
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionData* currentData = actionComp->GetCurrentData();
	CheckNull(currentData);

	ACEquipment* equipment = currentData->GetEquipment();
	CheckNull(equipment);

	equipment->Begin_Equip();
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	// 현재 장착 중인 ActionComp->DA[Type]->EquipmentObj->Begin_Equip();
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionData* currentData = actionComp->GetCurrentData();
	CheckNull(currentData);

	ACEquipment* equipment = currentData->GetEquipment();
	CheckNull(equipment);

	equipment->End_Equip();
}