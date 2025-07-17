// © Skydream Interactive LLC 2020-2025

#pragma once

#include "CoreMinimal.h"
#include "QuestEnumerators.generated.h" 

/*        QUEST TYPE        */	
UENUM(BlueprintType)
enum class EQuestType : uint8
{
	MainTask UMETA(DisplayName = "Main Task"), // Main Quest. If Quest failed - GameOver
	SideTask UMETA(DisplayName = "Side Task"), // SideQuest. If Quest failed - GameOver
	Investigation UMETA(DisplayName = "Investigation"), // Investigation Quest.
	KeyData UMETA(DisplayName = "Important Data"), //Сведения о местанахождении важных данных
	Measure UMETA(DisplayName = "Measures"), // Меры (генерируемые задания)
	Ocg UMETA(DisplayName = "OCG Confrontation"), // Меры (генерируемые задания)
	Echo UMETA(DisplayName = "Echo of the Past") //эхо прошлого, провал - GameOver
};

/*        OBJECTIVE TYPE        */
UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	None UMETA(DisplayName = "None"), //Дефолтная категория которую невозможно выполнить
	LocEnter UMETA(DisplayName = "Location - Enter"), //Нужно войти в определённую локацию
	LocLeave UMETA(DisplayName = "Location - Leave"), //Нужно покинуть определённую локацию
	Kill UMETA(DisplayName = "Kill"), //Нужно убить или уничтожить персонажа или объект
	Stun UMETA(DisplayName = "Neutralization"), //Нужно НЕСМЕРТЕЛЬНО убить персонажа или объект
	Interact UMETA(DisplayName = "Interact & Collect"), //Нужно подобрать или повзаимодействовать с определённым предметом
	Dialogue UMETA(DisplayName = "Dialogue"), //Нужно поговорить с кем-то
	LookAt UMETA(DisplayName = "Look At") //Нужно посмотреть на определённую область
};

/*        OBJECTIVE COMPLETE TYPE        */
UENUM(BlueprintType)
enum class EObjectiveCompleteType : uint8
{
	Completed UMETA(DisplayName = "Objective Completed"),
	Ignored UMETA(DisplayName = "Objective Ignored"),
	Failed UMETA(DisplayName = "Objective Failed")
};

UENUM(BlueprintType)
enum class ETaskGiverType : uint8
{
	NewTask UMETA(DisplayName = "New Task"),
	Failed UMETA(DisplayName = "Task Failed"),
	Completed UMETA(DisplayName = "Task Completed")
};