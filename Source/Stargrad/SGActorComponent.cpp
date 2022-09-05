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

	// Set the object to replicate by default. Otherwise IsReplicated must be set true in blueprints. Easier to do it here.
	// Replication can always be turned off on child classes in the normal way by unticking the IsReplicated checkbox in the properties panel.
	SetIsReplicatedByDefault(true);
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

	// Delete these. DOREPLIFETIME is used for C++ variables only.
	/* These do not compile? Am I missing an include class?
	* Am I able to replicate any variable that I add in Blueprints?
	DOREPLIFETIME(USGActorComponent, LocalInventory);   // Use the DOREPLIFETIME macro for C++ variables only!
	DOREPLIFETIME(USGActorComponent, BagInventory);     // If your variable is declared in blueprints than it will work just like any other replicated variable.
	*/													// Just set it to replicate in blueprints and the code below will handle it automatically.

	// Yes, any variable that you add in blueprints will be able to be replicated on this calss, however it is done in the normal way
	// in blueprints. You do not need to reference them here.
	// Below is the code that allows BP variables to be replicated.

	if (const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

bool USGActorComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags); // Although what you did here is generally good practice,
																				 // it is not needed here as UActorComponent::ReplicateSubobjects()
																				 // will always return false.
																				 // so "bool bWroteSomething = false;" would be equivalent.
																				 // Its not hurting anything so up to you if you want to change it or not.
	/* Same thing here, does not compile.
	* I assume I also need these called dynamically for any variables I include in Blueprints?
	bWroteSomething |= Channel->ReplicateSubobjectList(LocalInventory, *Bunch, *RepFlags);
	bWroteSomething |= Channel->ReplicateSubobjectList(BagInventory, *Bunch, *RepFlags);
	*/
	// You have the right idea here, just note that only object properties go here, so not an int or string.
	// Also, this only works for object properties declared in C++. I'm assuming that LocalInventory and BagInventory are declared in blueprints,
	// hence why it isn't compiling.

	// Loop through all the subobjects we want to replicate, replicate the object's subobjects, then replicate the object.
	for (USGReplicatedObject Obj : SubobjectsToReplicate)
	{
		bWroteSomething |= Obj->ReplicateSubobjects(Channel, Bunch, RepFlags); // Replicate the subobjects of "Obj"
		bWroteSomething |= bWroteSomething |= Channel->ReplicateSubobject(Obj, *Bunch, *RepFlags); // Replicate "Obj"
	}

	return bWroteSomething;
}


// Called every frame
void USGActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

