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
	// 왼발이 얼마만큼 떨어져있는지
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftDistance;

	// 오른발이 얼마만큼 떨어져있는지
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
	// 바이트 패딩때문에 구조체의 크기가 클 수 밖에 없어서 레퍼런스로 받음
	FORCEINLINE const FFeetData& GetData() { return Data; }

private:
	void Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation);	// 공중에 떠있는 정도를 리턴받는 함수

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
