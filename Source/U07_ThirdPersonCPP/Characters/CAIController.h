#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class U07_THIRDPERSONCPP_API ACAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void Tick(float DeltaTime) override;	// draw debug 표시 (' -> 4 안눌러도 되게)

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

private:
	class ACEnemy_AI* PossessedEnemy;
	class UAISenseConfig_Sight* Sight;
};
