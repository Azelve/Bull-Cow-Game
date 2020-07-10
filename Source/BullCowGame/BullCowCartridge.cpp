// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddeWordList.h"
// #include "Misc/FileHelper.h"
// #include "Misc/Paths.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    // Welcome the player
    PrintLine(TEXT("Welcome to Bull&Cows!"));

    // Loading Words At Runtime
    // const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    // FFileHelper::LoadFileToStringArray(WordList, *WordListPath);

    GetValidWords(Words);
    SetupGame();// Setting Up Game
    
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    // Clear the terminal
    ClearScreen();

    // if game is over then do ClearScreen() and SetupGame()
    if (bGameOver)
    {
        SetupGame();
    }
    else // Checking PlayerGuess
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = TEXT("Amorist"); // Set the Hidden Word / Array?P
    Lives = HiddenWord.Len(); // Set Lives
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Type in your guess and press enter..."));
    PrintLine(TEXT("You have %i chances."), Lives);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(FString Guess)
{
    // Check if the input is the same that the hidden word
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You guessed right, yaaay!!!!"));
        EndGame();
        return;
    }

    // Check the right number of letters, if not equal ask to guess again
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Your number of letters are not right...\nPlease guess again."));
        PrintLine(TEXT("The hidden word is %i letter long."), HiddenWord.Len());
        return;
    }

    // Check if it is not an isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again."));
        return;
    }

    
    if (--Lives == 0) // Decrement lives by one and check if are equal to zero
    {
        PrintLine(TEXT("Sorry, you lost all your chances."));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        PrintLine(TEXT("### GAME OVER ###"));
        EndGame();
        return;
    }
    
    // The number of bulls and cows
    PrintLine(TEXT("There's something wrong!\nPlease guess again."));
    PrintLine(TEXT("Chances: %i"), Lives);
}

bool UBullCowCartridge::IsIsogram(FString Word) const
{
    // Each letter is compare to another in order to know if is an isogram or not
    for (int32 i = 0; i < Word.Len(); i++)
    {
        for (int32 j = i+1; j < Word.Len(); j++) 
        {
            if (Word[i] == Word[j])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> WordList) const
{
    TArray<FString> ValidWords;

    for (int32 i = 0; i < WordList.Num(); i++)
    {
        if (WordList[i].Len() >= 4 && WordList[i].Len() <= 8 && IsIsogram(WordList[i]))
        {
            ValidWords.Emplace(WordList[i]);
        }
    }

    PrintLine(TEXT("The number of valid words is: %i"), ValidWords.Num());

    return ValidWords;
}