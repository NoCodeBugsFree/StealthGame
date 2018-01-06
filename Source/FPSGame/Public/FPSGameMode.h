// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	/** calls by extraction zone when character brings an objective or by Guard
	*	when he spotted the player */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void CompleteMission(class APawn* InstigatorPawn, bool bMisiionSuccess);
		
protected:

	AFPSGameMode();
	
};



