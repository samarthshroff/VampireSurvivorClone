// Copyright (c) Samarth Shroff. All Rights Reserved.
// This work is protected under applicable copyright laws in perpetuity.
// Licensed under the CC BY-NC-ND 4.0 License. See LICENSE file for details.

#include "Character/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/HeroAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

FOnCharacterBeingDestroyedDelegate ACharacterBase::OnCharacterBeingDestroyed;
FOnCharacterDestroyedDelegate ACharacterBase::OnCharacterDestroyed;
// Sets default values
ACharacterBase::ACharacterBase()
{	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ACharacterBase::GetAttributeSetComponent() const
{
	return AttributeSet;
}

bool ACharacterBase::TagExactExistsInAbilityComponent(const FGameplayTag InTag) const
{
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is null. Returning False."));
		return false;	
	}

	return AbilitySystemComponent->HasMatchingGameplayTag(InTag);			
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FActiveGameplayEffectHandle ACharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& AttributesGameplayEffect, float Level) const
{
	check(AbilitySystemComponent);
	check(AttributesGameplayEffect);

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttributesGameplayEffect, Level,ContextHandle);
	return AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ACharacterBase::InitializeAttributes()
{
	DefaultAttributesEffectHandle = ApplyEffectToSelf(DefaultAttributes, 1.0f);
	
	if (BonusAttributes != nullptr)
		BonusAttributesEffectHandle = ApplyEffectToSelf(BonusAttributes,1.0f);
	
	// this needs to be the last apply always
	if (EssentialAttributes != nullptr)
		EssentialAttributesEffectHandle = ApplyEffectToSelf(EssentialAttributes, 1.0f);
}

void ACharacterBase::InitAbilityActorInfo()
{
}

void ACharacterBase::GiveAbility(const FGameplayTag& AbilityTag)
{
	if (UHeroAbilitySystemComponent* ASC = CastChecked<UHeroAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->AcquireAbility(AbilityTag);
	}
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

