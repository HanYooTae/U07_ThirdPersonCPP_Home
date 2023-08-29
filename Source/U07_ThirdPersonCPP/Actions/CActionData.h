#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CActionData.generated.h"

USTRUCT(BlueprintType)

//-----------------------------------------------------------------------------
// Struct = EquipmentData
//-----------------------------------------------------------------------------
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1.f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	// 캐릭터 회전못하게
	// bUseControllerRotationYaw = true;
	// GetCharacterMovement()->bOrientRotationToMovement = false;
	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;
};

//-----------------------------------------------------------------------------
// Class = DataAsset
//-----------------------------------------------------------------------------
UCLASS()
class U07_THIRDPERSONCPP_API UCActionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	void BeginPlay(class ACharacter* InOwnerCharacter);

public:
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }

private:
	FString GetCustomActorLabel(class ACharacter* InOwnerCharacter, FString InMiddleName);

public:
	UPROPERTY(EditAnywhere, Category = "Equipment")
		TSubclassOf<class ACEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere, Category = "Equipment")
		FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere, Category = "Equipment")
		FLinearColor EquipmentColor;

private:
	class ACEquipment* Equipment;
};
