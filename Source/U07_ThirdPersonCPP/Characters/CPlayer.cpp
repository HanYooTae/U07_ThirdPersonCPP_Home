#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CFeetComponent.h"
#include "Actions/CActionData.h"
#include "Widgets/CPlayerHealthWidget.h"
#include "Widgets/CSelectActionWidget_Group.h"
#include "Widgets/CSelectActionWidget_Icon.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Scene Component
	CHelpers::CreateSceneComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateSceneComponent(this, &Camera, "Camera", SpringArm);

	// Create Actor Component
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Option, "Option");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Feet, "Feet");

	// Component Settings
	// -> SkeletalMesh
	USkeletalMesh* meshAsset;
	CHelpers::GetAsset(&meshAsset, "SkeletalMesh'/Game/Character/Heraklios/Mesh/Heraklios_By_A__Dizon.Heraklios_By_A__Dizon'");
	GetMesh()->SetSkeletalMesh(meshAsset);
	
	TSubclassOf<UAnimInstance> animClass;
	CHelpers::GetClass(&animClass, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animClass);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	// -> Spring Arm
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	 
	// -> Movement
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Get Widget Class Asset
	CHelpers::GetClass<UCPlayerHealthWidget>(&HealthWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_PlayerHealth.WB_PlayerHealth_C'");
	CHelpers::GetClass<UCSelectActionWidget_Group>(&SelectActionWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_SelectAction_Group.WB_SelectAction_Group_C'");
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Create & Set Dynamic Material
	BodyMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), nullptr);
	GetMesh()->SetMaterial(0, BodyMaterial);

	// State Type Changed Event
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	Action->SetUnarmedMode();

	// Create Widgets
	HealthWidget = Cast<UCPlayerHealthWidget>(CreateWidget(GetController<APlayerController>(), HealthWidgetClass));
	CheckNull(HealthWidget);
	HealthWidget->AddToViewport();

	SelectActionWidget = Cast<UCSelectActionWidget_Group>(CreateWidget(GetController<APlayerController>(), SelectActionWidgetClass));
	CheckNull(SelectActionWidget);
	SelectActionWidget->AddToViewport();
	SelectActionWidget->SetVisibility(ESlateVisibility::Hidden);

	// Bind SelectAction Widget Event
	SelectActionWidget->GetChildWidget("Icon1")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnFist);
	SelectActionWidget->GetChildWidget("Icon2")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnOneHand);
	SelectActionWidget->GetChildWidget("Icon3")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnTwoHand);
	SelectActionWidget->GetChildWidget("Icon4")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnMagicBall);
	SelectActionWidget->GetChildWidget("Icon5")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnWarp);
	SelectActionWidget->GetChildWidget("Icon6")->OnImageButtonPressed.AddDynamic(this, &ACPlayer::OnStorm);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis Event
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	// Action Event
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, this, &ACPlayer::OffWalk);
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction("MagicBall", EInputEvent::IE_Pressed, this, &ACPlayer::OnMagicBall);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarp);
	PlayerInputComponent->BindAction("Storm", EInputEvent::IE_Pressed, this, &ACPlayer::OnStorm);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnAction);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);

	PlayerInputComponent->BindAction("SelectAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSelectAction);
	PlayerInputComponent->BindAction("SelectAction", EInputEvent::IE_Released, this, &ACPlayer::OffSelectAction);
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	Causer = DamageCauser;

	Action->AbortByDamaged();
	Status->DecreaseHealth(DamageValue);
	HealthWidget->UpdateHealth();

	if (Status->IsDead())
	{
		State->SetDeadMode();
		return DamageValue;
	}

	State->SetHittedMode();

	return DamageValue;
}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckTrue(FMath::IsNearlyZero(InAxis));

	CheckFalse(Status->IsCanMove());

	FRotator controlRotationYaw = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(controlRotationYaw).GetForwardVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckTrue(FMath::IsNearlyZero(InAxis));

	CheckFalse(Status->IsCanMove());

	FRotator controlRotationYaw = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(controlRotationYaw).GetRightVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float speed = Option->GetHorizontalSpeed();
	AddControllerYawInput(speed * InAxis * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float speed = Option->GetVerticalSpeed();
	AddControllerPitchInput(speed * InAxis * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnZoom(float InAxis)
{
	float rate = Option->GetZoomSpeed() * InAxis * GetWorld()->GetDeltaSeconds();
	SpringArm->TargetArmLength += rate;
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, Option->GetZoomMin(), Option->GetZoomMax());
}

void ACPlayer::OnWalk()
{
	//GetCharacterMovement()->MaxWalkSpeed = Status->GetWalkSpeed();
	Status->ChangeMoveSpeed(EWalkSpeedType::Walk);
}

void ACPlayer::OffWalk()
{
	//GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	Status->ChangeMoveSpeed(EWalkSpeedType::Run);
}

void ACPlayer::OnEvade()
{
	CheckFalse(State->IsIdleMode());

	if (InputComponent->GetAxisValue("MoveForward") < 0)
	{
		State->SetBackStepMode();
		return;
	}

	State->SetRollMode();
}

void ACPlayer::OnFist()
{
	CheckFalse(State->IsIdleMode());

	Action->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	CheckFalse(State->IsIdleMode());

	Action->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	CheckFalse(State->IsIdleMode());

	Action->SetTwoHandMode();
}

void ACPlayer::OnMagicBall()
{
	CheckFalse(State->IsIdleMode());

	Action->SetMagicBallMode();
}

void ACPlayer::OnWarp()
{
	CheckFalse(State->IsIdleMode());

	Action->SetWarpMode();
}

void ACPlayer::OnStorm()
{
	CheckFalse(State->IsIdleMode());

	CLog::Print("Storm Mode!");
	Action->SetStormMode();
}

void ACPlayer::OnAction()
{
	Action->DoAction();
}

void ACPlayer::OnAim()
{
	Action->DoAim(true);
}

void ACPlayer::OffAim()
{
	Action->DoAim(false);
}

void ACPlayer::OnSelectAction()
{
	CheckFalse(State->IsIdleMode());

	SelectActionWidget->SetVisibility(ESlateVisibility::Visible);
	GetController<APlayerController>()->bShowMouseCursor = true;
	GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
}

void ACPlayer::OffSelectAction()
{
	SelectActionWidget->SetVisibility(ESlateVisibility::Hidden);
	GetController<APlayerController>()->bShowMouseCursor = false;
	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector target = start + GetVelocity().GetSafeNormal2D();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

	Montages->PlayRoll();
}

void ACPlayer::Begin_BackStep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Montages->PlayBackStep();
}

void ACPlayer::Hitted()
{
	// Play Hitted Montage
	Montages->PlayHitted();
}

void ACPlayer::Dead()
{
	// 이미 죽어있다면 return
	CheckFalse(State->IsDeadMode());

	// Disable Input
	//GetController<APlayerController>();
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	CheckNull(controller);

	DisableInput(controller);

	// Play Dead Montage
	Montages->PlayDead();

	// Off All Collisions, 충돌체 Spectator로 변경
	Action->OffAllCollisions();
	GetCapsuleComponent()->SetCollisionProfileName("Spectator");

	// Destroy All(Attachment, Equipment, DoAction...)
	UKismetSystemLibrary::K2_SetTimer(this, "End_Dead", 5.f, false);
}

void ACPlayer::End_Dead()
{
	Action->End_Dead();

	// Todo. 플레이어가 죽은 후에 동작할 event
	CLog::Log("You Died");
	CLog::Print("You Died");
}

void ACPlayer::End_Roll()
{
	CheckNull(Action->GetCurrentDataAsset());

	if (Action->GetCurrentDataAsset()->EquipmentData.bPawnControl == true)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	State->SetIdleMode();
}

void ACPlayer::End_BackStep()
{
	CheckNull(Action->GetCurrentDataAsset());

	if (Action->GetCurrentDataAsset()->EquipmentData.bPawnControl == false)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	
	State->SetIdleMode();
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Roll:		Begin_Roll();		break;
	case EStateType::BackStep:  Begin_BackStep();	break;
	case EStateType::Hitted:	Hitted();			break;
	case EStateType::Dead:		Dead();				break;
	}
}

void ACPlayer::ChangeBodyColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(PlayerTeamID);
}
