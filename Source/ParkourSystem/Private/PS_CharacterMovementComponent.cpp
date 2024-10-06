// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UPS_CharacterMovementComponent::UPS_CharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}

FNetworkPredictionData_Client* UPS_CharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		UPS_CharacterMovementComponent* MutableThis = const_cast<UPS_CharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_PS(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UPS_CharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UPS_CharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVeclocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVeclocity);

	if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}

void UPS_CharacterMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void UPS_CharacterMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

void UPS_CharacterMovementComponent::CrouchPressed()
{
	bWantsToCrouch = ~bWantsToCrouch;
}

bool UPS_CharacterMovementComponent::FSavedMove_PS::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_PS* NewPSMove = static_cast<FSavedMove_PS*>(NewMove.Get());

	if (Saved_bWantsToSprint != NewPSMove->Saved_bWantsToSprint)
	{
		return false;
	}

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UPS_CharacterMovementComponent::FSavedMove_PS::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

uint8 UPS_CharacterMovementComponent::FSavedMove_PS::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToSprint) Result |= FLAG_Custom_0;

	return Result;
}

void UPS_CharacterMovementComponent::FSavedMove_PS::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UPS_CharacterMovementComponent* CharacterMovement = Cast<UPS_CharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

void UPS_CharacterMovementComponent::FSavedMove_PS::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	UPS_CharacterMovementComponent* CharacterMovement = Cast<UPS_CharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

UPS_CharacterMovementComponent::FNetworkPredictionData_Client_PS::FNetworkPredictionData_Client_PS(const UCharacterMovementComponent& ClientMovement)
: Super(ClientMovement)
{

}

FSavedMovePtr UPS_CharacterMovementComponent::FNetworkPredictionData_Client_PS::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_PS);
}
