![After Darkness Ingame Logo](https://github.com/user-attachments/assets/6c2a4edd-925f-44ab-8694-50076bbd7904)
# AFTER DARKNESS QUEST SYSTEM

`v.1.0.7` `From 18.07.2025` `In Work`

`After Darkness © 2020 - 2025 Skydream Interactive. All rights reserved.`

Developed in Unreal Engine 5. "Unreal Engine" and its logo are trademarks or registered trademarks of Epic Games, Inc.
"After Darkness" and its logo are trademarks or registered trademarks of Skydream Interactive LLC

This project and its contents are the intellectual property of Skydream Interactive & Deviunter. Unauthorized copying, reproduction, or distribution is strictly prohibited.

# FUTURE PLANS
• Generated targets - if the system does not detect a target, it will automatically generate its analogue to avoid softlock. Or if the ObjectiveDetails initially specifies that this target must be generated

• Generated quests. Adding the ability to generate quests from templates for the **OCG CONFRONTATION** and **MEASURE** types

• **GENERATED OBJECTIVES** Save & Load, in QuestSystem or in outer game systems

# WHAT'S NEW
`v. 1.0.7`
Minor changes in QuestBase - finally added delivery for OnStageCompleted delegate

`v. 1.0.6`
Minor changes.

`v. 1.0.5`
Added Objective Spawner - class for spawn objective separetly from QuestBase. 

`v. 1.0.4`
Added Data Asset for generated objectives.

`v. 1.0.3`
Updated FindObjectiveIndex() method
``` c++
int32 UQuestBase::GetObjectiveIndex(FString ObjectiveID)
{
	for (int32 i = 0; i < CurrentObjectives.Num(); i++)
	{
		if (CurrentObjectives[i].ObjectiveID == ObjectiveID)
		{
			return i;
		}
	}
	return INDEX_NONE;
}
```
Updated loading of the quest system. Previously, it was missing adding created QuestBase to the array after loading into TaskLogger. And loading of data in QuestBase itself has been updated.
``` c++
if (!bIsQuestCompleted)
	{
		if (bIsQuestTracked == true)
		{
			TArray<AActor*> LocalActors;
			UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UQuestInterface::StaticClass(), LocalActors);
			for (AActor* Elem : LocalActors)
			{
				if (FindObjectiveByID(IQuestInterface::Execute_GetMyObjectiveID(Elem)))
				{
					FString LocalStr;
					LocalStr = IQuestInterface::Execute_GetMyObjectiveID(Elem);
					FObjectiveDetails LocalObjective;
					LocalObjective = GetObjectiveData(LocalStr).ObjectiveDetails;
					IQuestInterface::Execute_ActivateObjective(Elem, LocalObjective, this);
					if (!LocalObjective.bIsObjectiveHidden && bIsQuestTracked == true)
					{
						IQuestInterface::Execute_SpawnTargetMark(Elem, QuestDetails.QuestType);
					}
				}
			}
			AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
			if (CurrentGameMode)
			{
				IQuestInterface::Execute_RefreshQuest((UObject*)CurrentGameMode, this);
			}
		}
	}
```


`v. 1.0.2 HF`
A small hotfix, the quotation mark in the `#include` list was omitted


`v. 1.0.2`
Added interface implementation to display the receipt of new quests.


`v. 1.0.1`
Minor fixes and improvements.


`v. 1.0.0`
Improved UQuestBase: if the goal was multiple, it was impossible to complete it, because the goal could increase the CurrentAmmound value. Now, the logic of ObjectiveComplete has been refined and improved. Now it is possible to complete the goal, after increasing the CurrentAmmound value.
``` c++
if (GetObjectiveData(ObjectiveID).CurrentAmmound >= GetObjectiveData(ObjectiveID).MaxAmmound)
			{
				MakeCompletedObjective(LocalObjective, EObjectiveCompleteType::Completed);
				if (bIsQuestTracked == true)
				{
					AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
					if (CurrentGameMode)
					{
						IQuestInterface::Execute_RemoveLine((UObject*)CurrentGameMode, ObjectiveID, EObjectiveCompleteType::Completed);
					}
				}
				else
				{
					AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
					if (CurrentGameMode)
					{
						IQuestInterface::Execute_NonTrackedTaskUpdated((UObject*)CurrentGameMode, this);
					}
				}
				int32 LocalIndex;
				LocalIndex = GetObjectiveIndex(ObjectiveID);
				CurrentObjectives.RemoveAt(LocalIndex);
				CheckConflictObjectives(ObjectiveID);
				if (CurrentObjectives.Num() < 1)
				{
					//StageReward();
					/*if (FOnStageCompleted.IsBound())
					{
						FOnStageCompleted.Broadcast(CurrentStage);
					}*/
					GetWorld()->GetTimerManager().SetTimer(UpdateDelay, this, &UQuestBase::UpdateStage, 2.f, false, -1.f);
				}
			}
```

# TUTORIAL

**INITIALLY**

1. For correct operation, you need to make changes to the YourGame.Build.cs file by adding paths to the files according to the following example:
``` c#
PublicIncludePaths.AddRange(new string[]
    {
        "AfterDarkness",
        "AfterDarkness/TaskSystem",
        "AfterDarkness/TaskSystem/Classes",
        "AfterDarkness/TaskSystem/Interfaces",
        "AfterDarkness/TaskSystem/Structures",
        "AfterDarkness/TaskSystem/Enumerators"
    });
```

2. In all files you need to replace AFTERDARKNESS_API with YOURAGAMENAME_API. According to the following example:

``` c++
UCLASS(Blueprintable, BlueprintType)
class AFTERDARKNESS_API UQuestBase : public UObject
```
3. It is necessary to replace the paths to DataTable in the executable file QuestBase.cpp
``` c++
// INITIALIZATION
UDataTable* QuestData = LoadObject<UDataTable>(nullptr,
				TEXT("/Game/SD/Systems/TaskSystem/DataTables/DT_Quests.DT_Quests"));
			if (QuestData)
//REPLACEOBJECTIVE
UDataTable* ReplaceData = LoadObject<UDataTable>(nullptr,
				TEXT("/Game/SD/Systems/TaskSystem/DataTables/DT_ReplaceObjectives.DT_ReplaceObjectives"));
			if (ReplaceData)
```
**HIERARCHY OF OBJECTS AND THE PRINCIPLE OF OPERATION OF THE SYSTEM**

*Classes*

`QuestBase` - class inherited from UObject. contains the logic of completing/ignoring/failing goals, updating quest data, stage and decides how this affects further task completion. After completing the quest, it is marked as completed and becomes inactive, remaining in the game and storing information about the quest if it is suddenly needed. Located in `TaskSystem/Classes`

`TaskLogger` - class inherited from AActorComponent. Сontains FName arrays for current and completed quests and the QuestBase array, which stores all quest managers. Located in `TaskSystem/Classes`

`QuestObjective` - class inherited from AActorComponent. The class is attached to the character and is activated on demand when the manager defines the character as a task target. After that, a correct interaction with the component owner will call a method that will trigger the task execution process in the manager. An incorrect interaction will cause the task to fail or be ignored, provided that the task can be ignored or failed, otherwise the game will be over. Located in `TaskSystem/Classes`

*Structures*

`CompletedObjective` - A structure for recording completed goals and the method for achieving them. Located in `TaskSystem/Structures`

`CurrentObjectives` - A structure containing current targets. quantity of each target, its ID, etc. Located in `TaskSystem/Structures`

`ObjectiveDetails` - The main structure of the target, it stores all the data that is edited from the DataTable. Located in `TaskSystem/Structures`

`QuestBaseSave` - Structure for save QuestBase data: QuestID, CurrentStage ect. Located in `TaskSystem/Structures`

`QuestDetails` - The main data structure of a quest contains the name, an array of quest stages, and goals. Located in `TaskSystem/Structures`

`StageDetails` - Here is stored information on the stages of the quest. Located in `TaskSystem/Structures`

`TaskSystemSave` - Basic structure for saving. Located in `TaskSystem/Structures`

`GenerateDetails` - A structure containing info for generate objectives: Spawner class, Objective DataAsset & Spawn Transform etc. Located in `TaskSystem/Structures`

*Data Assets*

`DefaultQuestData` - Default DataAsset class. Located in `TaskSystem/DataAssets`

`CollectbleQuestData` - DataAsset for storing data about different collectibles that use their RowID in the DataTable as a data transfer method. Located in `TaskSystem/DataAssets`

`GoonQuestData` - DataAsset for storing data about a enemy NPC called Goon. Located in `TaskSystem/DataAssets`

`LocationQuestData` - DataAsset for storing data about a specific Volume type actor.

*Objective Spawers*

Objective Spawners are actors in which the process of spawning targets takes place. This logic was specifically allocated to a separate class system to delimit class responsibilities in the architecture, reduce the amount of code in QuestBase, and also to make it possible to conveniently spawn blueprint classes. Located in `TaskSystem/ObjectiveSpawners`

*Enumerators*

`QuestEnumerators` - contains several enumerators for the system to work. Located in `TaskSystem/Enumerators`

*Interfaces*

`QuestInterface` - The connecting link of the entire system. It is used to send updates to the HUD and activate mission objectives. Located in `TaskSystem/Interfaces`


![After Darkness Ingame Logo](https://github.com/user-attachments/assets/6c2a4edd-925f-44ab-8694-50076bbd7904)
`© 2025 Skydream Interactive LLC`
`© 2025 Deviunter`
