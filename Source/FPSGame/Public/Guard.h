// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Guard.generated.h"

/** describes all available guard states   */
UENUM(BlueprintType)
enum class EGuardState : uint8
{
	GS_Idle UMETA(DisplayName = "Idle"),
	GS_Patrol UMETA(DisplayName = "Patrol"),
	GS_Suspicious UMETA(DisplayName = "Suspicious"),
	GS_Attack UMETA(DisplayName = "Attack")
};

UCLASS()
class FPSGAME_API AGuard : public ACharacter
{
	GENERATED_BODY()

	/** Pawn Sensing Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensingComponent;
	
	/** Widget Component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* WidgetComponent;

public:
	
	/** calls to change the guard state to another  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetGuardState(EGuardState NewState);

	/** calls to update widget component  */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void OnChangeState(EGuardState NewState);

protected:

	// Sets default values for this character's properties
	AGuard();

	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Editor code to make updating values in the editor cleaner
	#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	/** Calls every time when this character see the player  */
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	/* Hearing function - will be executed when we hear a Pawn */
	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);
	
private:

	/** calls to rotate the AI to noise source  */
	void RotateTo(FVector NoiseLocation);

	/** calls to return patrolling after some suspicious time interval  */
	void ReturnToPatrol();

	/** calls to update current patrol to next patrol point 
	*	when we have reached the previous patrol point */
	void SetNextPatrolPoint();
	
	/** spotted enemy  */
	UPROPERTY()
	class APawn* TargetPawn;
	
	/** our AI controller reference  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class AAIController* AIController;

	/** first target point to patrol  */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true", EditCondition = "bPatrol"))
	class AActor* TargetPoint01;

	/** second target point to patrol  */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true", EditCondition = "bPatrol"))
	class AActor* TargetPoint02;

	/** current destination target point  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class AActor* CurrentPatrolPoint;
	
	// Called whenever OnRep_GuardState updated to update widget
	UFUNCTION()
	virtual void OnRep_GuardState();

	/** current state of the AI  */
	UPROPERTY(ReplicatedUsing = OnRep_GuardState, VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	EGuardState GuardState = EGuardState::GS_Idle;

	/** acceptance radius to current patrol point  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius = 50.f;

	/** shows how many seconds guard will be in suspicious state when he will heard the noise */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float SuspiciousTime = 5.f;

	/* Let the guard go on patrol */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	uint32 bPatrol : 1;

	/** timer that handles suspicious delay  */
	FTimerHandle ReturnToPatrolTimer;

public:
	
	/** returns current AI state **/
	FORCEINLINE EGuardState GetGuardState() const { return GuardState; }
	
};
