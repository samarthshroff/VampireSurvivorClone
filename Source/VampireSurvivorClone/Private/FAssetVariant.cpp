﻿#include "VampireSurvivorClone/FAssetVariant.h"

inline void FAssetVariant::SetNiagaraSystemClass(TSoftObjectPtr<UNiagaraSystem> InNiagaraSystemClass)
{
	NiagaraSystemClass = MoveTemp(InNiagaraSystemClass);
	StaticMesh = nullptr;
	Variant = EAssetVariant::NiagaraSystem;
}

inline void FAssetVariant::SetStaticMesh(TSoftObjectPtr<UStaticMesh> InStaticMesh)
{
	NiagaraSystemClass = nullptr;
	StaticMesh = MoveTemp(InStaticMesh);
	Variant = EAssetVariant::StaticMesh;
}

inline TSoftObjectPtr<UObject> FAssetVariant::GetAsset() const
{
	switch (Variant)
	{
	case EAssetVariant::NiagaraSystem:
		return NiagaraSystemClass;
	case EAssetVariant::StaticMesh:
		return StaticMesh;
	default:
		return nullptr;
	}

}
