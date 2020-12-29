// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

// Engine Includes
#include "Engine/World.h"

// Game Includes
#include "FPSPlayerController.h"



void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());

		// Hosting Player will have PlayerController for local client and
		// Server version, UMG is only on client to make sure PlayerController
		// is the one that is locally controlled
		if (PC != nullptr && PC->IsLocalController())
		{
			PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);

			// Disable Input
			APawn* MyPawn = PC->GetPawn();
			if (MyPawn != nullptr)
			{
				MyPawn->DisableInput(PC);
			}
		}
	}
}

void AFPSGameState::MulticastOnObjectivePickedUp_Implementation(APawn* ObjectiveHolder)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());
		if (PC != nullptr && PC->IsLocalController())
		{
			UE_LOG(LogTemp, Warning, TEXT("MulticastOnObjectivePickedUp_Implementation"));
			PC->OnObjectivePickup(true);
		}
	}
}
