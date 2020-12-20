// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
/*
* Members *
*/
protected:
	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
	UStaticMeshComponent* LaunchPadBase;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
	UStaticMeshComponent* LaunchPadIcon;

	UPROPERTY(EditDefaultsOnly, Category = "TriggerBox")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = "LaunchSettings")
	USceneComponent* LaunchDirection;

	UPROPERTY(EditAnywhere, Category = "LaunchSettings")
	float LaunchSpeed;

	UPROPERTY(EditAnywhere, Category = "LaunchFX")
	UParticleSystem* LaunchFX;

	UPROPERTY(EditAnywhere, Category = "LaunchSX")
	USoundBase* LaunchSX;

/*
* Methods *
*/
private:
	void PlayLaunchEffects();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

};
