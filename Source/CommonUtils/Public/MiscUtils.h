// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MiscUtils.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class COMMONUTILS_API UMiscUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    
#if WITH_EDITOR
    
    UFUNCTION(BlueprintCallable)
    static void ExportDataTableToJson(UDataTable* DataTable, FString TargetDirectory);

#endif

};
