// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameSavable/IngameSavable_Interface.h"

void IIngameSavable_Interface::Save()
{
    bool bSaved = MarkPackageAsDirty();
}

bool IIngameSavable_Interface::MarkPackageAsDirty()
{
    /*
    * COPYPASTED FROM SOURCE WITH SMALL CHANGES
    */

    auto Obj = GetSelf();
    
    // since transient objects will never be saved into a package, there is no need to mark a package dirty
    // if we're transient
    if ( !Obj->HasAnyFlags(RF_Transient) )
    {
        UPackage* Package = Obj->GetOutermost();

        if( Package != NULL	)
        {
            // It is against policy to dirty a map or package during load in the Editor, to enforce this policy
            // we explicitly disable the ability to dirty a package or map during load.  Commandlets can still
            // set the dirty state on load.
            if( IsRunningCommandlet() || 
                (GIsEditor && !GIsEditorLoadingPackage && !GIsCookerLoadingPackage /*&& !GIsPlayInEditorWorld*/ && !IsInAsyncLoadingThread()
#if WITH_HOT_RELOAD
                    && !GIsHotReload
#endif // WITH_HOT_RELOAD
#if WITH_EDITORONLY_DATA
                    && !Package->bIsCookedForEditor // Cooked packages can't be modified nor marked as dirty
#endif
                ))
            {
                const bool bIsDirty = Package->IsDirty();

                // We prevent needless re-dirtying as this can be an expensive operation.
                if( !bIsDirty )
                {
                    Package->SetDirtyFlag(true);
                }

                // Always call PackageMarkedDirtyEvent, even when the package is already dirty
                Package->PackageMarkedDirtyEvent.Broadcast(Package, bIsDirty);

                return true;
            }
            else
            {
                // notify the caller that the request to mark the package as dirty was suppressed
                return false;
            }
        }
    }
    return true;
}

UObject* IIngameSavable_Interface::GetSelf()
{
    check(false)
    return nullptr;
}
