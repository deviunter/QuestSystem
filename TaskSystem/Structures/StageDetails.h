// © Skydream Interactive LLC 2020-2025

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h" 
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "ObjectiveDetails.h"
#include "StageDetails.generated.h"

USTRUCT(BlueprintType)
struct FStageDetails
{
	GENERATED_BODY()

public:

	FStageDetails() : bHasStageSublevel(false) {}

	/*    OBJECTIVES    */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FObjectiveDetails> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasStageSublevel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StageSublevel;

	/*    STAGE REWARD   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayTag> GameplayTagsReward;

};