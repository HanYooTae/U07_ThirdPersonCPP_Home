#include "CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Global.h"

ACDoAction::ACDoAction()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACDoAction::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	StateComp = CHelpers::GetComponent<StateComponent>(OwnerCharacter);
	StatusComp = CHelpers::GetComponent<StatusComponent>(OwnerCharacter);

	Super::BeginPlay();
	
}

void ACDoAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

