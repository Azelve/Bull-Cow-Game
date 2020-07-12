// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

// struct FBullCowCount
// {
// 	int32 Bulls = 0;
// 	int32 Cows = 0;
// };

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void SetupGame();
	void EndGame();
	void ProcessGuess(const FString& Guess);
	bool IsIsogram(const FString& Word) const;
	TArray<FString> GetValidWords(const TArray<FString>& ListOfWords) const;
	void GetForca(const FString& Guess);
	// FBullCowCount GetBullCows(const FString& Guess) const;

	private:
	FString HiddenWord;
	int32 Lives;
	FString ForcaLetter;
	// FString ForcaWord;
	// Create a ForcaLetter with the letters already used
	bool bGameOver;
	TArray<FString> WordList;
	TArray<FString> ValidWordsList;
};
