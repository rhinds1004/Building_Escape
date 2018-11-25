// Copyright Robert Hinds 2018


#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"


#define OUT //just to give context that a parameter is out

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	
	//UE_LOG(LogTemp, Warning, TEXT("%s at location %s!"), *temp->GetName());
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	
	FString ObjectName = GetOwner()->GetName();
	FString ObjectPos = GetOwner()->GetTransform().GetLocation().ToCompactString();

	UE_LOG(LogTemp, Warning, TEXT("%s at location %s!"), *ObjectName, *ObjectPos);
	
	/// Look for attached Physics Handle
	/// findComponentByClass is a generic. The diamond operators specify which component to look for.
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
	///Physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find PhysicsHandle of %s"), *(GetOwner()->GetName()) )
	}

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get Player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	//getPlayerViewPoint takes references and modifies their contents
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.0f
		);
	//Logs out to test
	/*
	UE_LOG(LogTemp, Warning, TEXT("Position: %s with Rotation: %s!"), *(PlayerViewPointLocation.ToCompactString()),
		*(PlayerViewPointRotation.ToString()))
*/
		FHitResult Hit;

		///Setup quary parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

		///Line-trace (AKA ray-cast) out to reach distance
	///A ray shots out from beginning point (PlayerViewPointLocation) to the End point (LineTraceEnd). If any
	///objects of specified type (ECC_PhysicsBody in thsi case) then a hit is send on the specified channel (ECollisionChannel in this case).
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	
		///See what we hit
	AActor *ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit %s"), *(ActorHit->GetName()))
	}
}

