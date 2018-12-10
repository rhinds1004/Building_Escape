// Copyright Robert Hinds 2018


#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"


#define OUT //just to give context that a parameter is out

void UGrabber::Grab()
{
	///LINE TRACE and see if we reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent(); //gets the mesh in our case
	AActor *ActorHit = HitResult.GetActor();

	///If we hit something then attach a physics handle		
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab,
			NAME_None, //no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
	UE_LOG(LogTemp, Warning, TEXT("Released!"));
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() 
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Line-trace (AKA ray-cast) out to reach distance
	///A ray shots out from beginning point (PlayerViewPointLocation) to the End point (LineTraceEnd). If any
	///objects of specified type (ECC_PhysicsBody in thsi case) then a hit is send on the specified channel (ECollisionChannel in this case).
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	///getPlayerViewPoint takes references and modifies their contents
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	///getPlayerViewPoint takes references and modifies their contents
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

/// Look for attached Physics Handle
/// findComponentByClass is a generic. The diamond operators specify which component to look for.
void UGrabber::FindPhysicsHandleComponent() 
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find PhysicsHandle of %s"), *(GetOwner()->GetName()))
	}
}

/// Look for attached Input Component (only appears at runtime)
/// findComponentByClass is a generic. The diamond operators specify which component to look for.
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find Input Component of %s"), *GetOwner()->GetName())
	}
}

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsBeginPlay = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	FString ObjectName = GetOwner()->GetName();
	FString ObjectPos = GetOwner()->GetTransform().GetLocation().ToCompactString();
	UE_LOG(LogTemp, Warning, TEXT("%s at location %s!"), *ObjectName, *ObjectPos);	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	// if the physics handle is attached to a component
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object that we are holding
	PhysicsHandle->SetTargetLocationAndRotation(GetReachLineEnd(), FRotator(0.0f, 0.0f, 0.0f));
	}	
}




