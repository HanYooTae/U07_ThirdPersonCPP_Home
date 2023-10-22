#include "CMagicBall.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "particles/ParticleSystemComponent.h"

ACMagicBall::ACMagicBall()
{
	CHelpers::CreateSceneComponent(this, &Sphere, "Sphere");
	CHelpers::CreateSceneComponent(this, &Particle, "Particle", Sphere);

	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");

	InitialLifeSpan = 3.f;

	Projectile->InitialSpeed = 2000.f;
	Projectile->MaxSpeed = 4000.f;
	Projectile->ProjectileGravityScale = 0.f;
}

void ACMagicBall::BeginPlay()
{
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACMagicBall::OnComponentBeginOverlap);

	Super::BeginPlay();
}

void ACMagicBall::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());

	// Play Impact Particles
	if (!!ImpactParticle)
	{
		FTransform transform = ImpactTransform;
		transform.AddToTranslation(GetActorLocation());

		// 비스듬한 벽에 맞을 경우, 파티클을 회전시킴
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, transform);
	}

	// DoAction_MagicBall->TakeDamage
	if (OnBeginOverlap.IsBound())
		OnBeginOverlap.Broadcast(SweepResult);

	Destroy();
}
