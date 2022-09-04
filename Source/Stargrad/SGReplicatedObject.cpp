// Fill out your copyright notice in the Description page of Project Settings.


#include "SGReplicatedObject.h"


/**
 * 
 */

USGReplicatedObject::USGReplicatedObject()
{
	//Construct
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

		OnDestroyed();
		MarkAsGarbage();
	}
}

void USGReplicatedObject::OnDestroyed()
{
	//Alert actor or something?
}
