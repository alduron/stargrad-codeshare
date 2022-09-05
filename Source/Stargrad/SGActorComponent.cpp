// Fill out your copyright notice in the Description page of Project Settings.


#include "SGActorComponent.h"

// Sets default values for this component's properties
USGActorComponent::USGActorComponent(const FObjectInitializer& OI)
	: Super(OI)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USGActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USGActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/* These do not compile? Am I missing an include class?
	* Am I able to replicate any variable that I add in Blueprints?
	DOREPLIFETIME(USGActorComponent, LocalInventory);
	DOREPLIFETIME(USGActorComponent, BagInventory);
	*/
}

bool USGActorComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	/* Same thing here, does not compile.
	* I assume I also need these called dynamically for any variables I include in Blueprints?
	bWroteSomething |= Channel->ReplicateSubobjectList(LocalInventory, *Bunch, *RepFlags);
	bWroteSomething |= Channel->ReplicateSubobjectList(BagInventory, *Bunch, *RepFlags);
	*/
	return bWroteSomething;
}


// Called every frame
void USGActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

