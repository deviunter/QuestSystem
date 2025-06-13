// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "StageDetails.h"
#include "Enumerators/QuestEnumerators.h"
#include "QuestDetails.generated.h"

USTRUCT(BlueprintType)
struct FQuestDetails : public FTableRowBase
{
	GENERATED_BODY()

public:

	FQuestDetails() :
		QuestType(EQuestType::MainTask),
		MoneyReward(0),
		AbilityPointsAmmound(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TaskName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MultiLine ="true"))
	FText TaskDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType QuestType = EQuestType::MainTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStageDetails> StageDetails;

	/*     REWARD      */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MoneyReward = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AbilityPointsAmmound = 0;

	/*     NEXT TASK   */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextTask;
};
