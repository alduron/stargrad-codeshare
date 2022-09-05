// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Net/UnrealNetwork.h" // This is needed for any network related C++ code.
#include "Engine/ActorChannel.h" // Required for using actor channels
#include "SGReplicatedObject.generated.h" 

// This creates a blueprint event that will be called whenever the object is destroyed. This is a better way of having other classes notified of this classes destruction. 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReplicatedObjectOnDestoyedSignature, class USGReplicatedObject*, DestroyedObject);

/**
 *
 */
UCLASS(Blueprintable, BlueprintType) // Allow this class to be inherited from and referenced in blueprints.
class STARGRAD_API USGReplicatedObject : public UObject
{
	GENERATED_BODY()

public:
	USGReplicatedObject();

	// Allows the OnDestroyed event to be binded to in blueprints.
	UPROPERTY(BlueprintAssignable)
		FReplicatedObjectOnDestoyedSignature OnDestroyed;

	// THIS FUNCTION IS ESSENTIAL IF YOU WANT YOU OBJECTS TO BE ABLE TO REPLICATE THEIR PROPERTIES!!!!
	// Replicates the properties on an object.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Get outer's world context   // Correct. For reference outer is just the object that is the parent of this object.
	virtual UWorld* GetWorld() const override;

	// Get outer as owner   // A more descriptive comment would be "Gets the nearest parent that is an actor".
	// This is NOT getting the outer and casting it to type AActor. Instead it is walking up the outer chain of objects until it finds an object that is an actor.
	UFUNCTION(BlueprintPure, Category = "Replicated Object")
		AActor* GetOwningActor() const;

	// Override replication setting   // Yep, very simple.
	UFUNCTION(BlueprintPure, Category = "Replicated Object")
		virtual bool IsSupportedForNetworking() const override;

	// Get owning actor's RPC context I think   // Yes, you are exactly correct.
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;

	// Perform RPC call from Owner's context...I dont think I need RPC in Objects though.   // RPCs are totally optional to implement on objects.
	// If you don't need them than you can delete this function and the one above. Also make sure to delete from the .cpp file as well.
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override;

	//This is needed to provide a custom destroy function to ensure proper conditions are met
	// This is totally valid, you just have to be aware that objects behave differently than actors. Actors must be explicitly destroyed for them to stop existing.
	// This is because a reference to them is always held in the world. Objects do not have to exist in the world, so there is no garuntee that there is a reference to them.
	// If there is no references kept to an object it will be automatically removed by the garbage collector. On one hand this is nice as you do not need to explicitly define a destroy function,
	// because if the object looses all references it gets destroyed automatically. However, this means you also must take extra care when destroying the object,
	// as there could still be references that will become null when you destroy it. That's not a bad thing just something that you will have to be aware of.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Replicated Object")
		void Destroy();


	// This function is denoted as protected and virtual and therefore will not be able to be overridden or called from blueprints.
	// The protected part is good as that limits the access of this function to this class only, however it should have a UFUNCTION tag on it.
protected:
	// virtual void OnDestroyed();

	// Updated function declaration
	//UFUNCTION(BlueprintNativeEvent) // BlueprintNativeEvent specifier allows this function to be overridden in blueprints.
		//void BeginDestroy(); // Renamed to BeginDestroy to be more similar to the actor naming scheme for destory functions.
		// Note: This function is not callable in blueprints due to the lack of the BlueprintCallable specifier.
		//       This is by design as the fuction is called automatically when the Destroy() function is called.



public:
	// Everything up to this point was standard replication stuff for replicating objects in C++ only. However, since you want to work primarily in blueprints
	// some additional code can be setup to allow any child of the SGReplicatedObject class to be replicated entirely in blueprints.
	// The first part of this is creating an array of all the subobjects we want to replicate.
	// THE OBJECTS IN THIS ARRAY SHOULD ONLY BE SUBOBJECTS OF THIS OBJECT!!!
	// You will be able to add and remove objects from this list entirely in blueprints.
	UPROPERTY(BlueprintReadWrite, Category = "Replication")
		TArray<class USGReplicatedObject*> SubobjectsToReplicate;

	// To get any given object, or object list, to replicate, you must have the object, or object list, set to replicate in blueprints,
	// then add it to the SubobjectsToReplicate array. Note: This will only work with objects derived from this class.
	// This will allow you to have multiple levels of object replication. Subobjects replicating subobjects replicating subobjects etc.
	// If a given instance of this class doesn't need to replicate any subobjects, then don't add any to the SubobjectsToReplicate list.

	// The second part is the ReplicateSubobjects function where we replicate all the objects in the above list.

	void BeginDestroy_Implementation();

	// Replicate any subobjects this object has. Note: the override keyword isn't present as this function doesn't exist on UObject.
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags);

};
