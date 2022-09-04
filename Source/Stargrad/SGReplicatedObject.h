// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SGReplicatedObject.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class STARGRAD_API USGReplicatedObject : public UObject
{
	GENERATED_BODY()

public:
	USGReplicatedObject();

	// Get outer's world context
	virtual UWorld* GetWorld() const override;

	// Get outer as owner
	UFUNCTION(BlueprintPure, Category = "Replicated Object")
	AActor* GetOwningActor() const;

	// Override replication setting
	UFUNCTION(BlueprintPure, Category = "Replicated Object")
	virtual bool IsSupportedForNetworking() const override;

	// Get owning actor's RPC context I think
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;

	// Perform RPC call from Owner's context...I dont think I need RPC in Objects though.
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override;

	//This is needed to provide a custom destroy function to ensure proper conditions are met
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Replicated Object")
	void Destroy();


protected:
	virtual void OnDestroyed();
};
