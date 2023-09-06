#include "CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Characters/ICharacter.h"

ACEquipment::ACEquipment()
{
}

void ACEquipment::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	StateComp = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	StatusComp = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
}

void ACEquipment::Equip_Implementation()
{
	// Set State Equip
	StateComp->SetEquipMode();

	// bCanMove
	Data.bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();

	// Play Draw AnimMontage
	if (!!Data.AnimMontage)
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRate, Data.StartSection);
	else
	{
		// 몽타주가 없는 무기 장착 시
		Begin_Equip();

		// 몽타주가 없으니 Notify도 없으므로 바로 SetIdle
		End_Equip();
	}

	// bPawnControl
	if (Data.bPawnControl == true)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	IICharacter* characterInterface = Cast<IICharacter>(OwnerCharacter);
	CheckNull(characterInterface);
	characterInterface->ChangeBodyColor(Color);
}

void ACEquipment::Begin_Equip_Implementation()
{
	if (OnBeginEquip.IsBound())
		OnBeginEquip.Broadcast();
}

void ACEquipment::End_Equip_Implementation()
{
	StateComp->SetIdleMode();
	StatusComp->SetMove();
}

void ACEquipment::Unequip_Implementation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	if (OnUnequip.IsBound())
		OnUnequip.Broadcast();
}