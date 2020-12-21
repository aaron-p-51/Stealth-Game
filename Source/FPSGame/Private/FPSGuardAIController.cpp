// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGuardAIController.h"

#include "FPSAIGuard.h"

void AFPSGuardAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	// Only advance to next waypoint if success
	if (Result.IsSuccess())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnMoveComplete()"));

		auto GuardCharacter = Cast<AFPSAIGuard>(GetCharacter());
		if (GuardCharacter != nullptr)
		{
			GuardCharacter->SetNextPatrolPoint();
			GuardCharacter->SetCurrentMovement();
		}
	}
}