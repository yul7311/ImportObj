#pragma once

#include "Tickable.h"
#include "importObj.h"

class ImportObjImpl;

class Fm2uTickObject : FTickableGameObject
{

public:
Fm2uTickObject(ImportObjImpl*_Owner )
	: Owner(_Owner)
	{
	}

	virtual void Tick(float DeltaTime)
	{
		check( Owner != NULL );
		Owner->Tick(DeltaTime);
	}

	virtual bool IsTickable() const
	{
		return true;
	}

	virtual bool IsTickableWhenPaused() const
	{
		return true;
	}

	virtual bool IsTickableInEditor() const
	{
		return true;
	}

	virtual TStatId GetStatId() const 
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ImportObj, STATGROUP_Tickables);
	}

private:
	ImportObjImpl* Owner;
};
