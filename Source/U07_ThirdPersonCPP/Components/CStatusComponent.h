#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U07_THIRDPERSONCPP_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetSneakSpeed() { return SneakSpeed; }
	FORCEINLINE float GetWalkSpeed() { return WalkSpeed; }
	FORCEINLINE float GetRunSpeed() { return RunSpeed; }

	FORCEINLINE bool IsCanMove() { return bCanMove; }
	
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }

	void SetMove();		// CanMove를 true로 만드는 역할
	void SetStop();		// CanMove를 false로 만드는 역할

	void DecreaseHealth(float InAmount);
	void IncreaseHealth(float InAmount);
	FORCEINLINE bool IsDead() { return CurrentHealth <= 0; }

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float SneakSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float WalkSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float RunSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.f;

private:
	bool bCanMove = true;
	float CurrentHealth;
};
