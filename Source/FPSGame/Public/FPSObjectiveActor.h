// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()

	/* static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;
	
	/* overlapping sphere component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;

	/* Emitter Template */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* PickupFX;

protected:

	// Sets default values for this actor's properties
	AFPSObjectiveActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** calls to play pickup FX  */
	void PlayFX();

	/** calls when this actor will be overlapped by another  */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
