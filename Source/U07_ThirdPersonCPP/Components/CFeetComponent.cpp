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

	// �� �� �� ���� ���� IK���� (������� 0, �������� -)
	float offset = FMath::Min(leftDistance, rightDistance);

	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);

	// ĸ���� �״�� �ΰ�, ���߿� ���ִ� ���� �Ʒ��� ����
	Data.LeftDistance.Y = UKismetMathLibrary::FInterpTo(Data.LeftDistance.Y, (leftDistance - offset), DeltaTime, InterpSpeed);
	Data.RightDistance.Y = UKismetMathLibrary::FInterpTo(Data.RightDistance.Y, (rightDistance - offset), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);
}

void UCFeetComponent::Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation)
{
	// �Ķ���͸� ����ϴ� ������������ �ʱ�ȭ
	OutDistance = 0.f;
	OutRotation = FRotator::ZeroRotator;

	FVector socketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);
	FVector start = FVector(socketLocation.X, socketLocation.Y, OwnerCharacter->GetActorLocation().Z);

	FVector end = FVector(start.X, start.Y, start.Z - CapsuleHalfHeight - AddLength);

	// LineTrace�� OwnerCharacter�� �浹�Ǹ� ��� ���� �̻��ϰ� ���ϱ� ������ ����
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
		false,		// Actor Component�̱� ������ line�ϰ� Component�ϰ� �浹�� ����� ���� ���� ����
		FLinearColor::Green,
		FLinearColor::Red
	);

	// line�� �浹�ȴٸ� �Ʒ� �ڵ� ����
	CheckFalse(hitResult.IsValidBlockingHit());

	// ������ �� ������ �İ�� ���� (������ ����)
	float undergroundLength = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();

	// ���� ���߿� �� ����(���߿� �������� ���� ���� ����), ���鿡 ��������� 0
	OutDistance = AdjustHeight + undergroundLength - AddLength;

	// Draw Debug Impact Normal
	FVector impactNormal = hitResult.ImpactNormal;

	// impactNormal�� ȸ�� ���� �������µ�, pitch�� yaw�� ������ �� ����
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

