// Copyright Robert Hinds 2018

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Gameframework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
	
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
	}
	else 
	{
		CloseDoor();
	}
	
	// ...
}

void UOpenDoor::OpenDoor() {
	AActor *Owner = GetOwner();
	FRotator OwnerRotation = Owner->GetActorRotation();
	FQuat OwnerQuat = OwnerRotation.Quaternion();
	//FVector *RotVector = new FVector(0.0f, 0.0f, 90.0f);
	Owner->SetActorRotation(*(new FRotator(0.0f, 90.0f, 0.0f)));
	
}

void UOpenDoor::CloseDoor()
{
	AActor *Owner = GetOwner();
	FRotator OwnerRotation = Owner->GetActorRotation();
	FQuat OwnerQuat = OwnerRotation.Quaternion();
	//FVector *RotVector = new FVector(0.0f, 0.0f, 90.0f);
	Owner->SetActorRotation(*(new FRotator(0.0f, 0.0f, 0.0f)));
}