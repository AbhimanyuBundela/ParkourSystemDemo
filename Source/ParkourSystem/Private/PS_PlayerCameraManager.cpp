// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_PlayerCameraManager.h"

#include "ParkourSystemCharacter.h"
#include "PS_CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

APS_PlayerCameraManager::APS_PlayerCameraManager()
{
}

void APS_PlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (AParkourSystemCharacter* PS_Character = Cast<AParkourSystemCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UPS_CharacterMovementComponent* PS_CMC = PS_Character->GetPS_CharacterMovementComponent();
		FVector TargetCrouchOffset = FVector(0, 0, PS_CMC->GetCrouchedHalfHeight() - PS_Character->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (PS_CMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		if (PS_CMC->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}
	}
}
