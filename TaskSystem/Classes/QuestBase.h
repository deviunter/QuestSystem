// © Skydream Interactive LLC 2020-2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Object.h"
#include "TaskSystem/Structures/QuestDetails.h"
#include "GameplayTagContainer.h"
#include "TaskSystem/Structures/CurrentObjectives.h"
#include "TaskSystem/Structures/StageDetails.h"
#include "TaskSystem/Structures/CompletedObjective.h"
#include "TaskSystem/Enumerators/QuestEnumerators.h"
#include "TaskSystem/Structures/QuestBaseSave.h"
#include "TaskSystem/ObjectiveSpawners/ObjectiveSpawner.h"
#include "QuestBase.generated.h"

class UTaskLogger;

UCLASS(Blueprintable, BlueprintType)
class AFTERDARKNESS_API UQuestBase : public UObject
{
	GENERATED_BODY()

public:

	/*    DEFAULT FUNCTIONS   */

	/*Initial function. It sets default values ??for variables, clears arrays and loads the quest table to install the data manager*/
	UFUNCTION(BlueprintCallable, Category = "Default")
	void Initialization();

	/*    CHECK FUNCTIONS     */

	/*Search for objective in the list of current objectives (fcurrentobjectives) if found - returns "True" if not "False"*/
	UFUNCTION(BlueprintCallable, Category = "Check")
	bool FindObjectiveByID(FString ObjectiveID);

	/*Searches for the selected target in the list of current targets and if found returns the index of the selected target, otherwise returns "-1"*/
	UFUNCTION(BlueprintCallable, Category = "Check")
	int32 GetObjectiveIndex(FString ObjectiveID);

	/*Searches for a target in the full list of targets, returns "true" if found*/
	UFUNCTION(BlueprintCallable, Category = "Check")
	bool FindInAllObjectives(FString ObjectiveID);

	/*Returns the value of the structure if the specified ObjectiveID and the ObjectiveID of the array index match*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Check")
	FCurrentObjectives GetObjectiveData(FString ObjectiveID);

	/*    QUEST FUNCTIONS     */

	/*Activates the quest and spawns a quest marker on the current targets, unless these targets are hidden until completed (bIsObjectiveHidden == true)*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ActivateQuest();

	/*Deactivate quest and destroys a quest marker on the current targets*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void DeactivateQuest();

	/*Sets bIsQuestComplete flag & call CompleteQuest Function in TaskLogger*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void CompleteQuest();
	
	/*Updates the current mission objectives depending on the stage*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void RefreshQuest();

	/*    STAGE FUNCTIONS    */

	/*Update Stage*/
	UFUNCTION(BlueprintCallable, Category = "Stage")
	void UpdateStage();

	UFUNCTION(BlueprintCallable, Category = "Stage")
	void StageReward();  

	/*    SAVE GAME FUNCTIONS    */

	/*Return Save Data for SaveGameToSlot*/
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	FQuestBaseSave GetSaveData();

	/*Get LoadedData from LoadGameFromSlot*/
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SetLoadData(FQuestBaseSave LoadedData);

	/*    OBJECTIVE FUNCTIONS    */

	/*The process of completing a goal. If it is in the current list, it is either executed or its Ammound is incremented.
If it is not in the current list, it is removed from AllObjectives, then if there are few goals left,
they are checked for possible ignorability or optionality. If possible, they are ignored and the stage is updated*/
	UFUNCTION(BlueprintCallable, Exec, Category = "Objective")
	void ObjectiveComplete(FString ObjectiveID, int32 AddedValue);

	/*The process of failing a goal. If it is in the current list, it is either executed or its Ammound is incremented.
If it is not in the current list, it is removed from AllObjectives, then if there are few goals left,
they are checked for possible ignorability or optionality. If possible, they are ignored and the stage is updated*/
	UFUNCTION(BlueprintCallable, Exec, Category = "Objective")
	void ObjectiveFailed(FString ObjectiveID, bool ForceUpdateStage);

	/*The process of ignoring a goal. If it is in the current list, it is either executed or its Ammound is incremented.
If it is not in the current list, it is removed from AllObjectives, then if there are few goals left,
they are checked for possible ignorability or optionality. If possible, they are ignored and the stage is updated*/
	UFUNCTION(BlueprintCallable, Exec, Category = "Objective")
	void ObjectiveIgnored(FString ObjectiveID, bool ForceUpdateStage);

	/*If there are less than 2 targets, then this method is launched, the targets are checked for the possibility of ignoring and optionality.
if the target is bIsOptional or bAllowFail then this target is ignored*/
	UFUNCTION(BlueprintCallable, Category = "Objective")
	void IsOptionalRemainder();
	
	/*Overwrites the maximum number of the target if it is in the CurrentObjectives list*/
	UFUNCTION(BlueprintCallable, Category = "Objective")
	void WriteMaxAmmound(FString ObjectiveID,int32 NewMaxAmmound);

	/*If the target was failed or ignored, it is replaced with another target(s)
if it has them in the ReplaceObjectives array in the FObjectiveDetails structure*/
	UFUNCTION(BlueprintCallable, Category = "Objective")
	void ReplaceObjective(TArray<FName> ReplaceID);

	/*The check whether the goal contains a list of conflicting goals is activated only in the ObjectiveComplete() method,
if the goal is completed, then the conflicting goals will be automatically ignored*/
	UFUNCTION(BlueprintCallable, Category = "Objective")
	void CheckConflictObjectives(FString ObjectiveID);

	/*Create CompleteObjective Array Elem*/
	UFUNCTION(BlueprintCallable, Category = "Objective")
	void MakeCompletedObjective(FCurrentObjectives Objective, EObjectiveCompleteType CompleteType);

	UFUNCTION(BlueprintCallable, Category = "Objective")
	bool CheckPossibilityForGenerateObjective(FObjectiveDetails ObjectiveDetails);

	UFUNCTION(BlueprintCallable, Category = "Objective")
	bool GenerateObjective(FString ObjectiveID, FObjectiveDetails ObjectiveDetails);

	/*     DISPATCHERS     */

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageCompleted, int32, StageIndex);
	UPROPERTY(BlueprintAssignable)
	FOnStageCompleted OnStageCompleted;

	// ﬂ ’Œ“≈À —Œ«ƒ¿“‹  ¿ »≈ “Œ ƒ≈À≈√¿“€ ƒÀﬂ –¿«Õ€’ —»“”¿÷»… ¬Œ“ “ŒÀ‹ Œ «¿¡€À  ¿ »≈
	// Õ¿ƒŒ ¬—œŒÃÕ»“‹ » ≈—À» Õ≈ ¡”ƒ≈“ œŒƒ –” Œ…  Œƒ¿ » «¿œ»—¿“‹ ◊“Œ «¿ ƒ≈À≈√¿“€ ﬂ ’Œ“≈À —Œ«ƒ¿“‹

	// delegate for SPECIAL ACTORS & SYSTEMS

	/*    VARIABLES    */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UTaskLogger* TaskLogger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	int32 CurrentStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FCurrentObjectives> CurrentObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FCurrentObjectives> AllObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FQuestDetails QuestDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool bIsQuestTracked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool bIsQuestCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FStageDetails StageDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FCompletedObjective> CompletedObjectives;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	FTimerHandle UpdateDelay;
};