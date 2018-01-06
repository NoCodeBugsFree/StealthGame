// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "BlackHole.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API ABlackHole : public AStaticMeshActor
{
	GENERATED_BODY()
	
	/* kill sphere component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* KillSphere;

	/* attractive sphere component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AttractiveSphere;

protected:

	ABlackHole();

	virtual void Tick(float DeltaSeconds) override;

	/** calls when kill sphere overlaps by another actor  */
	UFUNCTION()
	void OnKillSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	
	/** the force strength to attract physics objects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float ForceStrength = 2500.f;
};
