// Copyright (c) Samarth Shroff. All Rights Reserved.
// This work is protected under applicable copyright laws in perpetuity.
// Licensed under the CC BY-NC-ND 4.0 License. See LICENSE file for details.

#include "UI/WidgetComponent/PlayerHealthBarWidgetComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widget/SSWidget.h"
#include "UI/WidgetController/PlayerHealthWidgetController.h"
#include "UI/WidgetController/SSCWidgetController.h"

UPlayerHealthBarWidgetComponent::UPlayerHealthBarWidgetComponent(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerHealthBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	// UE_LOG(LogTemp, Log, TEXT("HealthBar UPlayerHealthBarWidgetComponent::BeginPlay"));
	PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

void UPlayerHealthBarWidgetComponent::OnRegister()
{
	Super::OnRegister();
	// UE_LOG(LogTemp, Log, TEXT("HealthBar UPlayerHealthBarWidgetComponent::OnRegister"));

	// As per the WidgetComponent::OnRegister, InitWidget is called only if it is the editor and the world is not a
	// Game World (GetWorld()->IsGameWorld() == false) but this is true when I run it in editor.
	// So to be on a safer side I am adding a check that if the widget is null (be it editor or actual gameplay)
	// then call InitWidget
	if (GetWidget() == nullptr)
	{
		InitWidget();
	}
}

TObjectPtr<UPlayerHealthWidgetController> UPlayerHealthBarWidgetComponent::GetWidgetController(const FWidgetControllerParams &WidgetControllerParams)
{
	if (PlayerHealthWidgetController == nullptr)
	{
		PlayerHealthWidgetController = NewObject<UPlayerHealthWidgetController>(this, UPlayerHealthWidgetController::StaticClass());
		PlayerHealthWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		PlayerHealthWidgetController->BindCallbacksToDependencies();
	}

	return PlayerHealthWidgetController;
}

void UPlayerHealthBarWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
													FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerCameraManager)
	{
		const FRotator Angle = UKismetMathLibrary::FindLookAtRotation(this->GetComponentLocation(),
																	  PlayerCameraManager->GetCameraLocation());
		this->SetWorldRotation(Angle);
	}
}

void UPlayerHealthBarWidgetComponent::Initialize(const FWidgetControllerParams &WidgetControllerParams)
{
	// UE_LOG(LogTemp, Log, TEXT("HealthBar UPlayerHealthBarWidgetComponent::Initialize"));
	checkf(WidgetClass, TEXT("UPlayerHealthBarWidgetComponent::Initialize: Widget class is null. Please fill it in Associate Blueprint."));
	USSWidget *SSCWidget = Cast<USSWidget>(GetWidget());

	UPlayerHealthWidgetController *WidgetController = GetWidgetController(WidgetControllerParams);
	SSCWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
}
