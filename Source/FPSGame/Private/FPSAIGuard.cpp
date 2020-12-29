// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"

// Engine Includes
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Game Includes
#include "FPSGameMode.h"
#include "FPSGuardAIController.h"


// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	GuardState = EAIState::EAIS_Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp != nullptr)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSensingCompSeePawn);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnPawnSensingCompHearNoise);
	}

	OriginalRotation = GetActorRotation();

	CurrentPatrolPoint = 0;

	if (bPatrol)
	{
		SetCurrentMovement();
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSAIGuard, GuardState);
}

void AFPSAIGuard::SetNextPatrolPoint()
{
	if (bPatrol == false || NavPoints.Num() == 0) return;

	++CurrentPatrolPoint;
	if (CurrentPatrolPoint >= NavPoints.Num())
	{
		CurrentPatrolPoint = 0;
	}
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState) return;

	GuardState = NewState;

	// OnRep function not called by default on the server.
	// for cases when playing as a listen server ie server and client
	// then we call On_Rep function to make sure the servers client is on
	// the same page as the other clients
	OnRep_GuardState();
}

void AFPSAIGuard::StopCurrentMovment()
{
	if (bPatrol == false) return;

	auto Controller = Cast<AFPSGuardAIController>(GetController());
	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAIGuard::SetCurrentMovement()
{
	if (bPatrol == false) return;

	auto Controller = Cast<AFPSGuardAIController>(GetController());
	if (Controller != nullptr && NavPoints[CurrentPatrolPoint] != nullptr && NavPoints.Num() > 0)
	{
		Controller->MoveToLocation(NavPoints[CurrentPatrolPoint]->GetActorLocation(), 1.0f);
	}
}

void AFPSAIGuard::OnPawnSensingCompSeePawn(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) return;

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10);

	// If player is seen then game over
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM != nullptr)
	{
		GM->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EAIState::EAIS_Alerted);
}

void AFPSAIGuard::OnPawnSensingCompHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::EAIS_Alerted) return;
	if (InstigatorPawn == nullptr) return;

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10);

	// Get the direction the noise was heard from the Actor
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	// Get look at rotator for direction, only use Yaw
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	if (bPatrol)
	{
		StopCurrentMovment();
	}

	SetActorRotation(NewLookAt);
	
	// Set timer to look back at original rotation, it actor is HearNoise is trigger before timer runs
	// then clear and restart
	GetWorldTimerManager().ClearTimer(ResetOrientationTimerHandle);
	GetWorldTimerManager().SetTimer(ResetOrientationTimerHandle, this, &AFPSAIGuard::ResetOrientation, 3.0f);

	SetGuardState(EAIState::EAIS_Suspicious);
}

void AFPSAIGuard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
	if (GuardState == EAIState::EAIS_Alerted) return;
	SetGuardState(EAIState::EAIS_Idle);

	if (bPatrol)
	{
		SetCurrentMovement();
	}
}



