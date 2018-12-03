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
	UE_LOG(LogTemp, Warning, TEXT("Grabbing!"));

	///LINE TRACE and see if we reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
	AActor *ActorHit = HitResult.GetActor();
	///If we hit something then attach a physics handle
		
	if (ActorHit)
	{
		// TODO attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
	
	/* just a test to see wht happens with no rotation constraint. Objet flops around being under the control of physics engine
	if (ActorHit)
	{
		// TODO attach physics handle
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation()
		
		);
	}
	*/
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!"));
	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get Player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	//getPlayerViewPoint takes references and modifies their contents

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

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
	return Hit;
}

/// Look for attached Physics Handle
/// findComponentByClass is a generic. The diamond operators specify which component to look for.
void UGrabber::FindPhysicsHandleComponent() {

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		///Physics handle is found
	}
	else
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
		//Input handle
		UE_LOG(LogTemp, Warning, TEXT("Found Input Component of %s"), *GetOwner()->GetName())
		///Bind the input axis
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
	// Get Player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	//getPlayerViewPoint takes references and modifies their contents

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	// if the physics handle is attached to a component
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object that we are holding
	PhysicsHandle->SetTargetLocationAndRotation(LineTraceEnd, FRotator(0.0f, 0.0f, 0.0f));

	}
		


	
}




