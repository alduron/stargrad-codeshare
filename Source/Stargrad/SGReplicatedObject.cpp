// Fill out your copyright notice in the Description page of Project Settings.


#include "SGReplicatedObject.h"


/**
 *
 */

USGReplicatedObject::USGReplicatedObject()
{
	//Construct
}

void USGReplicatedObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here is where you would replcate all the properties present on your C++ object
	// using the DOREPLIFETIME(USGReplicatedObject, MyVar) macro.
	// Since this object has no C++ properties it can be skipped.

	// The code below is essential! This is what replicates all the variables that you add in blueprints.
	// This is it in its final form. No modification needs to be done to this when adding BP variables.
	if (const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

// Get outer's world context
UWorld* USGReplicatedObject::GetWorld() const
{
	if (const UObject* MyOuter = GetOuter())
	{
		return MyOuter->GetWorld();
	}
	return nullptr;
}

// Get outer as owner
AActor* USGReplicatedObject::GetOwningActor() const
{
	return GetTypedOuter<AActor>();
}

// Override replication setting
bool USGReplicatedObject::IsSupportedForNetworking() const
{
	return true;
}

// Get owning actor's RPC context I think
int32 USGReplicatedObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	check(GetOuter() != nullptr);
	return GetOuter()->GetFunctionCallspace(Function, Stack);
}

// Perform RPC call from Owner's context...I dont think I need RPC in Objects though.
bool USGReplicatedObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	check(!HasAnyFlags(RF_ClassDefaultObject));

	AActor* Owner = GetOwningActor();
	UNetDriver* NetDriver = Owner->GetNetDriver();
	if (NetDriver)
	{
		NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
		return true;
	}

	return false;
}

void USGReplicatedObject::Destroy()
{
	if (!IsValid(this))
	{
		checkf(GetOwningActor()->HasAuthority() == true, TEXT("Destroy:: Object does not have authority to destroy itself!"));

		OnDestroyed.Broadcast(this); // Notifies any blueprints that have bound to the event that we are being destroyed. The "this" denotes that this is the object begin destroyed.
		BeginDestroy();
		MarkAsGarbage();
	}
}

//void USGReplicatedObject::OnDestroyed()
//{
//	//Alert actor or something?
//}

// Updated definition
void USGReplicatedObject::BeginDestroy_Implementation() // Note: the "_Implementation" bit at the end is a c++ only thing and doesn't apply to blueprints. This is required because of the "BlueprintNativeEvent" specifier.
{
	// This is where you would handle anything related to deletion of this object.
	// Note: Since this is now a blueprint native event this function can have an implementation in blueprints instead of here.
	//       It can also be overridden with each child class.
	// IE. Sending a notification to a child object to also get destroyed because this is being destroyed.
	// Warning: It is not good practice to alert external classes about this object's destruction from here. Use the OnDestroyed Event for that instead.
	// IE. The player has a list of these objects and this one will get deleted.
	//     IT IS NOT this object's responsibility to clean up its information in the player's list.
	//     This goes against OOP principles pretty heavily.
	//     Use the OnDestroyed event for that instead.
	//     When the player adds this object to the list it also subcribes to the event,
	//     then the event will be triggered and the player class can deal with its own clean up as it sees fit.
}



bool USGReplicatedObject::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = false;

	// Loop through all the subobjects we want to replicate, replicate the object's subobjects, then replicate the object.
	for (USGReplicatedObject* Obj : SubobjectsToReplicate)
	{
		if (IsValid(Obj)) // ensure that the object isn't null
		{
			bWroteSomething |= Obj->ReplicateSubobjects(Channel, Bunch, RepFlags); // Replicate the subobjects of "Obj"
			bWroteSomething |= bWroteSomething |= Channel->ReplicateSubobject(Obj, *Bunch, *RepFlags); // Replicate "Obj"
		}
	}

	return bWroteSomething;
}
