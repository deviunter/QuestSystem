// © Skydream Interactive LLC 2020-2025

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ObjectiveDetails.h"
#include "TaskSystem/Enumerators/QuestEnumerators.h"
#include "CompletedObjective.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FCompletedObjective
{
	GENERATED_BODY()

public:

	FCompletedObjective() : CompleteType(EObjectiveCompleteType::Completed) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FObjectiveDetails Objective;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectiveCompleteType CompleteType = EObjectiveCompleteType::Completed;
};