// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHoleActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
// #include "Engine/StaticMeshActor.h"

// Sets default values
AFPSBlackHoleActor::AFPSBlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;


	SphereBlackHole = CreateDefaultSubobject<USphereComponent>(TEXT("SphereBlackHole"));
	SphereBlackHole->SetSphereRadius(100);
	SphereBlackHole->SetupAttachment(MeshComp);


	OuterSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	OuterSphereComp->SetSphereRadius(3000);
	OuterSphereComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AFPSBlackHoleActor::BeginPlay()
{
	Super::BeginPlay();

	SphereBlackHole->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHoleActor::OverlapInnerSphere);
	
}

void AFPSBlackHoleActor::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray <UPrimitiveComponent*> OverlappingComps;
	OuterSphereComp->GetOverlappingComponents(OverlappingComps);

	int32 listSize = OverlappingComps.Num();

	//UE_LOG(LogTemp, Log, TEXT("Finded actors %i"), listSize);
	
	for (int32 i = 0; i < OverlappingComps.Num(); ++i)
	{
		UPrimitiveComponent* box = OverlappingComps[i];
		if (box && box->IsSimulatingPhysics())
		{
			const float SphereRadius = OuterSphereComp->GetScaledSphereRadius();
			const float ForceStrength = -2000;
			box->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);

		}
	}

}

