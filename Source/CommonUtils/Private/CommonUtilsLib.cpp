// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonUtils/Public/CommonUtilsLib.h"
#include "Kismet/KismetSystemLibrary.h"

float UCommonUtilsLib::ValueFromEGSTDM(const EGSTimeDilationMul E)
{
    switch (E)
    {
        case EGSTDM_1: return 1;
        case EGSTDM_1_2: return 1.0f/2;
        case EGSTDM_1_4: return 1.0f/4;
        case EGSTDM_1_8: return 1.0f/8;
        case EGSTDM_1_16: return 1.0f/16;
        case EGSTDM_1_32: return 1.0f/32;
        case EGSTDM_1_64: return 1.0f/64;
        case EGSTDM_0: return 0;
    }
    return 1;
}

float UCommonUtilsLib::GetGlobalTimeDilation(const UObject* WorldContextObject)
{
    if (WorldContextObject)
    {
        const auto World = WorldContextObject->GetWorld();
        if (World)
        {
            const auto WorldSettings = World->GetWorldSettings();
            if (WorldSettings)
                return WorldSettings->TimeDilation;
        }
    }
    return 1;
}

void UCommonUtilsLib::SetGlobalTimeDilation(const UObject* WorldContextObject, const EGSTimeDilationMul E)
{
    const float Dilation = ValueFromEGSTDM(E);
    if (WorldContextObject)
    {
        const auto World = WorldContextObject->GetWorld();
        if (World)
        {
            const auto WorldSettings = World->GetWorldSettings();
            if (WorldSettings)
            {
                WorldSettings->SetTimeDilation(Dilation);
            }
        }
    }
}

UAnimSequenceBase* UCommonUtilsLib::GetAnimSequence(const FString Path, UObject* Outer)
{
    return LoadObject<UAnimSequenceBase>(Outer, *Path, NULL, LOAD_None, NULL);
}

void UCommonUtilsLib::GetLocationArrayFromTransformArray(const TArray<FTransform>& T, TArray<FVector>& V)
{
    for (auto El : T)
    {
        V.Add(El.GetLocation());
    }
}

TArray<FVector> UCommonUtilsLib::GetLocationsFromTransforms(const TArray<FTransform>& T)
{
    TArray<FVector> V;
    GetLocationArrayFromTransformArray(T, V);
    return V;
}

float UCommonUtilsLib::GetTickTimeScaled(const UObject* WorldContextObject)
{
    if(WorldContextObject)
    {
        const auto World = WorldContextObject->GetWorld();
        if(World) return World->GetDeltaSeconds();
    }
    return 0;
}

float UCommonUtilsLib::GetTickTimeUnscaled(const UObject* WorldContextObject)
{
    return GetTickTimeScaled(WorldContextObject) / GetGlobalTimeDilation(WorldContextObject);
}

float UCommonUtilsLib::GetTickDebugTime(const UObject* WorldContextObject, float Mul)
{
    return GetTickTimeScaled(WorldContextObject) * Mul;
}

float UCommonUtilsLib::GetForeverTime()
{
    return FOREVER_TIME;
}

void UCommonUtilsLib::PrintStringWithinTick(UObject* WorldContextObject, const FString& InString, bool bPrintToScreen, bool bPrintToLog,
    FLinearColor TextColor, float TimeMul)
{
    const float Time = GetTickDebugTime(WorldContextObject, TimeMul);
    UKismetSystemLibrary::PrintString(WorldContextObject, InString, bPrintToScreen, bPrintToLog, TextColor, Time);
}

void UCommonUtilsLib::PrintStringForever(UObject* WorldContextObject, const FString& InString, bool bPrintToScreen, bool bPrintToLog,
    FLinearColor TextColor)
{
    const float Time = GetForeverTime();
    UKismetSystemLibrary::PrintString(WorldContextObject, InString, bPrintToScreen, bPrintToLog, TextColor, Time);
}

UStaticMesh* UCommonUtilsLib::GetCubeMesh()
{
    const auto P = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(P);
    return BaseMeshAsset.Object ? BaseMeshAsset.Object : nullptr;
}

UStaticMesh* UCommonUtilsLib::GetSphereMesh()
{
    const auto P = TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'");
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(P);
    return BaseMeshAsset.Object ? BaseMeshAsset.Object : nullptr;
}

void UCommonUtilsLib::WriteFloatCurveToAsset(const FRuntimeFloatCurve& Curve, UCurveFloat* CurveAsset)
{
    check(CurveAsset)
    const auto RefCurveAsset = &CurveAsset->FloatCurve;
    RefCurveAsset->Reset();

    auto InKeys = Curve.GetRichCurveConst()->Keys;
    for (const auto Key : InKeys)
    {
        RefCurveAsset->AddKey(Key.Time, Key.Value);
    }
}
