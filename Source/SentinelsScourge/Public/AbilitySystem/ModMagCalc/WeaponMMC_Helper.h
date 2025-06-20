﻿// Copyright (c) Samarth Shroff. All Rights Reserved.
// This work is protected under applicable copyright laws in perpetuity.
// Licensed under the CC BY-NC-ND 4.0 License. See LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "WeaponMMC_Helper.generated.h"

class UHeroAbilitySystemComponent;
class IAbilitySystemInterface;
class UPlayerAttributeSet;
class UWeaponAttributeSet;

/**
 * Helper class for the Weapon attribute MMCs
 */
UCLASS()
class SENTINELSSCOURGE_API UWeaponMMC_Helper : public UObject
{
	GENERATED_BODY()
private:
	// statics may not work if the game is made multiplayer, assuming that
	// this will run on server. If run on client then it should work.
	static UHeroAbilitySystemComponent *AbilitySystemComponent;
	static const UPlayerAttributeSet *PlayerAttributeSet;

public:
	static const UWeaponAttributeSet *GetWeaponAttributeSet(const FGameplayEffectSpec &Spec);
	static const UPlayerAttributeSet *GetPlayerAttributeSet(const FGameplayEffectSpec &Spec);

private:
	static void InitializeAbilitySystemComponent(const FGameplayEffectSpec &Spec);
};
