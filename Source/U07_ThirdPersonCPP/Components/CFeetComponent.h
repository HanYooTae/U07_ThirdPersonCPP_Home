#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public:
	// �޹��� �󸶸�ŭ �������ִ���
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftDistance;

	// �������� �󸶸�ŭ �������ִ���
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector PelvisDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RightRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U07_THIRDPERSONCPP_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// ����Ʈ �е������� ����ü�� ũ�Ⱑ Ŭ �� �ۿ� ��� ���۷����� ����
	FORCEINLINE const FFeetData& GetData() { return Data; }

private:
	void Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation);	// ���߿� ���ִ� ������ ���Ϲ޴� �Լ�

protected:
	UPROPERTY(EditAnywhere, Category = "IK")
		FName LeftFootSocket = "foot_l";

	UPROPERTY(EditAnywhere, Category = "IK")
		FName RightFootSocket = "foot_r";

	UPROPERTY(EditAnywhere, Category = "IK")
		float AddLength = 55.f;

	UPROPERTY(EditAnywhere, Category = "IK")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

	UPROPERTY(EditAnywhere, Category = "IK")
		float AdjustHeight = 5.f;

	UPROPERTY(EditAnywhere, Category = "IK")
		float InterpSpeed = 5.f;

private:
	FFeetData Data;

	class ACharacter* OwnerCharacter;

	float CapsuleHalfHeight;
};
