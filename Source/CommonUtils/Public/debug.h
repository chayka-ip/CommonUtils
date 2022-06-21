#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"

static void PrintOnScreen(FString Msg)
{
// #if WITH_EDITOR
    if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, *Msg);}     
// #endif
}

static void PrintToLog(const FString Msg)
{
// #if WITH_EDITOR  
    UE_LOG(LogTemp,Warning,TEXT("%s"), *Msg)
// #endif
}