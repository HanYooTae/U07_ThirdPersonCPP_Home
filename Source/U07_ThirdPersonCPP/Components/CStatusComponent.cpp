#include "CStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"

UCStatusComponent::UCStatusComponent()
{

}


void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
}

void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}

void UCStatusComponent::ChangeMoveSpeed(EWalkSpeedType InType)
{
	CheckNull(GetOwner());

	UCharacterMovementComponent* movementComp = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());
	CheckNull(movementComp);
	movementComp->MaxWalkSpeed = WalkSpeed[(int8)InType];
}

void UCStatusComponent::DecreaseHealth(float InAmount)
{
	CurrentHealth -= InAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
}

void UCStatusComponent::IncreaseHealth(float InAmount)
{
	CurrentHealth += InAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
}
