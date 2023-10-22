#include "CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance;
	FRotator leftRotation;
	Trace(LeftFootSocket, leftDistance, leftRotation);

	float rightDistance;
	FRotator rightRotation;
	Trace(RightFootSocket, rightDistance, rightRotation);

	// 둘 중 더 작은 값을 IK적용 (닿았으면 0, 떠있으면 -)
	float offset = FMath::Min(leftDistance, rightDistance);

	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);

	// 캡슐은 그대로 두고, 공중에 떠있는 발을 아래로 내림
	Data.LeftDistance.Y = UKismetMathLibrary::FInterpTo(Data.LeftDistance.Y, (leftDistance - offset), DeltaTime, InterpSpeed);
	Data.RightDistance.Y = UKismetMathLibrary::FInterpTo(Data.RightDistance.Y, (rightDistance - offset), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);
}

void UCFeetComponent::Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation)
{
	// 파라미터를 사용하는 지역변수들을 초기화
	OutDistance = 0.f;
	OutRotation = FRotator::ZeroRotator;

	FVector socketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);
	FVector start = FVector(socketLocation.X, socketLocation.Y, OwnerCharacter->GetActorLocation().Z);

	FVector end = FVector(start.X, start.Y, start.Z - CapsuleHalfHeight - AddLength);

	// LineTrace가 OwnerCharacter와 충돌되면 결과 값이 이상하게 변하기 때문에 제외
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	FHitResult hitResult;

	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		true,
		ignoreActors,
		DrawDebugType,
		hitResult,
		false,		// Actor Component이기 때문에 line하고 Component하고 충돌될 경우의 수가 전혀 없음
		FLinearColor::Green,
		FLinearColor::Red
	);

	// line에 충돌된다면 아래 코드 실행
	CheckFalse(hitResult.IsValidBlockingHit());

	// 라인이 땅 속으로 파고든 깊이 (빨간색 라인)
	float undergroundLength = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();

	// 발이 공중에 뜬 길이(공중에 떠있으면 음수 값이 나옴), 지면에 닿아있으면 0
	OutDistance = AdjustHeight + undergroundLength - AddLength;

	// Draw Debug Impact Normal
	FVector impactNormal = hitResult.ImpactNormal;

	// impactNormal의 회전 값을 가져오는데, pitch와 yaw만 가져올 수 있음
	//OutRotation = impactNormal.Rotation();

	float roll = UKismetMathLibrary::DegAtan2(impactNormal.Y, impactNormal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(impactNormal.X, impactNormal.Z);
	
	OutRotation = FRotator(pitch, 0.f, roll);

	pitch = FMath::Clamp(pitch, -45.f, 45.f);
	roll = FMath::Clamp(roll, -45.f, 45.f);


	DrawDebugDirectionalArrow
	(
		GetWorld(),
		hitResult.ImpactPoint,
		hitResult.ImpactPoint + impactNormal * 100.f,
		3.f,
		FColor::Orange,
		false,
		-1.f,
		0,
		3.f
	);
}

