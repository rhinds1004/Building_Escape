// Copyright Robert Hinds 2018

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Gameframework/Actor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


float UOpenDoor::GetTotalMassOFActorsOnTriggerPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	//find all overlapping actors
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for(const auto* Actor : OverlappingActors)
	{
		
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("actor name: %s"), *Actor->GetName())
	}
	//Iterate through the actors adding their mass
	return TotalMass;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOFActorsOnTriggerPlate() >= TriggerPlateTotalMassThreshold)
	{
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
		OpenDoor();
		
		
	}
	else 
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpenTime > DoorCloseDelay) {
			CloseDoor();
		}
	}
	
	// ...
}

void UOpenDoor::OpenDoor() {
	
	Owner->SetActorRotation( FRotator(0.0f, OpenAngle, 0.0f));
	
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}