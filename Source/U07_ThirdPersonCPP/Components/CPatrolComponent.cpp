#include "CPatrolComponent.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Objects/CPatrolPath.h"

UCPatrolComponent::UCPatrolComponent()
{
}


void UCPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

bool UCPatrolComponent::GetMoveToLocation(FVector& OutLocation)
{
	OutLocation = FVector::ZeroVector;
	CheckNullResult(Path, false);

	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);

	return true;
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckFalse(IsPathValid());	//== CheckNull(Path);

	int32 count = Path->GetSpline()->GetNumberOfSplinePoints();

	// Reverse
	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;
			return;
		}

		// Reverse & ClosedLoop == true
		if (Path->GetSpline()->IsClosedLoop())
		{
			Index = count - 1;
			return;
		}

		// Reverse & ClosedLoop == false
		Index = 1;
		bReverse = false;
		return;
	}

	// Forward
	if (Index < count - 1)
	{
		Index++;
		return;
	}

	// Forward & ClosedLoop == true
	if (Path->GetSpline()->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	// Forward & ClosedLoop == false
	Index--;
	bReverse = true;
}
