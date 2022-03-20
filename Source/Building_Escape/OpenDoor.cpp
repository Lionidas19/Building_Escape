

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

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

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	OpenAngle += InitialYaw;

	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("5s has the opern door component on it, but no pressureplate set!"), *GetOwner()->GetName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if(!PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		if(GetWorld()->GetTimeSeconds() - DoorLastOpened >= DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning,TEXT("%f"), OpenAngle);

	CurrentYaw = GetOwner()->GetActorRotation().Yaw;

	FRotator NewRotation(0.f, FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed/*Current Yaw, Target Yaw, 0-1*/), 0.f);

	GetOwner()->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning,TEXT("%f"), OpenAngle);

	CurrentYaw = GetOwner()->GetActorRotation().Yaw;

	FRotator NewRotation(0.f, FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed/*Current Yaw, Target Yaw, 0-1*/), 0.f);

	GetOwner()->SetActorRotation(NewRotation);
}