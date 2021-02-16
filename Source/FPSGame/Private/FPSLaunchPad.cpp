// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComp->InitBoxExtent(FVector(100, 100, 50));
	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(false);

	PlatformBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformBox"));
	PlatformBox->SetWorldScale3D(FVector(1.0f, 1.0f, 0.1f));
	PlatformBox->SetupAttachment(RootComponent);

	PlatformArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformArrow"));
	PlatformArrow->SetWorldScale3D(FVector(1.0f, 1.0f, 0.01f));
	PlatformArrow->SetupAttachment(RootComponent);
	PlatformArrow->AddRelativeLocation(FVector(0.0f, 0.0f, 15.0f));

	LaunchStrength = 1500.0f;
	LaunchPitchAngle = 35.0f;

}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);
}

void AFPSLaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);

	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

	if (MyPawn) {
		// const FVector ForwardDir = this->GetActorForwardVector();
		// const FVector LaunchVelocity = (ForwardDir + FVector(0, 0, 1)) * LaunchStrength;
		MyPawn->LaunchCharacter(LaunchVelocity, true, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}
	else if ((OtherComp != NULL) && (OtherComp->IsSimulatingPhysics())) {
		// const FVector ForwardDir = this->GetActorForwardVector();
		// const FVector Impulse = (ForwardDir + FVector(0, 0, 1)) * LaunchStrength;

		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}
}


