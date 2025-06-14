﻿// Copyright (c) Samarth Shroff. All Rights Reserved.
// This work is protected under applicable copyright laws in perpetuity.
// Licensed under the CC BY-NC-ND 4.0 License. See LICENSE file for details.

#include "Weapon/Homing/ProjectileHoming.h"

#include "AbilitySystemGlobals.h"
#include "SentinelsScourgeGameMode.h"
#include "SentinelsScourgeGameplayTags.h"
#include "AbilitySystem/EnemyHitGEContext.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileHoming::AProjectileHoming()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_ProjectileChannel);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Sphere->SetCollisionResponseToChannel(ECC_EnemyChannel, ECR_Overlap);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileHoming::OnSphereOverlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	ProjectileMovement->InitialSpeed = 550.0f;
	ProjectileMovement->MaxSpeed = 550.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AProjectileHoming::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	// Ignore the Hero Actor and all it's components from collision.
	Sphere->IgnoreActorWhenMoving(GetOwner(), true);
}

void AProjectileHoming::Initialize(const bool InbBlockedByWalls, const float InSpeed, const float InPierce, const float InDamage, const float InArea, const float InLevel,
								   const float InKnockback, const TObjectPtr<const AActor> &InHomingTargetActor, const TObjectPtr<AActor> &InAvatarActor)
{
	bIsBlockedByWalls = InbBlockedByWalls;
	Speed = InSpeed;
	Pierce = InPierce;
	Damage = InDamage;
	Area = InArea;
	Level = InLevel;
	Knockback = InKnockback;
	PierceCount = 0;
	HomingTargetActor = InHomingTargetActor;
	AvatarActor = InAvatarActor;

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	// UE_LOG(LogTemp, Log, TEXT("Projectile Fired!!!!"));
}

void AProjectileHoming::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
										UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// UE_LOG(LogTemp, Log, TEXT("OnSphereHit %hhd"), OtherComp->GetCollisionObjectType());

	if (OtherComp->GetCollisionObjectType() == ECC_WorldStatic && bIsBlockedByWalls)
	{
		Destroy();
		return;
	}
	if (OtherActor != nullptr && OtherActor == HomingTargetActor)
	{
		SetByCallerValues.Reset();
		SetByCallerValues.Add(SentinelsScourgeGameplayTags::Effect_Modifier_Damage, -1.0f * Damage);

		OnBeginOverlap(OtherActor, Level);
		Sphere->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		Destroy();
	}

	// PierceCount++;
	// if (PierceCount > Pierce)
	// {
	// 	Destroy();
	// }

	// if (OtherActor != nullptr && OtherActor == HomingTargetActor)
	// {
	// 	TArray<AActor*> IgnoreActors;
	// 	IgnoreActors.Add(this);
	// 	IgnoreActors.Add(AvatarActor.Get());
	//
	// 	//using ApplyRadialDamageWithFalloff in case I plan on implementing the Limit-Break feature in the future then
	// 	// in that case the area param varies, for example for magic wand the area increases by 2.5% for each upgrade after limit break till max of 1000%
	// 	UGameplayStatics::ApplyRadialDamage(GetWorld(),Damage, SweepResult.Location,
	// 		Area, UDamageType::StaticClass(), IgnoreActors,AvatarActor, GetOwner()->GetInstigatorController(), true);
	// 	Destroy();
	// }
}

void AProjectileHoming::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HomingTargetActor != nullptr)
	{
		const FVector TargetLocation = HomingTargetActor->GetActorLocation();
		const FVector CurrentLocation = GetActorLocation();
		const FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

		if (bIsBlockedByWalls)
		{
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			if (AvatarActor != nullptr)
				Params.AddIgnoredActor(AvatarActor);

			if (GetWorld()->LineTraceSingleByChannel(Hit, CurrentLocation, TargetLocation, ECC_Visibility, Params))
			{
				if (Hit.GetActor() != HomingTargetActor)
				{
					return;
				}
			}
		}

		float HomingStrength = Speed * 0.5f;
		const FVector CurrentVelocity = ProjectileMovement->Velocity;
		const FVector TargetVelocity = Direction * CurrentVelocity.Size();
		// UE_LOG(LogTemp, Log, TEXT("The CurrentVelocity is %s and Target is %s"), *CurrentVelocity.ToString(), *TargetVelocity.ToString());
		ProjectileMovement->Velocity = FMath::VInterpTo(CurrentVelocity, TargetVelocity, DeltaSeconds, HomingStrength);
		// Rotate towards enemy
		const FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), Direction.Rotation(), DeltaSeconds, HomingStrength);
		SetActorRotation(SmoothRotation);
	}
}

FGameplayEffectContextHandle AProjectileHoming::MakeEffectContext(const UAbilitySystemComponent *AbilitySystemComponent)
{
	FEnemyHitGEContext *Context = new FEnemyHitGEContext();
	Context->SetKnockback(Knockback);
	Context->AddInstigator(AvatarActor, AvatarActor);
	return FGameplayEffectContextHandle(Context);
}
