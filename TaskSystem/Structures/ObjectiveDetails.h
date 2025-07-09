// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h" 
#include "Enumerators/QuestEnumerators.h"
#include "GenerateDetails.h"
#include "ObjectiveDetails.generated.h"

USTRUCT(BlueprintType)
struct FObjectiveDetails : public FTableRowBase
{
	GENERATED_BODY()

public:

	FObjectiveDetails() :
		ObjectiveType(EObjectiveType::LookAt),
		MaxAmmound(0),
		bIsOptional(false),
		bAllowIgnore(false),
		bAllowFail(false),
		bIsObjectiveHidden(false)
	{
	}

	//Goal name displayed in player UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ObjectiveName;

	//ID of each object for which the search is performed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectiveType ObjectiveType = EObjectiveType::LookAt;

	//If the target is a single copy, then you need to leave the value 0 or set it to 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmound = 0;

	//Select this if the goal is optional to complete (if there are less than 2 goals left, the goal is included regardless of the specified: bool bAllowIgnore and bool bAllowFail)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOptional = false;

	//Set this if the target can be ignored, (the target is ignored if there are less than 2 targets left in the stage)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowIgnore = false;

	//Set this if the objective can be failed, otherwise the game will be over.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowFail = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ConflictWith;

	//It is necessary to indicate if this goal can be replaced with a similar one.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ObjectiveReplaceID;

	//Set this if the target is hidden until acquired (it does not have a TargetMark)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsObjectiveHidden = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bObjectiveInitialSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGenerateDetails ObjectiveSpawnInfo;
};