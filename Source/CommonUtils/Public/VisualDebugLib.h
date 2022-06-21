// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VisualDebugLib.generated.h"

UENUM()
enum ETrajectoryDrawType
{
	// ETDT_Points UMETA(DisplayName = "Points"),
	ETDT_Line UMETA(DisplayName = "Line"),
	ETDT_Spheres UMETA(DisplayName = "Spheres"),
	ETDT_ConnectedSpheres UMETA(DisplayName = "Connected Spheres")
};

/**
 * 
 */
UCLASS()
class COMMONUTILS_API UVisualDebugLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf="Actor"))
	static void ActorDrawDebugVector(UPARAM(ref) AActor* Actor,  FVector VectorEnd,  int LengthMultiplier = 2000,  FLinearColor Color=FLinearColor::Green,  float LifeTime = 0.115f, float Thickness = 2);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ActorDrawDebugSphere(AActor* Actor, FVector Center, float Radius = 8, int Segments = 4, FColor Color = FColor::Green, float LifeTime = 0.01f);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ActorDrawDebugPoint(AActor* Actor,  FVector Position,float Size = 100, FColor Color=FColor::Green,  float Time = 0.01f);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ActorDrawTrajectoryByPointArray(AActor* Actor,const TArray<FVector> &PointArray, float LifeTime = 0.01f,FColor Color=FColor::Yellow,float Thickness = 5);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void DrawTrajectory(UObject* WorldContextObject, const TArray<FVector>& PointArray, float DrawTime, FLinearColor Color, ETrajectoryDrawType Type=ETDT_Line, float Thickness=2, FVector Offset=FVector::ZeroVector, bool bClose=false);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void DrawPointArray(UObject* WorldContextObject, const TArray<FVector>& PointArray, FLinearColor Color=FLinearColor::Green, float Size = 2.0f, float DrawTime=0.1f);
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void DrawActorLocation(AActor* Actor, bool bXY=false, float Radius = 8, int Segments = 4, FLinearColor Color = FLinearColor::Green, float LifeTime = 0.01f);

	UFUNCTION(BlueprintPure)
	static FLinearColor GetColorByBool(bool B);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void DrawQuatFromPoint(UObject* WorldContextObject, FQuat Q, FVector Point, float Distance, float DrawTime, float Thickness);
};
