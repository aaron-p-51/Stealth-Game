// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"

// Engine Includes
#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CenterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CenterSphere"));
	if (CenterSphere != nullptr)
	{
		SetRootComponent(CenterSphere);
		CenterSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CenterSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CenterSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECR_Overlap);
	}

	InfluenceSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InfluenceSphere"));
	if (InfluenceSphere != nullptr)
	{
		InfluenceSphere->SetupAttachment(GetRootComponent());
		InfluenceSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InfluenceSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		InfluenceSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECR_Overlap);
	}

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	if (MeshComp != nullptr)
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComp->SetupAttachment(GetRootComponent());
	}
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();

	if (CenterSphere != nullptr)
	{
		CenterSphere->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OnCenterSphereBeginOverlap);
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttractActorsInInfluenceSphere();
}

void AFPSBlackHole::AttractActorsInInfluenceSphere()
{
	TArray<UPrimitiveComponent*> InfluenceSphereOverlapComponents;

	if (InfluenceSphere != nullptr)
	{
		InfluenceSphere->GetOverlappingComponents(InfluenceSphereOverlapComponents);
	}

	for (auto Comp : InfluenceSphereOverlapComponents)
	{
		if (Comp != nullptr)
		{
			if (Comp->IsSimulatingPhysics() == false) continue;
			Comp->AddRadialForce(GetActorLocation(), InfluenceSphere->GetScaledSphereRadius(), -AttractionForce, ERadialImpulseFalloff::RIF_Linear, true);

		}
	}
}

void AFPSBlackHole::OnCenterSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		OtherActor->Destroy();
	}
}


