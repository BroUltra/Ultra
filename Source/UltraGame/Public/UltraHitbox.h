// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UltraHitbox.generated.h"

UENUM(BlueprintType)
enum class EHitboxEnum : uint8
{
	HB_STRIKE	UMETA(DisplayName = "Strike"),
	HB_HURTBOX	UMETA(DisplayName = "Hurtbox")
};

UCLASS()
class ULTRAGAME_API AHitbox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitbox();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// The score this hitbox will apply
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	int hitboxScore;

	// The location to spawn the hitbox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	FVector htboxLocation;

	// The type of the hitbox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	EHitboxEnum hitboxType;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
