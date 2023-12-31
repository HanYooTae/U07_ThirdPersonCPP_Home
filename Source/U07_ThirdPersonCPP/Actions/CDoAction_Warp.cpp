#include "CDoAction_Warp.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Global.h"
#include "Actions/CAttachment.h"
#include "AIController.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();		// OwnerCharacter, StateComp, StatusComp 저장되어있음
	
	//CHelpers::GetClass(&AttachmentClass, "Blueprint'/Game/Actions/BP_CAttachment_Warp.BP_CAttachment_Warp_C'");
	/*
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	actionComp->GetCurrentData()->GetAttachment();*/

	for (AActor* child : OwnerCharacter->Children)
	{
		// IsA() : 상속받았으면 true, 아니면 false
		if (child->IsA<ACAttachment>() && child->GetActorLabel().Contains("Warp"))
		{
			WarpPoint = CHelpers::GetComponent<UStaticMeshComponent>(child);
			break;
		}
	}
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(IsPlayerControlled());
	// Warp가 장착이 되어있을 때만 동작
	CheckFalse(*bEquippedThis);

	FVector location;
	FRotator rotation;

	if (GetCursorLocationAndRotation(location, rotation))
	{
		// 마우스 커서가 닿을 수 있는 지점이라면
		WarpPoint->SetVisibility(true);
		WarpPoint->SetWorldLocationAndRotation(location + WarpPoint->GetUpVector() * 120.f, FQuat(rotation));
	}
	else
	{
		// 마우스 커서가 닿을 수 없는 지점이라면
		WarpPoint->SetVisibility(false);
	}
}

void ACDoAction_Warp::DoAction()
{
	Super::DoAction();

	CheckFalse(StateComp->IsIdleMode());

	// PlayerController
	if (IsPlayerControlled())
	{
		FRotator temp;

		// 마우스 커서가 닿을 수 없는 지점이라면 false 리턴, 아니라면 true
		CheckFalse(GetCursorLocationAndRotation(WarpLocation, temp));
	}

	// AIController
	else
	{
		AAIController* controller = OwnerCharacter->GetController<AAIController>();
		if (!!controller)
		{
			UCBehaviorComponent* behaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(controller);
			if (!!behaviorComp)
			{
				WarpLocation = behaviorComp->GetLocationKey();
			}
		}
	}

	// 캡슐 충돌체 절반 높이만큼 올려줘야 함(땅 속에 끼일 수가 있음)
	WarpLocation.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	StateComp->SetActionMode();
	Datas[0].bCanMove ? StatusComp->SetMove() : StatusComp->SetStop();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
}

void ACDoAction_Warp::Begin_DoAction()
{
	Super::Begin_DoAction();


	FTransform transform = Datas[0].EffectTransform;

	UGameplayStatics::SpawnEmitterAttached
	(
		Datas[0].Effect,
		OwnerCharacter->GetMesh(),
		"Root",
		transform.GetLocation(),
		FRotator(transform.GetRotation()),
		transform.GetScale3D()
	);
}

void ACDoAction_Warp::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->SetActorLocation(WarpLocation);

	StateComp->SetIdleMode();
	StatusComp->SetMove();
}

bool ACDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNullResult(controller, false);

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	FHitResult hitResult;

	if (controller->GetHitResultUnderCursorForObjects(objectTypes, true, hitResult))
	{
		OutLocation = hitResult.Location;
		//OutRotator = hitResult.ImpactNormal.Rotation();

		FVector normal = hitResult.ImpactNormal;
		float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);
		float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);

		OutRotator = FRotator(pitch, 0, roll);

		return true;
	}

	return false;
}

bool ACDoAction_Warp::IsPlayerControlled()
{

	return (OwnerCharacter->GetController()->GetClass() == APlayerController::StaticClass());
}
