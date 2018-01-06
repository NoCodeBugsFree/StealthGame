// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "LaunchPad.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API ALaunchPad : public AStaticMeshActor
{
	GENERATED_BODY()
	
	/* arrow plane static mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ArrowPlane;
	
	/* box simple collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	/* Emitter Template */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* EmitterTemplate;

protected:

	ALaunchPad();

	virtual void PostInitializeComponents() override;

	/** calls when launch pad starts overlapping  */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);	

private:
	
	/** the velocity to launch the character when he overlaps this launch pad  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector LaunchVelocity = FVector(500.f, 0.f, 1500.f);
};
