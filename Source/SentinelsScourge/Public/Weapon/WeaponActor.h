// Copyright (c) Samarth Shroff. All Rights Reserved.
// This work is protected under applicable copyright laws in perpetuity.
// Licensed under the CC BY-NC-ND 4.0 License. See LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UCLASS()
class SENTINELSSCOURGE_API AWeaponActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Graphics, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *StaticMesh;

public:
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	void SetStaticMesh(TObjectPtr<UStaticMesh> Mesh) const;
};
