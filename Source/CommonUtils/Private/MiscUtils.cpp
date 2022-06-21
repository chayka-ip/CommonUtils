// Fill out your copyright notice in the Description page of Project Settings.

#include "MiscUtils.h"
#include "EditorFramework/AssetImportData.h"
#include "Engine/DataTable.h"

#if WITH_EDITOR

void UMiscUtils::ExportDataTableToJson(UDataTable* DataTable, FString TargetDirectory)
{
    if (DataTable)
    {
        // const FString CurrentFilename = DataTable->AssetImportData->GetFirstFilename();
        const FString Name = DataTable->GetName();
        const FString DefaultPath = TargetDirectory + "/" + Name + ".json";
        FFileHelper::SaveStringToFile(DataTable->GetTableAsJSON(EDataTableExportFlags::UseJsonObjectsForStructs), *DefaultPath);
    }
}

#endif
