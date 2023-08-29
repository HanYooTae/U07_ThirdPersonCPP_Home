#include "CActionData.h"
#include "CEquipment.h"
#include "GameFramework/Character.h"
#include "Global.h"

void UCActionData::BeginPlay(class ACharacter* InOwnerCharacter)
{
	FTransform transform;
	if (!!EquipmentClass)
	{
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);

		Equipment->SetData(EquipmentData);
		Equipment->SetColor(EquipmentColor);
		Equipment->SetActorLabel(GetCustomActorLabel(InOwnerCharacter, "Equipment"));

		//Equipment->FinishSpawning()
		UGameplayStatics::FinishSpawningActor(Equipment, transform);
	}
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