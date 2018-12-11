// Copyright Robert Hinds 2018

#include "PlatformMover.h"
#include "Gameframework/Actor.h"
#include "Components/PrimitiveComponent.h"


float UPlatformMover::GetTotalMassOFActorsOnTriggerPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	//find all overlapping actors
	if (!PressurePlate)
	{
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* Actor : OverlappingActors)
	{

		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("actor name: %s"), *Actor->GetName())
	}
	//Iterate through the actors adding their mass
	return TotalMass;
}

// Sets default values for this component's properties
UPlatformMover::UPlatformMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlatformMover::BeginPlay()
{
	Super::BeginPlay();
	InitialLocationVector = GetOwner()->GetActorLocation();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find PressurePlate triggger volume of %s"), *(GetOwner()->GetName()))
	}

	// ...
	
}


// Called every frame
void UPlatformMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ((GetTotalMassOFActorsOnTriggerPlate() >= TriggerPlateTotalMassThreshold))
	{
		if (GetOwner()->GetActorLocation().Z < (InitialLocationVector.Z + MaxPlatformHeight.Z))
		{
			OnUp.Broadcast();
		}
	}
	else if ((GetTotalMassOFActorsOnTriggerPlate() < TriggerPlateTotalMassThreshold) && GetOwner()->GetActorLocation() != InitialLocationVector)
	{
		if (GetOwner()->GetActorLocation().Z > InitialLocationVector.Z)
		{
			OnDown.Broadcast();
		}
	}
	else
	{

	}
/*	else if ((GetTotalMassOFActorsOnTriggerPlate() < TriggerPlateTotalMassThreshold))
	{
		OnDown.Broadcast();

	}
	*/


	// ...
}


