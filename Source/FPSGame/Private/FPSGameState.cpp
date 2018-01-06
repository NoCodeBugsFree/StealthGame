// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameState.h"
#include "EngineUtils.h"
#include "LevelCamera.h"
#include "EngineUtils.h"
#include "FPSPlayerController.h"

void AFPSGameState::BeginPlay()
{
	Super::BeginPlay();

	SetLevelCameraReference();
}

void AFPSGameState::SetLevelCameraReference()
{
	TArray<AActor*> LevelCameras;
	for (TActorIterator<ALevelCamera> Iter(GetWorld()); Iter; ++Iter)
	{
		if ((*Iter))
		{
			LevelCameras.Add(*Iter);
		}
	}
	if (LevelCameras.IsValidIndex(0))
	{
		LevelCamera = LevelCameras[0];
	}
}

void AFPSGameState::MulticastOnMissionComplete_Implementation(class APawn* InstigatorPawn, bool bMisiionSuccess)
{
	/** for each pawn in the world  */
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		if (APawn* Pawn = Cast<APawn>(*It))
		{
			/** valid and locally controlled pawn  */
			if (Pawn->IsLocallyControlled())
			{
				/** disable input  */
				Pawn->DisableInput(nullptr);

				if (AFPSPlayerController* PC = Cast<AFPSPlayerController>(Pawn->GetController()))
				{
					/** swap cameras  */
					if (LevelCamera)
					{
						PC->SetViewTargetWithBlend(LevelCamera, 1.f, EViewTargetBlendFunction::VTBlend_Cubic);
					}

					/** update HUD message  */
					PC->OnMissionComplete(InstigatorPawn, bMisiionSuccess);	
				}
			}
		}
	}
}