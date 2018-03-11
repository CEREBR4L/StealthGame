// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	DestroySphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("DestroySphereComp"));
	DestroySphereComp->SetSphereRadius(100);
	DestroySphereComp->SetupAttachment(MeshComp);

	DestroySphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapInnerSphere);

	AttractSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("AttractSphereComp"));
	AttractSphereComp->SetSphereRadius(3000);
	AttractSphereComp->SetupAttachment(MeshComp);
}

void AFPSBlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	AttractSphereComp->GetOverlappingComponents(OverlappingComponents);

	for (int32 i = 0; i < OverlappingComponents.Num(); i++) {
		UPrimitiveComponent* PrimComp = OverlappingComponents[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			// the component we are looking for! It needs to be simulating in order to apply forces.

			const float SphereRadius = AttractSphereComp->GetScaledSphereRadius();
			const float ForceStrength = -2000; // Negative value to make it pull towards the origin instead of pushing away

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

