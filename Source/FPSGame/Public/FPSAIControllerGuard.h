// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"
#include "FPSAIControllerGuard.generated.h"


/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAIControllerGuard : public AAIController
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "AI")
		TArray<AActor*> Waypoints;

	UFUNCTION(Category = "AI")
		ATargetPoint* GetRandomWaypoint();

	UFUNCTION(Category = "AI")
		void GoToRandomWaypoint();

	FTimerHandle TimerHandle;
};
