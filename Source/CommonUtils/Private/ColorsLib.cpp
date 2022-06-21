// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorsLib.h"
static constexpr int MaxUint = 255;

TArray<FLinearColor> UColorsLib::MakeColorsArray(int Num)
{
    TArray<FLinearColor> OutArray;

    if(Num > 0)
    {
        const float DeltaColor = MaxUint / Num;

        for (int i = 0; i < Num; ++i)
        {
            const int H = DeltaColor * i;
            constexpr int S = MaxUint;
            constexpr int V = MaxUint;
            FLinearColor Color = FLinearColor::MakeFromHSV8(H, S, V);
            OutArray.Add(Color);
        }
    }
    
    return OutArray;
}

FLinearColor UColorsLib::MakeColorFromAlphaNormalised(float Alpha)
{
    const float H = FMath::Clamp(Alpha, 0.0f, 1.0f)  * MaxUint;
    constexpr int S = MaxUint;
    constexpr int V = MaxUint;
    return  FLinearColor::MakeFromHSV8(H, S, V);
}
