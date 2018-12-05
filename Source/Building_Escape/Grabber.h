// Copyright Robert Hinds 2018

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()
private:
	//How far ahead of the player can we reach in cm
	float Reach = 100.f;
	UPhysicsHandleComponent *PhysicsHandle = nullptr;
	UInputComponent *InputComponent = nullptr;
	
	//finds (assumed) attached physics handle
	void FindPhysicsHandleComponent();

	//Setup (assumed) attached input component
	void SetupInputComponent();

	///Agruement could be made that we could have an inputcontroller someone where else
	///and the grabber class registers with it. Then it would be the controllers job
	///to call the grabber and have it do a grab or release function. This would remove
	///having the input controller code inside the grabber class. The reason we did it this
	///is because of simplicity. It sounds like this is not the way we should be doing it.
	//Ray-cast and grab what is in reach
	void Grab();
	//Call when input for grab is released
	void Release();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	FVector GetReachLineEnd();
	FVector GetReachLineStart();

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};


