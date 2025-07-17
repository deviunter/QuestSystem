// © Skydream Interactive LLC 2020-2025

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