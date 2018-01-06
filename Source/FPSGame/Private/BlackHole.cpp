// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "ConstructorHelpers.h"

ABlackHole::ABlackHole()
{
	PrimaryActorTick.bCanEverTick = true;

	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(GetStaticMeshComponent());
	/**  UStaticMesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(Sphere.Object);
	}
	
	/** Kill Sphere  */
	KillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("KillSphere"));
	KillSphere->SetupAttachment(RootComponent);
	KillSphere->SetSphereRadius(200.f);
	KillSphere->SetCollisionProfileName("OverlapAll");
	KillSphere->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OnKillSphereOverlapBegin);

	/** Attractive Sphere  */
	AttractiveSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttractiveSphere"));
	AttractiveSphere->SetupAttachment(RootComponent);
	AttractiveSphere->SetSphereRadius(3000.f);
	AttractiveSphere->SetCollisionProfileName("OverlapAll");
}

void ABlackHole::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	TArray<UPrimitiveComponent*> Components;
	AttractiveSphere->GetOverlappingComponents(Components);
	for (UPrimitiveComponent* Component : Components)
	{
		if (Component && Component->IsSimulatingPhysics())
		{
			FVector DirectionToAttract = GetActorLocation() - Component->GetComponentLocation();
			FVector Force = DirectionToAttract * ForceStrength;
			Component->AddForce(Force);

			// -------------- Tom ----------------------------------------------------------
			// const float Radius = AttractiveSphere->GetUnscaledSphereRadius();
			// Component->AddRadialForce(GetActorLocation(), Radius, -ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
			// --------------Tom----------------------------------------------------------
		}
	}
}

void ABlackHole::OnKillSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && !OtherActor->IsPendingKill())
	{
		OtherActor->Destroy();
	}
}
