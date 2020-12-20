// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBlackHole.generated.h"

class USphereComponent;

UCLASS()
class FPSGAME_API AFPSBlackHole : public AActor
{
	GENERATED_BODY()
	
/*
* Members *
*/

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "InfluenceSphere")
	USphereComponent* InfluenceSphere;

	UPROPERTY(EditDefaultsOnly, Category = "CenterSphere")
	USphereComponent* CenterSphere;

	UPROPERTY(EditAnywhere)
	float AttractionForce = 1500.0f;

/*
* Methods *
*/

private:

	UFUNCTION()
	void OnCenterSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void AttractActorsInInfluenceSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AFPSBlackHole();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
