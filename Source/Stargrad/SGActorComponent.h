// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h" // This is needed for any network related C++ code.
#include "Engine/ActorChannel.h" // Required for using actor channels
#include "SGActorComponent.generated.h"

// Blueprintable may be wrong in this context, not sure.  // Not at all. This means that the class will be able to be inherited from by blueprints.
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable, BlueprintType) // Adding BlueprintType will allow this class to be referenced in blueprints.
class STARGRAD_API USGActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USGActorComponent(); // Note: The "const FObjectInitializer& OI" isn't needed anymore, and is deprecated if I recall.
													 // The function declaration can just as well be USGActorComponent(); with nothing in the parenthesis.
													 // Remember to change the .cpp file as well. However, it's not hurting anything so your call.

	// Override list of replicated variables?   // Not exactly. This function tells the replication system what variables need to be replicated. And yes it is overridden from the actor component.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // Anything not in here won't be replicated.

	// Override replication of sub objects?    // Yes, this overrides the replicate subobjects function to allow you to add additional objects that you want to be replicated along with this object.
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;


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



// The functions below are uneeded and can be deleted. Don't worry they will still work in Blueprints. Remember to delete from the .cpp as well.
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};