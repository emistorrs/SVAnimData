// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SVSharedAnimTrace.generated.h"

/**
 * A data asset that defines some animation shapes. Should probably be used for only 1 skeleton, but if the socket names matches,
 * it can be used across multiple
 */
UCLASS(BlueprintType)
class SVANIMDATA_API USVSharedAnimTrace : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BaseStruct = "/Script/SVAnimData.SVAnimTrace", ExcludeBaseStruct),
		Category="SVAnimData")
	TArray<FInstancedStruct> Traces;

	/*
	 * UPrimaryDataAsset overrides
	 */

	// Overriden because of AssetManager string matching complications:
	// https://forums.unrealengine.com/t/searching-for-primarydataassets/125813/2
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "PrimaryDataAsset")
	FString PrimaryDataAssetId;

	void SetAssetPreviewValues();

	/*
	* UObject overrides
	*/
	virtual void PostLoad() override;

	virtual void PostDuplicate(bool bDuplicateForPIE) override;

	virtual void PostInitProperties() override;

	virtual void PostRename(UObject* OldOuter, const FName OldName) override;

	inline static FPrimaryAssetType AssetType = FPrimaryAssetType("SVSharedAnimTrace");
};
