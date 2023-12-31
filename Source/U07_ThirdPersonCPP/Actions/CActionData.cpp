		//Equipment->FinishSpawning()
#include "CActionData.h"
#include "CEquipment.h"
#include "CAttachment.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "Actions/CDoAction.h"

void UCActionData::BeginPlay(class ACharacter* InOwnerCharacter, UCActionData_Spawned** OutSpawned)
{
	FTransform transform;

	ACAttachment* Attachment = nullptr;
	if (!!AttachmentClass)
	{
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);

		Attachment->SetActorLabel(GetCustomActorLabel(InOwnerCharacter, "Attachment"));

		UGameplayStatics::FinishSpawningActor(Attachment, transform);

	}

	ACEquipment* Equipment = nullptr;
	if (!!EquipmentClass)
	{
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);

		Equipment->SetData(EquipmentData);
		Equipment->SetColor(EquipmentColor);
		Equipment->SetActorLabel(GetCustomActorLabel(InOwnerCharacter, "Equipment"));

		//Equipment->FinishSpawning()
		UGameplayStatics::FinishSpawningActor(Equipment, transform);

		if (!!Attachment)
		{
			Equipment->OnBeginEquip.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equipment->OnUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}

	ACDoAction* DoAction = nullptr;
	if (!!DoActionClass)
	{
		DoAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, transform, InOwnerCharacter);

		DoAction->SetDatas(DoActionDatas);
		DoAction->SetActorLabel(GetCustomActorLabel(InOwnerCharacter, "DoAction"));

		UGameplayStatics::FinishSpawningActor(DoAction, transform);

		// Equipment에 있는 주소가 DoAction으로 참조됨 (같은 주소를 사용함)
		if (!!Equipment)
		{
			DoAction->SetEquippedThis(Equipment->IsEquippedThis());
		}

		if (!!Attachment)
		{
			Attachment->OnBeginOverlap.AddDynamic(DoAction, &ACDoAction::OnBeginOverlap);
			Attachment->OnEndOverlap.AddDynamic(DoAction, &ACDoAction::OnEndOverlap);
		}
	}

	(*OutSpawned) = NewObject<UCActionData_Spawned>();
	(*OutSpawned)->Attachment = Attachment;
	(*OutSpawned)->Equipment = Equipment;
	(*OutSpawned)->DoAction = DoAction;
	(*OutSpawned)->EquipmentColor = EquipmentColor;

}

FString UCActionData::GetCustomActorLabel(ACharacter* InOwnerCharacter, FString InMiddleName)
{
	FString name;
	name.Append(InOwnerCharacter->GetActorLabel());
	name.Append("_");
	name.Append(InMiddleName);
	name.Append("_");

	name.Append(GetName().Replace(L"DA_", L""));

	return name;
}