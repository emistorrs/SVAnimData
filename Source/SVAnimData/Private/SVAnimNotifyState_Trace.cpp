// Fill out your copyright notice in the Description page of Project Settings.


#include "SVAnimNotifyState_Trace.h"
#include "SVAnimAvatar.h"
#include "SVSharedAnimTrace.h"
#include "Engine/AssetManager.h"

void USVAnimNotifyState_Trace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Avatar = MeshComp->GetOwner();
	for (FInstancedStruct& TraceData: Traces)
	{
		DoTrace(MeshComp, TraceData);
	}
}

void USVAnimNotifyState_Trace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	for (FInstancedStruct& TraceData: Traces)
	{
		DoTrace(MeshComp, TraceData);
	}
}

void USVAnimNotifyState_Trace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	HitActors.Empty();
}

bool USVAnimNotifyState_Trace::ValidateSocket(const USkeletalMeshComponent* MeshComp, const FName SocketName) const
{
	bool bValid = true;

	if (!MeshComp->DoesSocketExist(SocketName) && MeshComp->GetBoneIndex(SocketName) == INDEX_NONE)
	{
		bValid = false;
	}

	return bValid;
}

void USVAnimNotifyState_Trace::CalculateRelativeTransform(const USkeletalMeshComponent* MeshComp,
	const FName& SocketName, bool bRelativeToSocket, const FTransform& RelativeTransform, FVector& OutLocation,
	FQuat& OutRotation) const
{
	if (bRelativeToSocket && ValidateSocket(MeshComp, SocketName))
	{
		const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
		OutLocation = SocketTransform.TransformPosition(RelativeTransform.GetLocation());
		OutRotation = SocketTransform.TransformRotation(RelativeTransform.GetRotation());
	} else
	{
		const FTransform MeshTransform = MeshComp->GetComponentTransform();
		OutLocation = MeshTransform.TransformPosition(RelativeTransform.GetLocation());
		OutRotation = MeshTransform.TransformRotation(RelativeTransform.GetRotation());
	}
}

void USVAnimNotifyState_Trace::DoTrace(const USkeletalMeshComponent* MeshComp, FInstancedStruct& TraceData)
{
	//unpack shared shapes and do their traces
	if (TraceData.GetScriptStruct() == FSVSharedAnimTraceReference::StaticStruct())
	{
		FSVSharedAnimTraceReference SharedShape = TraceData.GetMutable<FSVSharedAnimTraceReference>();
		if (UAssetManager* AM = UAssetManager::GetIfInitialized())
		{
			if (USVSharedAnimTrace* Shared = AM->GetPrimaryAssetObject<USVSharedAnimTrace>(SharedShape.DataAssetId))
			{
				for (FInstancedStruct InnerTrace : Shared->Traces)
				{
					DoTrace(MeshComp, InnerTrace);
				}
			}
			
		}
	}
	//otherwise we have inline traces
	else
	{
		bool bDrawDebug = SV::AnimData::DrawDebug;
		UWorld* World = MeshComp->GetWorld();
		TArray<FHitResult> Results;
		FVector Start;
		FVector End;
		FQuat Rot;
		ECollisionChannel Channel = ECC_WorldDynamic;
		FCollisionShape Shape;
		FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
		QueryParams.AddIgnoredActor(MeshComp->GetOwner());
		//do sphere
		if (TraceData.GetScriptStruct() == FSVAnimSphere::StaticStruct())
		{
			//set some shared variables
			FSVAnimSphere& Sphere = TraceData.GetMutable<FSVAnimSphere>();
			Channel = Sphere.CollisionChannel;
			Shape = FCollisionShape::MakeSphere(Sphere.Radius);
			//set start and rot
			CalculateRelativeTransform(MeshComp, Sphere.SocketName, Sphere.bRelativeToSocket, Sphere.RelativeTransform, Start, Rot);
			//end = start;
			End = Start;
#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
			if (bDrawDebug)
			{
				DrawDebugSphere(World, Start, Sphere.Radius, 12, DrawColor);
			}
#endif
		}
		//do box
		else if (TraceData.GetScriptStruct() == FSVAnimBox::StaticStruct())
		{
			//set some shared variables
			FSVAnimBox& Box = TraceData.GetMutable<FSVAnimBox>();
			Channel = Box.CollisionChannel;
			Shape = FCollisionShape::MakeBox(Box.HalfExtents);
			// set start and rot
			CalculateRelativeTransform(MeshComp, Box.SocketName, Box.bRelativeToSocket, Box.RelativeTransform, Start, Rot);
			End = Start;
#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
			if (bDrawDebug)
			{
				DrawDebugBox(World, Start, Box.HalfExtents, DrawColor);
			}
#endif
		}
		else if (TraceData.GetScriptStruct() == FSVAnimCapsule::StaticStruct())
		{
			//set some shared variables
			FSVAnimCapsule& Capsule = TraceData.GetMutable<FSVAnimCapsule>();
			Channel = Capsule.CollisionChannel;
			Shape = FCollisionShape::MakeCapsule(Capsule.Radius, Capsule.HalfHeight);
			//set start and rot
			CalculateRelativeTransform(MeshComp, Capsule.SocketName, Capsule.bRelativeToSocket, Capsule.RelativeTransform, Start, Rot);
			End = Start;
#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
			if (bDrawDebug)
			{
				DrawDebugCapsule(World, Start, Capsule.HalfHeight, Capsule.Radius, Rot, DrawColor);
			}
#endif
		}
		World->SweepMultiByChannel(Results, Start, End, Rot, Channel, Shape, QueryParams);
		for (FHitResult& Hit: Results)
		{
			if (bTriggerRepeatedly || !HitActors.Contains(Hit.GetActor()))
			{
				TryEmitHit(Hit);
				HitActors.Add(Hit.GetActor());
			}
		}
	}

}

void USVAnimNotifyState_Trace::TryEmitHit(const FHitResult& Hit) const
{
	if (Avatar.IsValid() && Avatar.Get()->GetClass()->ImplementsInterface(USVAnimAvatar::StaticClass()))
	{
		ISVAnimAvatar::Execute_HandleAnimTraceHit(Avatar.Get(), Hit, Tags, DataId);
	}
}