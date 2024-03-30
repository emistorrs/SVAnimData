// Fill out your copyright notice in the Description page of Project Settings.


#include "SVSharedAnimTrace.h"

void USVSharedAnimTrace::SetAssetPreviewValues()
{
	PrimaryDataAssetId = GetPrimaryAssetId().ToString();
}

FPrimaryAssetId USVSharedAnimTrace::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}

void USVSharedAnimTrace::PostLoad()
{
	Super::PostLoad();

	SetAssetPreviewValues();
}

void USVSharedAnimTrace::PostDuplicate(bool bDuplicateForPIE)
{
	Super::PostDuplicate(bDuplicateForPIE);

	// Guard against PIE
	// TODO: Do we really need to guard here? We should confirm what causes bDupliateForPIE to be true/false -Oak
	if (bDuplicateForPIE)
	{
		return;
	}

	SetAssetPreviewValues();
}

void USVSharedAnimTrace::PostInitProperties()
{
	Super::PostInitProperties();

	SetAssetPreviewValues();
}

void USVSharedAnimTrace::PostRename(UObject* OldOuter, const FName OldName)
{
	Super::PostRename(OldOuter, OldName);

	SetAssetPreviewValues();
}

