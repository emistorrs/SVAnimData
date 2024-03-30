// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "SVAnimDataSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "SV Anim Data"))
class SVANIMDATA_API USVAnimDataSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category=Debug, meta=(ConsoleVariable="SV.AnimData.DrawDebug"))
	bool DrawDebug;
};
