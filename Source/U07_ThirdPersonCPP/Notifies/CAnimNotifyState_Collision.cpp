#include "CAnimNotifyState_Collision.h"
#include "GameFramework/Character.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Global.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	// 현재 장착 중인 ActionComp->DA[Type]->EquipmentObj->Begin_Equip();
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionData* currentData = actionComp->GetCurrentData();
	CheckNull(currentData);

	ACAttachment* attachment = currentData->GetAttachment();
	CheckNull(attachment);

	attachment->OnCollisions();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	// 현재 장착 중인 ActionComp->DA[Type]->EquipmentObj->Begin_Equip();
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionData* currentData = actionComp->GetCurrentData();
	CheckNull(currentData);

	ACAttachment* attachment = currentData->GetAttachment();
	CheckNull(attachment);

	attachment->OffCollisions();
}