// © Skydream Interactive LLC 2020-2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enumerators/QuestEnumerators.h"
#include "TaskSystem/Classes/QuestBase.h"
#include "TaskSystem/Structures/QuestDetails.h"
#include "TaskSystem/Structures/CurrentObjectives.h"
#include "QuestInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UQuestInterface : public UInterface
{
	GENERATED_BODY()
};

class AFTERDARKNESS_API IQuestInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/*        OBJECTIVE FUNCTIONS        */

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective")
	void ActivateObjective(FObjectiveDetails ObjectiveDetails, UQuestBase* QuestBaseReference);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective")
	void DeactivateObjective();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective")
	void SpawnTargetMark(EQuestType QuestType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective")
	void DestroyTargetMark();

	//Return Actor Objective ID
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Objective")
	FString GetMyObjectiveID();
};
