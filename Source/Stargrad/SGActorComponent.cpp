// Fill out your copyright notice in the Description page of Project Settings.


#include "SGActorComponent.h"

// Sets default values for this component's properties
USGActorComponent::USGActorComponent()
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

// Force Replication
bool USGActorComponent::IsSupportedForNetworking() const
{
	return true;
}


// Called every frame
void USGActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

