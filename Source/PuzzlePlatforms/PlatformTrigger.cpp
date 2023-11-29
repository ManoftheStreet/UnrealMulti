// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatFormTrigger.h"
#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

// Sets default values
APlatFormTrigger::APlatFormTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	/*if (TriggerVolume)
	{
		RootComponent = TriggerVolume;
	}*/
	if (!ensure(TriggerVolume != nullptr)) return;
	RootComponent = TriggerVolume;

	
}

// Called when the game starts or when spawned
void APlatFormTrigger::BeginPlay()
{
	Super::BeginPlay();
	if (TriggerVolume)
	{
		TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatFormTrigger::OnOverlapBegin);
		TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatFormTrigger::OnOverlapEnd);
	}
	
}

// Called every frame
void APlatFormTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatFormTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

void APlatFormTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}

