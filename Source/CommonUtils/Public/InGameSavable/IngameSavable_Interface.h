// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IngameSavable_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIngameSavable_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONUTILS_API IIngameSavable_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	void Save();
	bool MarkPackageAsDirty();
	virtual UObject* GetSelf();
};
