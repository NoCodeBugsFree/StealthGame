// Fill out your copyright notice in the Description page of Project Settings.

#include "Guard.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** Pawn Sensing Component  */
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	PawnSensingComponent->HearingThreshold = 1400.f;
	PawnSensingComponent->LOSHearingThreshold = 2800.f;
	PawnSensingComponent->SightRadius = 5000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(30.f);
	PawnSensingComponent->SensingInterval = 0.5f;
	PawnSensingComponent->HearingMaxSoundAge = 1.f;
	PawnSensingComponent->bEnableSensingUpdates = true;
	PawnSensingComponent->bOnlySensePlayers = true;
	PawnSensingComponent->bSeePawns = true;
	PawnSensingComponent->bHearNoises = true;

	/** Widget Component  */
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	bPatrol = true;

	// Replication
	SetReplicates(true);
	SetReplicateMovement(true);
}

void AGuard::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AGuard::OnSeePlayer);
		PawnSensingComponent->OnHearNoise.AddDynamic(this, &AGuard::OnHearNoise);
	}
}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();

	if(AAIController* TestAIController = Cast<AAIController>(Controller))
	{
		AIController = TestAIController;
	}
	if (!ensure(AIController)) { return; }
	
	if (bPatrol)
	{
		SetGuardState(EGuardState::GS_Patrol);
		SetNextPatrolPoint();
	}
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AIController)
	{

		switch (GuardState)
		{
		case EGuardState::GS_Patrol:
			if (CurrentPatrolPoint && bPatrol)
			{
				if (AIController->MoveToActor(CurrentPatrolPoint, AcceptanceRadius) == EPathFollowingRequestResult::AlreadyAtGoal)
				{
					SetNextPatrolPoint();
				}
			}
			break;

		case EGuardState::GS_Suspicious:
			AIController->StopMovement();
			ReturnToPatrol();
			break;

		case EGuardState::GS_Attack:
			AIController->StopMovement();
			break;

		default:
			break;	
		}
	}
}

void AGuard::OnSeePlayer(APawn* Pawn)
{
	if (Pawn)
	{
		TargetPawn = Pawn;

		SetGuardState(EGuardState::GS_Attack);

		FVector Location = TargetPawn->GetActorLocation();

		RotateTo(Location);

		DrawDebugSphere(GetWorld(), Location, 150.f, 12, FColor::Red, false, 5.f, 0, 1.f);

		/** lose the game  */
		if (AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode()))
		{
			FPSGameMode->CompleteMission(TargetPawn, false);
		}
	}
}

void AGuard::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	/** attack state has more priority  */
	if (GuardState != EGuardState::GS_Attack)
	{
		/** we are interesting at noise source  */
		RotateTo(Location);
		SetGuardState(EGuardState::GS_Suspicious);

		/** draw sphere at noise location  */
		DrawDebugSphere(GetWorld(), Location, 150.f, 12, FColor::Green, false, 5.f, 0, 1.f);

		/** if we now is suspicious and waiting returning to patrol
		*	we must reset suspicious time */
		GetWorldTimerManager().ClearTimer(ReturnToPatrolTimer);
	}
}

void AGuard::ReturnToPatrol()
{
	if (!ReturnToPatrolTimer.IsValid())
	{
		/** return to patrolling with delay  */
		FTimerDelegate ReturnToPatrolDelegate;
		ReturnToPatrolDelegate.BindLambda([&] {
			if (GuardState != EGuardState::GS_Attack)
			{
				SetGuardState(EGuardState::GS_Patrol);
			}
		});
		GetWorldTimerManager().SetTimer(ReturnToPatrolTimer, ReturnToPatrolDelegate, SuspiciousTime, false);
	}
}

void AGuard::SetNextPatrolPoint()
{
	if (!ensure(TargetPoint01 && TargetPoint02)) { return; }

	if (CurrentPatrolPoint == TargetPoint01)
	{
		CurrentPatrolPoint = TargetPoint02;
	} 
	else
	{
		CurrentPatrolPoint = TargetPoint01;
	}
}

void AGuard::OnRep_GuardState()
{
	OnChangeState(GuardState);
}

void AGuard::RotateTo(FVector NoiseLocation)
{
	FVector DirectioinToNoiseSource = NoiseLocation - GetActorLocation();
	FMatrix RotationMatrix = FRotationMatrix::MakeFromX(DirectioinToNoiseSource); 
	FRotator DesiredRotation = RotationMatrix.Rotator();
	DesiredRotation.Pitch = 0.f;
	DesiredRotation.Roll = 0.f;
	SetActorRotation(DesiredRotation);
}

void AGuard::SetGuardState(EGuardState NewState)
{
	if (GuardState != NewState)
	{
		GuardState = NewState;
		/** Server doesn't call it automatically !  */
		if (Role == ROLE_Authority)
		{
			OnRep_GuardState();
		}
	}
}

#if WITH_EDITOR
void AGuard::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// here what to check before Super::
	if (bPatrol == false)
	{
		SetGuardState(EGuardState::GS_Idle);
		TargetPoint01 = nullptr;
		TargetPoint02 = nullptr;
	}
	else
	{
		SetGuardState(EGuardState::GS_Patrol);
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// GetLifetimeReplicatedProps
/** Returns properties that are replicated for the lifetime of the actor channel */
void AGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGuard, GuardState);
}