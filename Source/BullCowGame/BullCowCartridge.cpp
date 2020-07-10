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
    Isograms = GetValidWords(Words);

    SetupGame();// Setting Up Game

    
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
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
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num())]; // Set the Hidden Word
    Lives = HiddenWord.Len(); // Set Lives
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Type in your guess and press enter..."));
    PrintLine(TEXT("The hidden word is: %s"), *HiddenWord);
    PrintLine(TEXT("You have %i chances."), Lives);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
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
    int32 Bulls, Cows;
    GetBullCows(Guess, Bulls, Cows);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);
    PrintLine(TEXT("There's something wrong!\nPlease guess again."));
    PrintLine(TEXT("Chances: %i"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
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

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    // PrintLine(TEXT("%i"), FMath::RandRange(0, ValidWords.Num()));
    // PrintLine(TEXT("The number of valid words is: %i"), ValidWords.Num());

    return ValidWords;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for(int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            BullCount++;
            continue;
        }

        for (int32 ii = 0; ii < HiddenWord.Len(); ii++)
        {
            if (Guess[i] == HiddenWord[ii])
            {
                CowCount++;
                break;
            }
        }
    }
}