// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatfromTrigger.h"
#include "Components/BoxComponent.h"

// Sets default values
APlatfromTrigger::APlatfromTrigger()
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
void APlatfromTrigger::BeginPlay()
{
	Super::BeginPlay();
	if (TriggerVolume)
	{
		TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatfromTrigger::OnOverlapBegin);
		TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatfromTrigger::OnOverlapEnd);
	}
	
}

// Called every frame
void APlatfromTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatfromTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Activated!"));
	//UE_LOG(LogTemp, Warning, TEXT("Overlap with: %s"), *OtherComp->GetName());
	if (OtherActor != nullptr && OtherComp != nullptr)
	{
		FString ActorName = OtherActor->GetName();
		FString ComponentName = OtherComp->GetName();

		UE_LOG(LogTemp, Warning, TEXT("%s Overlapped with %s"), *ActorName, *ComponentName);
	}
}

void APlatfromTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("DeActivated"));
	UE_LOG(LogTemp, Warning, TEXT("Overlap with: %s"), *OtherComp->GetName());
}

