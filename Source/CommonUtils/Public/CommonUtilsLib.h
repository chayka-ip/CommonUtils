// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/GameEngine.h"

#include "CommonUtilsLib.generated.h"

static constexpr float FOREVER_TIME = TNumericLimits< float >::Max();

UENUM(BlueprintType)
enum EGSTimeDilationMul
{
	EGSTDM_1 UMETA(DisplayName = "1"),
	EGSTDM_1_2 UMETA(DisplayName = "1/2"),
	EGSTDM_1_4 UMETA(DisplayName = "1/4"),
	EGSTDM_1_8 UMETA(DisplayName = "1/8"),
	EGSTDM_1_16 UMETA(DisplayName = "1/16"),
	EGSTDM_1_32 UMETA(DisplayName = "1/32"),
	EGSTDM_1_64 UMETA(DisplayName = "1/64"),
	EGSTDM_0 UMETA(DisplayName = "0"),
};

/*
* StableFrame = 1/FixedFrameRate
* Render Frame = StableFrame / TimeDilation 
* Physics, animation execution rely on Stable Frames
*
* The bigger time dilation - the bigger delta time
*
* for 60 fps: 
* 20 - 0.333333 (limit)
* 10 - 0.166667
* 1 - 0.016667 
* 0.5 - 0.008333
*
*  *  there are time types:
*  1) real time - time in real life
*  2) game time - is constant relative to the game world and is not changed because of time dilation
*  3) tick time - delta seconds between ticks
*
*  Example: TimeDilation is 2:
*		- real time is not changed;
*		- game time is not changed (but feels twice faster for human)
*		- tick time became twice smaller (twice more ticks needed to elapse same game time) 
*/
UCLASS()
class COMMONUTILS_API UCommonUtilsLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	static float ValueFromEGSTDM(const EGSTimeDilationMul E);
	
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static float GetGlobalTimeDilation(const UObject* WorldContextObject);

	/*
	 *  time dilation affects much on physics simulation
	 *  now default parameters are: 60 fps locked for general settings, 120 fps - physics calculation substeps
	 *  working time dilations : 1 , 0.5, 0.25, 0.125 ...
	 */	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static  void SetGlobalTimeDilation(const UObject* WorldContextObject, const EGSTimeDilationMul E);

	UFUNCTION(BlueprintPure)
	static float FramesToSeconds(const float Frames, const float FPS)
	{
		return FPS == 0 ? 0 : Frames/FPS;
	}

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static float GetFixedFrameRate() {return GEngine ? GEngine->FixedFrameRate : 15;}
	
	UFUNCTION(BlueprintPure)
	static float StableFramesToSeconds(const float Frames)
	{
	    return Frames / GetFixedFrameRate();
	}

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
    static int StableFramesToRenderFrames(const UObject* WorldContextObject, const float StableFrames)
	{
		return FMath::RoundHalfFromZero(StableFrames / GetGlobalTimeDilation(WorldContextObject));
	}

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static float StableFramesToRenderSeconds(const UObject* WorldContextObject, const float StableFrames)
	{
		return RenderFramesToSeconds(WorldContextObject, StableFramesToRenderFrames(WorldContextObject, StableFrames));
	}

	//1 render frame equals 1 tick update
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static float RenderFramesToSeconds(const UObject* WorldContextObject, const float Frames)
	{
		return RenderFramesToSeconds(Frames, GetGlobalTimeDilation(WorldContextObject));
	}
	
	static float RenderFramesToSeconds(const float Frames, float TimeDilation=1)
	{
		return StableFramesToSeconds(Frames) * TimeDilation;
	}

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static int RenderFramesToStableFrames(const UObject* WorldContextObject, const float RenderFrames)
	{
		return RenderFramesToStableFrames(RenderFrames, GetGlobalTimeDilation(WorldContextObject));
	}
	
	static int RenderFramesToStableFrames(const float Frames, const float TimeDilation=1)
	{
		return FMath::RoundHalfFromZero(Frames * TimeDilation);
	}

	UFUNCTION(BlueprintPure)
	static int SecondsToStableFrames(const float Time)
	{
		return Time * GetFixedFrameRate();
	}

	UFUNCTION(BlueprintPure)
	static int AnimSecondsToAnimFrames(const float Time)
	{
		return Time * GetAnimationFPS();
	}

	// Get actual game time elapsed in the world (not real) no matter the dilation if framerate is const
	UFUNCTION(BlueprintPure)
	static float AnimFramesToSeconds(const float Frames=1.0f)
	{
		return FramesToSeconds(Frames, GetAnimationFPS());
	}
	
	UFUNCTION(BlueprintPure)
	static int AnimFramesToStableFrames(const float Frames)
	{
		return FMath::RoundHalfFromZero(Frames * GetFixedFrameRate() / GetAnimationFPS());
	}

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static float AnimFramesToRenderSeconds(const UObject* WorldContextObject, const float Frames)
	{
		return StableFramesToRenderSeconds(WorldContextObject, AnimFramesToStableFrames(Frames));
	}
	
	// Standard value for any important animation used in the game 
	static float GetAnimationFPS() {return 24;}
	static FVector RawAxisInputToUeCoords(const float X, const float Y) {return FVector(Y,X,0);}

	UFUNCTION(BlueprintCallable)
	static UAnimSequenceBase* GetAnimSequence(const FString Path, UObject* Outer);

	UFUNCTION(BlueprintCallable)
	static void GetLocationArrayFromTransformArray(const TArray<FTransform> &T, TArray<FVector> &V);
	
	UFUNCTION(BlueprintPure)
	static TArray<FVector> GetLocationsFromTransforms(const TArray<FTransform> &T);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static float GetTickTimeScaled(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static float GetTickTimeUnscaled(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static  float GetTickDebugTime(const UObject* WorldContextObject, float Mul = 1.2f);

	UFUNCTION(BlueprintPure)
	static float GetForeverTime();

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", CallableWithoutWorldContext, Keywords = "log print", AdvancedDisplay = "2", DevelopmentOnly), Category="Utilities|String")
	static void PrintStringWithinTick(UObject* WorldContextObject, const FString& InString = FString(TEXT("Hello")), bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float TimeMul = 1.2f);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", CallableWithoutWorldContext, Keywords = "log print", AdvancedDisplay = "2", DevelopmentOnly), Category="Utilities|String")
	static void PrintStringForever(UObject* WorldContextObject, const FString& InString = FString(TEXT("Hello")), bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0));

	static UStaticMesh* GetCubeMesh();
	static UStaticMesh* GetSphereMesh();

	UFUNCTION(BlueprintCallable)
	static void WriteFloatCurveToAsset(const FRuntimeFloatCurve& Curve, UCurveFloat* CurveAsset);

	template <typename T>
	static TScriptDelegate<FWeakObjectPtr> AddUniqueDelegate(T& Delegate, UObject* Obj, FName FuncName)
	{
		TScriptDelegate<FWeakObjectPtr> ScriptDelegate;
		if(!Obj || !Obj->FindFunction(FuncName)) return ScriptDelegate;
		ScriptDelegate.BindUFunction(Obj, FuncName);
		Delegate.AddUnique(ScriptDelegate);
		return ScriptDelegate;
	}
};
