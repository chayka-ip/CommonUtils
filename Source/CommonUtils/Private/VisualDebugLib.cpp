// Fill out your copyright notice in the Description page of Project Settings.


#include "VisualDebugLib.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

void UVisualDebugLib::ActorDrawDebugVector(AActor* Actor, FVector VectorEnd, int LengthMultiplier,
                                           FLinearColor Color, float LifeTime, float Thickness)
{
// #if WITH_EDITOR
	if(Actor)
	{
		const FVector ActorLocation = Actor->GetActorLocation();
		const FVector End = (VectorEnd.GetSafeNormal() * LengthMultiplier) + ActorLocation;
		DrawDebugLine(Actor->GetWorld(), ActorLocation, End, Color.ToFColor(true), false, LifeTime, 0, Thickness);
	}
// #endif
}

void UVisualDebugLib::ActorDrawDebugSphere(AActor* Actor, const FVector Center, const float Radius, const int Segments, const FColor Color, const float LifeTime)
{
// #if WITH_EDITOR
    DrawDebugSphere(Actor->GetWorld(),Center,Radius,Segments,Color,false, LifeTime, 0, 5);
// #endif
}

void UVisualDebugLib::ActorDrawDebugPoint(AActor* Actor, const FVector Position, const float Size, const FColor Color, const float Time)
{
// #if WITH_EDITOR
	DrawDebugPoint(Actor->GetWorld(),Position,Size,Color,false,Time, 0);
// #endif
}

void UVisualDebugLib::ActorDrawTrajectoryByPointArray(AActor* Actor, const TArray<FVector>& PointArray, const float LifeTime, const FColor Color, const float Thickness)
{
// #if WITH_EDITOR
		if(PointArray.Num()>1)
		{
			const UWorld* World = Actor->GetWorld();

			FVector PreviousPoint = PointArray[0];
			for(auto& Point:PointArray)
			{
				if(!(Point == PreviousPoint))
				{
					DrawDebugLine(World, PreviousPoint, Point, Color, false, LifeTime, 0, Thickness);
					PreviousPoint = Point;
				}
			}
		}
// #endif
}

void UVisualDebugLib::DrawTrajectory(UObject* WorldContextObject, const TArray<FVector>& PointArray, float DrawTime, FLinearColor Color,
										ETrajectoryDrawType Type, float Thickness, FVector Offset, bool bClose)
{
	const auto W = WorldContextObject->GetWorld();
	const FColor _Color = Color.ToFColor(true);

	TArray<FVector> PArray = PointArray;
	
	if(!Offset.IsZero())
	{
		for (int i = 0; i < PArray.Num(); ++i) PArray[i] += Offset;
	}
	
	if(PArray.Num()>1)
	{
		constexpr float SphereR = 5;
		constexpr float SphereSeg = 4;
		
		FVector PreviousPoint = PArray[0];

		if(Type == ETDT_Spheres || Type == ETDT_ConnectedSpheres)
		{
			DrawDebugSphere(W, PreviousPoint, SphereR, SphereSeg, _Color, false, DrawTime, 0, Thickness);
		}
		
		for(auto Point:PArray)
		{
			if(!Point.Equals(PreviousPoint))
			{
				if(Type == ETDT_Line)
				{
					DrawDebugLine(W, PreviousPoint, Point, _Color, false, DrawTime, 0, Thickness);

				}
				else if(Type == ETDT_Spheres)
				{
					DrawDebugSphere(W, Point, SphereR, SphereSeg, _Color, false, DrawTime, 0, Thickness);
				}
				else if(Type == ETDT_ConnectedSpheres)
				{
					DrawDebugLine(W, PreviousPoint, Point, _Color, false, DrawTime, 0, Thickness);
					DrawDebugSphere(W, Point, SphereR, SphereSeg, _Color, false, DrawTime, 0, Thickness);
				}
				PreviousPoint = Point;
			}
		}

		if(bClose)
		{
			DrawDebugLine(W, PArray.Last(), PArray[0], _Color, false, DrawTime, 0, Thickness);
		}
	}
}

void UVisualDebugLib::DrawPointArray(UObject* WorldContextObject, const TArray<FVector>& PointArray, FLinearColor Color, float Size, float DrawTime)
{
	for (const auto P : PointArray)
	{
		UKismetSystemLibrary::DrawDebugPoint(WorldContextObject, P, Size, Color, DrawTime);
	}
}

void UVisualDebugLib::DrawActorLocation(AActor* Actor, bool bXY, float Radius, int Segments, FLinearColor Color, float LifeTime)
{
	FVector V = Actor->GetActorLocation();
	if(bXY)
	{
		V.Z = 0;
	}
	ActorDrawDebugSphere(Actor, V, Radius, Segments, Color.ToFColor(true), LifeTime);
}

FLinearColor UVisualDebugLib::GetColorByBool(bool B)
{
	return B ? FLinearColor::Green : FLinearColor::Red;
}

void UVisualDebugLib::DrawQuatFromPoint(UObject* WorldContextObject, FQuat Q, FVector Point, float Distance, float DrawTime, float Thickness)
{
	const FLinearColor ColorForward = FLinearColor::Red;
	const FLinearColor ColorRight = FLinearColor::Yellow;
	const FLinearColor ColorUp = FLinearColor::Blue;

	const FVector ForwardEnd = Point + Q.GetForwardVector() * Distance;
	const FVector RightEnd = Point + Q.GetRightVector() * Distance;
	const FVector UpEnd = Point + Q.GetUpVector() * Distance;

	UKismetSystemLibrary::DrawDebugLine(WorldContextObject, Point, ForwardEnd, ColorForward, DrawTime, Thickness);
	UKismetSystemLibrary::DrawDebugLine(WorldContextObject, Point, RightEnd, ColorRight, DrawTime, Thickness);
	UKismetSystemLibrary::DrawDebugLine(WorldContextObject, Point, UpEnd, ColorUp, DrawTime, Thickness);
}
