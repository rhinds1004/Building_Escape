// Copyright Robert Hinds 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "PlatformMover.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlatformEvent);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UPlatformMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlatformMover();

protected:
	float GetTotalMassOFActorsOnTriggerPlate();
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FPlatformEvent OnUp;

	UPROPERTY(BlueprintAssignable)
		FPlatformEvent OnDown;
	UPROPERTY(EditAnyWhere)
		FVector InitialLocationVector;




private:
	UPROPERTY(EditAnywhere)
		ATriggerVolume *PressurePlate = nullptr;

	UPROPERTY(EditAnyWhere)
		float TriggerPlateTotalMassThreshold = 40;

	FVector MaxPlatformHeight = { 0.f, 0.f, 100.0f };
};
