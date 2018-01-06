// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FPSGameState.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	/** broadcasts mission complete for all players  */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnMissionComplete(class APawn* InstigatorPawn, bool bMisiionSuccess);
	void MulticastOnMissionComplete_Implementation(class APawn* InstigatorPawn, bool bMisiionSuccess);
	
protected:

	virtual void BeginPlay() override;

	/** calls to set level camera reference for further lerp to  */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void SetLevelCameraReference();

	/** lever camera reference  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class AActor* LevelCamera;
};
