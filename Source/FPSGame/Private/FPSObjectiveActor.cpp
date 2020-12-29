// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"

// Engine Includes
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Game Includes
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	if (MeshComp != nullptr)
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetRootComponent(MeshComp);
	}

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	if (SphereComp != nullptr)
	{
		SphereComp->SetupAttachment(MeshComp);
		SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	}

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffects();
}


void AFPSObjectiveActor::PlayEffects()
{
	if (PickupFX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
	}
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
		if (MyCharacter != nullptr && GM != nullptr)
		{
			MyCharacter->bIsCarryingObjective = true;
			APawn* MyPawn = Cast<APawn>(MyCharacter);
			if (MyPawn != nullptr)
			{
				GM->OnObjectivePickedUp(MyPawn);
			}
			Destroy();
		}
	}
}