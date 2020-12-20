// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"

// Engine Includes
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	if (TriggerBox != nullptr)
	{
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
		SetRootComponent(TriggerBox);
	}

	LaunchPadBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaunchPadBase"));
	if (LaunchPadBase != nullptr)
	{
		TriggerBox->SetupAttachment(GetRootComponent());
	}

	LaunchPadIcon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaunchPadIcon"));
	if (LaunchPadIcon != nullptr)
	{
		LaunchPadIcon->SetupAttachment(GetRootComponent());
	}

	LaunchDirection = CreateDefaultSubobject<USceneComponent>(TEXT("LaunchDirection"));
	if (LaunchDirection != nullptr)
	{
		LaunchDirection->SetupAttachment(GetRootComponent());
	}

	LaunchSpeed = 1000.0f;
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox != nullptr)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OnTriggerBoxBeginOverlap);
	}
}

void AFPSLaunchPad::OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Launch Character
	if (OtherActor != nullptr)
	{
		ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
		if (OtherCharacter != nullptr)
		{
			OtherCharacter->LaunchCharacter(LaunchDirection->GetForwardVector() * LaunchSpeed, true, true);
			PlayLaunchEffects();
			return;
		}
	}
	
	// Launch Actors Simulating Physics
	if (OtherComp != nullptr)
	{
		if (OtherComp->IsSimulatingPhysics() == true)
		{
			OtherComp->AddImpulse(LaunchDirection->GetForwardVector() * LaunchSpeed, NAME_None, true);
			PlayLaunchEffects();
		}
	}
}

void AFPSLaunchPad::PlayLaunchEffects()
{
	if (LaunchFX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchFX, GetActorLocation());
	}

	if (LaunchSX != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), LaunchSX);
	}
}


