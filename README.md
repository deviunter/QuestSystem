![After Darkness Ingame Logo](https://github.com/user-attachments/assets/6c2a4edd-925f-44ab-8694-50076bbd7904)
# AFTER DARKNESS QUEST SYSTEM

`v.1.0.9`

Developed in Unreal Engine 5. "Unreal Engine" and its logo are trademarks or registered trademarks of Epic Games, Inc.

# FUTURE PLANS
• Generated targets - if the system does not detect a target, it will automatically generate its analogue to avoid softlock. Or if the ObjectiveDetails initially specifies that this target must be generated

• Generated quests. Adding the ability to generate quests from templates for the **OCG CONFRONTATION** and **MEASURE** types

• **GENERATED OBJECTIVES** Save & Load, in QuestSystem or in outer game systems

# WHAT'S NEW

`1.0.9`
Minor Update.

# TUTORIAL


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
