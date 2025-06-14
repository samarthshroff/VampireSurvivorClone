// Copyright (c) Samarth Shroff. All Rights Reserved.
// This work is protected under applicable copyright laws in perpetuity.
// Licensed under the CC BY-NC-ND 4.0 License. See LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SSCWidgetController.generated.h"

class UPlayerAttributeSet;
class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(Blueprintable)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY()
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

	FWidgetControllerParams() {}

	FWidgetControllerParams(TObjectPtr<APlayerController> InPlayerController,
							TObjectPtr<APlayerState> InPlayerState, TObjectPtr<UAbilitySystemComponent> InAbilitySystemComponent, TObjectPtr<UAttributeSet> InAttributeSet) : PlayerController(InPlayerController), PlayerState(InPlayerState), AbilitySystemComponent(InAbilitySystemComponent), AttributeSet(InAttributeSet)
	{
	}
};

/**
 *
 */
UCLASS()
class SENTINELSSCOURGE_API USSCWidgetController : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<UPlayerAttributeSet> PlayerAttributeSet;

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams &InWidgetControllerParams);

	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	virtual void Initialize();

protected:
	TObjectPtr<UPlayerAttributeSet> GetPlayerAttributeSet();
};
