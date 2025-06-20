// Copyright (c) Samarth Shroff. All Rights Reserved.
// This work is protected under applicable copyright laws in perpetuity.
// Licensed under the CC BY-NC-ND 4.0 License. See LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_HomingProjectile.generated.h"

class UWeaponManager;
class USphereComponent;
class UWeaponAttributeSet;
class AProjectileHoming;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHomingProjectileCompleteDelegate, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHomingProjectileImpactDelegate, AActor *, TargetActor, FVector, ImpactLocation);

/**
 * Ability Task for all the Weapons from the Projectile Homing category.
 * This Task is targeted to spawn projectiles and find the closest target actor for them.
 */
UCLASS()
class SENTINELSSCOURGE_API UAbilityTask_HomingProjectile : public UAbilityTask
{
	GENERATED_BODY()

private:
	// The radius of the targeting sphere. Initially it is max of viewport width or height.
	float DefaultTargetingRadius;

	// Number of projectiles to spawn in one activation. Value comes from this weapon attribute set.
	int ProjectilesToSpawn;
	int ProjectileCount;
	FTimerHandle ProjectileSpawnTimerHandle;

	UPROPERTY()
	TSubclassOf<AProjectileHoming> ProjectileClass;

	UPROPERTY()
	TObjectPtr<UWeaponAttributeSet> WeaponAttributeSet;

	// Actors that overlap the Targeting Sphere.
	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitTargets;

	// The Sphere used to detect the overlapping Actors that are then used to find the closest Target Actor.
	// This sphere's radius changes based on the number of Overlapped Actors in order to maintain performance.
	UPROPERTY()
	TObjectPtr<USphereComponent> TargetingSphere;

	// The Ability's weapon tag used to get the radius multiplier from a curve table.
	UPROPERTY()
	FGameplayTag WeaponTag;

	// Has the Radius multiplier based on number of Target Actors.
	UPROPERTY()
	TObjectPtr<UCurveTable> TargetingRadiusCurveTable;

	UPROPERTY()
	TObjectPtr<UWeaponManager> WeaponManager;

public:
	// Delegates for blueprint callbacks
	UPROPERTY(BlueprintAssignable)
	FHomingProjectileCompleteDelegate OnComplete;

	UPROPERTY(BlueprintAssignable)
	FHomingProjectileImpactDelegate OnImpact;

private:
	// Updates the Targeting Sphere radius based on the total hit targets found.
	void UpdateTargetingSphereRadius() const;
	void SpawnProjectile_Internal();

protected:
	virtual void TickTask(float DeltaTime) override;
	const AActor *FindClosestTarget() const;

	UFUNCTION()
	void OnCharacterBeingDestroyed(const AActor *Actor);

	UFUNCTION()
	void OnTargetingSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								  const FHitResult &SweepResult);

	void Initialize_Internal(const UGameplayAbility *OwningAbility, const FGameplayTag &InWeaponTag, const TSubclassOf<AProjectileHoming> &InProjectileClass, UWeaponManager *InWeaponManager);

public:
	explicit UAbilityTask_HomingProjectile(const FObjectInitializer &ObjectInitializer);

	UFUNCTION(Blueprintable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_HomingProjectile *CreateHomingProjectile(UGameplayAbility *OwningAbility, FName TaskInstanceName, const FGameplayTag &InWeaponTag,
																 const TSubclassOf<AProjectileHoming> &InProjectileClass, UWeaponManager *InWeaponManager);

	void SpawnProjectile();

	virtual void OnDestroy(bool bInOwnerFinished) override;
};
