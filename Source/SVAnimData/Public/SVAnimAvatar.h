// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SVAnimData.h"
#include "UObject/Interface.h"
#include "SVAnimAvatar.generated.h"

UINTERFACE()
class USVAnimAvatar : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SVANIMDATA_API ISVAnimAvatar
{
	GENERATED_BODY()

public:
	// Event that contains the hit results, tags, and dataId for each AnimShape collision that happens in any attached skeletal mesh
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HandleAnimTraceHit(const FHitResult& Hit, const FGameplayTagContainer& Tags, const FPrimaryAssetId& DataId);
};
