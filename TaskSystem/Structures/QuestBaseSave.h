// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskSystem/Structures/QuestDetails.h"
#include "TaskSystem/Structures/CurrentObjectives.h"
#include "TaskSystem/Structures/StageDetails.h"
#include "TaskSystem/Structures/CompletedObjective.h"
#include "QuestBaseSave.generated.h"

USTRUCT(BlueprintType)
struct FQuestBaseSave
{
	GENERATED_BODY()

public:

	FQuestBaseSave() :
		CurrentStage(0),
		bIsQuestTracked(false),
		bIsQuestCompleted(false)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuestDetails QuestDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCurrentObjectives> CurrentObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCurrentObjectives> AllObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStageDetails StageDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCompletedObjective> CompletedObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsQuestTracked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsQuestCompleted = false;

};