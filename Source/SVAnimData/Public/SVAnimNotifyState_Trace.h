// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InstancedStruct.h"
#include "SVAnimData.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SVAnimNotifyState_Trace.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SVANIMDATA_API USVAnimNotifyState_Trace : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TWeakObjectPtr<AActor> Avatar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BaseStruct = "/Script/SVAnimData.SVAnimTrace", ExcludeBaseStruct), Category="SVAnimData")
	TArray<FInstancedStruct> Traces;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SVAnimData")
	FGameplayTagContainer Tags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SVAnimData")
	FPrimaryAssetId DataId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SVAnimData")
	bool bTriggerRepeatedly = false;

	TSet<TWeakObjectPtr<AActor>> HitActors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SVAnimData | Debug")
	FColor DrawColor = FColor::Red;

	virtual void NotifyBegin(class USkeletalMeshComponent * MeshComp, class UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(class USkeletalMeshComponent * MeshComp, class UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	bool ValidateSocket(const USkeletalMeshComponent* MeshComp, const FName SocketName) const;

	void CalculateRelativeTransform(const USkeletalMeshComponent* MeshComp, const FName& SocketName, bool bRelativeToSocket, const FTransform& RelativeTransform, FVector& OutLocation, FQuat& OutRotation) const;

	UFUNCTION()
	void DoTrace(const USkeletalMeshComponent* MeshComp, FInstancedStruct& TraceData);

	UFUNCTION()
	void TryEmitHit(const FHitResult& Hit) const;
};
