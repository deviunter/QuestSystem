// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ObjectiveDetails.h"
#include "CurrentObjectives.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCurrentObjectives
{
	GENERATED_BODY()

public:

	FCurrentObjectives() : CurrentAmmound(0), MaxAmmound(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentAmmound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FObjectiveDetails ObjectiveDetails;
};