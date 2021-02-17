// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIControllerGuard.h"
#include "Kismet/GameplayStatics.h"


void AFPSAIControllerGuard::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), Waypoints);

    GoToRandomWaypoint();
}

void AFPSAIControllerGuard::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    GetWorldTimerManager().SetTimer(TimerHandle, this, &AFPSAIControllerGuard::GoToRandomWaypoint, 1.0f, false);
}

ATargetPoint* AFPSAIControllerGuard::GetRandomWaypoint()
{
    auto index = FMath::RandRange(0, Waypoints.Num() - 1);
    return Cast<ATargetPoint>(Waypoints[index]);
}

void AFPSAIControllerGuard::GoToRandomWaypoint()
{
    MoveToActor(GetRandomWaypoint());
}
