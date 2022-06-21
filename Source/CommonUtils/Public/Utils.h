#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "EngineUtils.h"
#include "Engine/DataTable.h"
#include "debug.h"


namespace CommonUtils
{
    template <typename V>
    static void RemoveArrayItemsFromEdge(TArray<V> &Array, const int NumToRemove, const bool IsFromStart=true)
    {
        if (NumToRemove < 1 || Array.Num() == 0)
            return;
        
        if (NumToRemove >= Array.Num())
        {
            Array.Empty();
            return;
        }
        
        if (IsFromStart)
        {
            Array.RemoveAt(0,NumToRemove);
        }
        else
        {
            int Index = Array.Num() - NumToRemove - 1;
            Array.RemoveAt(Index,NumToRemove);
        }
    }

    template<typename T>
    static void AddToArrayAsInverted(TArray<T> &Destination, const TArray<T> &ArrayToAdd)
    {
        const int Num = ArrayToAdd.Num() - 1;
        for (int i = Num; i >= 0; --i)
        {
            Destination.Add(ArrayToAdd[i]);
        }
    }

    template<typename T>
    bool AreEqual(T Var1, T Var2)
    {
        return (Var1 == Var2);
    }
    
    template <typename K,typename V>
    TMap<V, K> SwapTMap(TMap<K, V> InMap)
    {
        TMap<V, K> OutMap;
        for(auto& Item:InMap)
        {
            OutMap.Add(Item.Value,Item.Key);
        }
        return OutMap;
    }
    
    template<typename K, typename V>
    static TArray<V> GetTMapValues(TMap<K, V> InputMap)
    {
        TArray<V> OutArray;
        for(auto& Elem: InputMap)
            OutArray.Add(Elem.Value);

        return OutArray;
    }

    template<typename T>
    void FindAllActors(UWorld* World, TArray<T*>& Out)
    {
        for (TActorIterator<T> It(World); It; ++It)
        {
            Out.Add(*It);
        }
    }

    template<typename T>
    static void ReadDataTableRows(UDataTable* Table, FString CallerName, TArray<T*> &OutRows)
    {
        if (!IsValid(Table))
        {
            PrintToLog(CallerName + ": Table is not valid");
            return;
        }

        Table->GetAllRows("",OutRows);

        if (OutRows.Num() == 0)
        {
            PrintToLog(CallerName + ": Empty or Invalid Table");
        }
    }

    template<typename T, typename K>
    static bool GetValueFromMap(TMap<int, T> InMap, const K Key, T & OutV)
    {
        const auto V = InMap.Find(Key);
        if(V != nullptr)
        {
            OutV = *V;
            return true;
        }
        return false;
    }

    template<typename T>
    static T SelectClosestValue(T A, T B, T Ref)
    {
        auto DifA = FGenericPlatformMath::Abs(A - Ref);
        auto DifB = FGenericPlatformMath::Abs(B - Ref);
        return DifA < DifB ? A : B;
    }

    static float SelFloatFromNeighborsAbs(float Min, float Max, float Ref)
    {
        Min = FGenericPlatformMath::Abs(Min);
        Max = FGenericPlatformMath::Abs(Max);
        Ref = FGenericPlatformMath::Abs(Ref);
        
        if(Ref <= Min)
        {
            return Min;
        }
        if(Ref >= Max)
        {
            return Max;
        }

        const float Mid = 0.5f * (Max - Min);
        return Ref <= Mid ? Min : Max;
    }

    template<typename E>
    FString EnumAsString(E Value)
    {
        return *UEnum::GetValueAsString(Value);
    }

    template<typename T>
    static bool ArrayGetLargestValueLessOrEqualRef(const TArray<T> Array, T Ref, T &FoundValue, int &FoundIndex)
    {
        if(Array.Num() > 0)
        {
            for (auto Element : Array)
            {
                if(Element <= Ref)
                {
                    FoundValue = Element;
                }
                else
                    break;
            }
            FoundIndex = Array.IndexOfByKey(FoundValue);
            return true;
        }
        return false;
    }

    template<typename T>
    static bool ArrayGetSmallestValueBiggerThanRef(const TArray<T> Array, T Ref, T &FoundValue, int &FoundIndex)
    {
        if(Array.Num() > 0)
        {
            for (auto Element : Array)
            {
                if(Element > Ref)
                {
                    FoundValue = Element;
                    FoundIndex = Array.IndexOfByKey(FoundValue);
                    return true;
                }
            }
        }
        return false;
    }
    
    template<typename T>
    static bool ArrayGetNeighborsOfValue(const TArray<T> Array, T Ref, T &FoundMin, T &FoundMax)
    {
        bool MinFound = false;
        
        if(Array.Num() > 0)
        {
            for (auto Element : Array)
            {
                if(Element <= Ref)
                {
                    FoundMin = Element;
                    MinFound = true;
                }
                if(Element > Ref && MinFound)
                {
                    FoundMax = Element;
                    return true;
                }
            }
        }
        return false; 
    }

    template<typename  R>
    R* GetAnimationLayer(UAnimInstance* AnimInstance, TSubclassOf<R> Klass)
    {
        if(AnimInstance)
        {
            return Cast<R>(AnimInstance->GetLinkedAnimLayerInstanceByClass(Klass));
        }
        return nullptr;
    }

    template<typename A>
    A* GetLinkedAnimGraphInstanceByTag(UAnimInstance* Owner, FName Tag)
    {
        return Cast<A>(Owner->GetLinkedAnimGraphInstanceByTag(Tag));
    }

    template<typename A>
    A*  GetFirstLinkedAnimGraphInstanceByTag(UAnimInstance* Owner, FName Tag = "None")
    {
        TArray<UAnimInstance*> Arr;
        Owner->GetLinkedAnimGraphInstancesByTag(Tag, Arr);
        for (auto Element : Arr)
        {
            A* T = Cast<A>(Element);
            if (T)
            {
                return T;
            }
        }
        return nullptr;
    }
    
    template<typename T>
    static void ClearMapByValidIndices(const TArray<int> ValidIndices, TMap<int,T> InMap, TMap<int,T> &OutMap)
    {
        TArray<int> Keys;
        InMap.GetKeys(Keys);
        
        for (int Key : Keys)
        {
            if (ValidIndices.Contains(Key))
                OutMap.Add(Key,*InMap.Find(Key));
        }
    }

    static  bool IsObjectInPlayableWorld(const UObject* Obj)
    {
        if(!Obj) return false;
        auto World = Obj->GetWorld();
        if(!World)
        {
            const auto Outer = Obj->GetOuter();
            if(Outer) World = Outer->GetWorld();
        }
        if(!World) return  false;
        
        const auto WT = Obj->GetWorld()->WorldType;
        return WT == EWorldType::Game || WT ==EWorldType::PIE;
    }

    template<typename T>
    static void ShuffleArray(TArray<T> &A)
    {
        if (A.Num() == 0) return;
		
        TArray<T> TmpArray;

        while (true)
        {
            const int Num = A.Num();
            if (Num == 0) break;

            int Index = FMath::RandRange(0, Num - 1);
            auto Elem = A[Index];
            TmpArray.Add(Elem);
            A.RemoveAt(Index);
        }
        A = TmpArray;		
    }

    template<typename T>
    static TArray<T*> ArrayToPointerArray(const TArray<T>& A)
    {
        TArray<T*> Out;
        for (auto Item : A)
        {
            Out.Add(&Item);
        }
        return Out;
    }
    
    template<typename T>
    static TMap<int, T*> IntMapToPointerMap(TMap<int, T> &M)
    {
        TMap<int, T*> Out;
        for (auto Item : M)
        {
            Out.Add(Item.Key, &Item.Value);
        }
        return Out;
    }

    template<typename T>
    static TArray<T> CopyArray(const TArray<T> &InT)
    {
        TArray<T> Out;
        Out.Append(InT);
        return Out;
    }
    
    
}
