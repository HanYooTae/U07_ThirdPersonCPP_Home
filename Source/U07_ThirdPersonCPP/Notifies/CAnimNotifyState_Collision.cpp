#include "CAnimNotifyState_Collision.h"
#include "GameFramework/Character.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction_Melee.h"
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

	// ���� ���� ���� ActionComp->DA[Type]->EquipmentObj->Begin_Equip();
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionData_Spawned* currentData = actionComp->GetCurrentData();
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

	// ���� ���� ���� ActionComp->DA[Type]->EquipmentObj->Begin_Equip();
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	UCActionData_Spawned* currentData = actionComp->GetCurrentData();
	CheckNull(currentData);

	ACAttachment* attachment = currentData->GetAttachment();
	CheckNull(attachment);

	attachment->OffCollisions();

	ACDoAction_Melee* doAction_melee = Cast<ACDoAction_Melee>(currentData->GetDoAction());
	CheckNull(doAction_melee);
	doAction_melee->ClearHittedCharacters();
}