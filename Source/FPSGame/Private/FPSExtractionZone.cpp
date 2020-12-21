// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"

// Engine Includes
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Game Includes
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	if (OverlapComp != nullptr)
	{
		OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		OverlapComp->SetBoxExtent(FVector(200.0f));
		SetRootComponent(OverlapComp);

		OverlapComp->SetHiddenInGame(false);
	}

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	if (DecalComp != nullptr)
	{
		DecalComp->DecalSize = FVector(200.0f);
		DecalComp->SetupAttachment(GetRootComponent());
	}
}

// Called when the game starts or when spawned
void AFPSExtractionZone::BeginPlay()
{
	Super::BeginPlay();

	if (OverlapComp != nullptr)
	{
		OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::OnOverlapCompBeginOverlap);
	}
	
}


void AFPSExtractionZone::OnOverlapCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn == nullptr) return;

	if (MyPawn->bIsCarryingObjective == true)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM != nullptr)
		{
			GM->CompleteMission(MyPawn, true);
		}
	}
	else
	{
		if (ObjectiveMissingSound != nullptr)
		{
			UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Overlap With Extraction Zone"));
}
