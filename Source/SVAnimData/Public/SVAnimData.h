// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SVAnimData.generated.h"

namespace SV
{
	namespace AnimData
	{
		static bool DrawDebug = true;
		static FAutoConsoleVariableRef CVarDrawDebug(
			TEXT("SV.AnimData.DrawDebug"),
			DrawDebug,
			TEXT("Should we draw the anim traces."),
			ECVF_Default);
	}
}

USTRUCT(BlueprintType, DisplayName = "... Base Class Do Not Use ...")
struct FSVAnimTrace
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName = "Shared Anim Trace")
struct FSVSharedAnimTraceReference : public FSVAnimTrace
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowedTypes = "SVSharedAnimTrace"))
	FPrimaryAssetId DataAssetId;
};

USTRUCT(BlueprintType, DisplayName = "Sphere Trace")
struct FSVAnimSphere : public FSVAnimTrace
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform RelativeTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRelativeToSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bRelativeToSocket", EditConditionHides, ToolTip = "Will be relative to Socket if enabled"))
	FName SocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radius = 50;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;
};

USTRUCT(BlueprintType, DisplayName = "Box Trace")
struct FSVAnimBox : public FSVAnimTrace
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform RelativeTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRelativeToSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bRelativeToSocket", EditConditionHides, ToolTip = "Will be relative to Socket if enabled"))
	FName SocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector HalfExtents = FVector(25);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;
};

USTRUCT(BlueprintType, DisplayName = "Capsule Trace")
struct FSVAnimCapsule : public FSVAnimTrace
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform RelativeTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRelativeToSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bRelativeToSocket", EditConditionHides, ToolTip = "Will be relative to Socket if enabled"))
	FName SocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radius = 25;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HalfHeight = 50;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;
};

class FSVAnimDataModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
