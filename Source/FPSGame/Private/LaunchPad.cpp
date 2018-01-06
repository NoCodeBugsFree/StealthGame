// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchPad.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"

ALaunchPad::ALaunchPad()
{
	/* box simple collision  */
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(40.f, 40.f, 30.f));
	BoxComponent->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	/** body  */
	GetStaticMeshComponent()->SetupAttachment(RootComponent);
	GetStaticMeshComponent()->SetRelativeScale3D(FVector(1.f, 1.f, 0.1f));
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

	/**  set the default static mesh to root mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeTemplate(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeTemplate.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(CubeTemplate.Object);
	}

	/* arrow plane static mesh  */
	ArrowPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	ArrowPlane->SetupAttachment(RootComponent);
	ArrowPlane->SetRelativeLocation(FVector(0.f, 0.f, 6.f));

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneTemplate(TEXT("/Engine/BasicShapes/Plane"));
	if (PlaneTemplate.Succeeded())
	{
		ArrowPlane->SetStaticMesh(PlaneTemplate.Object);
	}

	/** set the explosion emitter  */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("/Game/Effects/P_gold_ore_appearance"));
	if (ParticleSystem.Object)
	{
		EmitterTemplate = ParticleSystem.Object;
	}
}

void ALaunchPad::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OnOverlapBegin);
}

void ALaunchPad::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (EmitterTemplate)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterTemplate, GetActorTransform());
		}

		if(ACharacter* Character = Cast<ACharacter>(OtherActor))
		{
			Character->LaunchCharacter(LaunchVelocity, true, true);
		}
		else if (OtherComp->IsSimulatingPhysics())
		{
			FVector Location = OtherComp->GetCenterOfMass();
			OtherComp->AddImpulseAtLocation(LaunchVelocity * OtherComp->GetMass(), Location);
		}
	}
}
