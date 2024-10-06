// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "PS_PlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURSYSTEM_API APS_PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) float CrouchBlendDuration = 0.5f;
	float CrouchBlendTime;
	
public:
	APS_PlayerCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
