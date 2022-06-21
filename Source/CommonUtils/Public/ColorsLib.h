// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ColorsLib.generated.h"

/**
 * TODO: additional colors
 */
UCLASS()
class COMMONUTILS_API UColorsLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static TArray<FLinearColor> MakeColorsArray(int Num);

	UFUNCTION(BlueprintPure)
	static FLinearColor MakeColorFromAlphaNormalised(float Alpha);
};
