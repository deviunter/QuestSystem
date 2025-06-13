// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h" 
#include "Enumerators/QuestEnumerators.h"
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
	//Íàçâàíèå öåëè îòîáðàæàåìîå â èíòåôåéñå èãðîêà
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ObjectiveName;

	//ID of each object for which the search is performed
	//ID êàæäîãî îáúåêòà ïî êîòîðîìó ïðîâîäèòñÿ ïîèñê
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectiveType ObjectiveType = EObjectiveType::LookAt;

	//If the target is a single copy, then you need to leave the value 0 or set it to 1
	//Åñëè öåëü â åäåíè÷íîì ýêçåìïëÿðå, òî íóæíî îñòàâèòü çíà÷åíèå 0 èëè ïîñòàâèòü 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmound = 0;

	//Select this if the goal is optional to complete (if there are less than 2 goals left, the goal is included regardless of the specified: bool bAllowIgnore and bool bAllowFail)
	//Âûáåðèòå ýòî åñëè öåëü íåîáÿçàòåëüíà äëÿ âûïîëíåíèÿ (åñëè öåëåé îñòàëîñü ìåíüøå 2, òî öåëü èíãîðèðóåòñÿ íåçàâèñèìî îò ïðîñòàâëåííîãî: bool bAllowIgnore è bool bAllowFail)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOptional = false;

	//Set this if the target can be ignored, (the target is ignored if there are less than 2 targets left in the stage)
	//Âûáåðèòå ýòî åñëè öåëü ìîæíî èãíîðèðîâàòü, (öåëü èãíîðèðóåòñÿ, åñëè íà ñòàäèè öåëåé îñòàëîñü ìåíüøå 2)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowIgnore = false;

	//Set this if the objective can be failed, otherwise the game will be over.
	//Âûáåðèòå ýòî, åñëè öåëü ìîæíî ïðîâàëèòü, â ïðîòèâíîì ñëó÷àå èãðà áóäåò îêîí÷åíà
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowFail = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ConflictWith;

	//It is necessary to indicate if this goal can be replaced with a similar one.
	//Íåîáõîäèìî ïðîñòàâèòü åñëè ýòó öåëü ìîæíî çàìåíèòü íà àíàëîãè÷íóþ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ObjectiveReplaceID;

	//Set this if the target is hidden until acquired (it does not have a TargetMark)
	//Âûáåðèòå ýòî åñëè öåëü ñêðûòà äî ïîëó÷åíèÿ (íà íåé íåòó TargetMark)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsObjectiveHidden = false;
};