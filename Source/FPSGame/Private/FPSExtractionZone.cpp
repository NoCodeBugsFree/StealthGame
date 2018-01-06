// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* box simple collision component  */
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(OverlapComp);
	OverlapComp->SetBoxExtent(FVector(200.f, 200.f, 200.f));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::OnOverlapBegin);
	OverlapComp->SetHiddenInGame(false);

	/* decal component  */
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->DecalSize = FVector(200.f, 200.f, 200.f);

	SetReplicates(true);
}

void AFPSExtractionZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(OtherActor))
		{
			if (FPSCharacter->IsCarryingObjective())
			{
				if (AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode()))
				{
					FPSGameMode->CompleteMission(FPSCharacter, true);
				}
			}
			else
			{
				PlayWarningSound();
			}
		}
	}
}

void AFPSExtractionZone::PlayWarningSound()
{
	// try and play the sound if specified
	if (WarningSound)
	{
		UGameplayStatics::PlaySound2D(this, WarningSound);
	}
}

