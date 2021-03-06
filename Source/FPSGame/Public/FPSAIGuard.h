// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"


class UPawnSensingComponent;
class AAIController;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAIS_Idle		UMETA(DisplayName = "Idle"),
	EAIS_Suspicious	UMETA(DisplayName = "Suspicious"),
	EAIS_Alerted	UMETA(DisplayName = "Alerted"),

	EAIS_MAX		UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

/*
* Members *
*/

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	FRotator OriginalRotation;

	FTimerHandle ResetOrientationTimerHandle;

	// ReplicatedUsing function will only run on clients
	// when the GuardStart changes
	// the AI will only run on the server, which is setting the AIguardState
	// for the clients to know about the update use ReplicatedUsing
	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState GuardState;

	void SetGuardState(EAIState NewState);

	UPROPERTY(EditInstanceOnly, Category = "Patrol")
	bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "Patrol", meta = (EditCondition = "bPatrol"))
	TArray<AActor*> NavPoints;

	int32 CurrentPatrolPoint;

/*
* Methods *
*/
public:


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPawnSensingCompSeePawn(APawn* SeenPawn);

	UFUNCTION()
	void OnPawnSensingCompHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume);

	UFUNCTION()
	void ResetOrientation();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

	UFUNCTION()
	void OnRep_GuardState();

public:	
	// Sets default values for this character's properties
	AFPSAIGuard();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetNextPatrolPoint();

	void SetCurrentMovement();

	void StopCurrentMovment();
};
