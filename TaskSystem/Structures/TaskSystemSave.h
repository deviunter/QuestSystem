// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskSystem/Structures/QuestBaseSave.h"
#include "TaskSystemSave.generated.h"

USTRUCT(BlueprintType)
struct FTaskSystemSave
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActiveQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ReceivedQuests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> CompletedQuests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestBaseSave> QuestBaseSaves;
};