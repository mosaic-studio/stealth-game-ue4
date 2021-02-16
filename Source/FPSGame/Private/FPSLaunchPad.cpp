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

	const float SpeedForce = 1100.0f;

	if (MyPawn != nullptr) {
		const FVector ForwardDir = this->GetActorForwardVector();
		const FVector AddForce = (ForwardDir + FVector(0, 0, 1)) * SpeedForce;
		MyPawn->LaunchCharacter(AddForce, true, true);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Overlap!"));
		UPrimitiveComponent* box = Cast<UPrimitiveComponent>(OtherComp);
		if ((box != NULL) && (box->IsSimulatingPhysics()))
		{
			UE_LOG(LogTemp, Log, TEXT("Box Overlap!"));
			const FVector ForwardDir = this->GetActorForwardVector();
			const FVector Impulse = (ForwardDir + FVector(0, 0, 1)) * SpeedForce;

			box->AddImpulse(Impulse, "None", true);
		}
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
}


