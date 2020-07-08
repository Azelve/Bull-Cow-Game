// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    // Welcome the player
    PrintLine(TEXT("Welcome to Bull&Cows!"));

    SetupGame();// Setting Up Game

}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    // Clear the terminal
    ClearScreen();

    // if game is over then do ClearScreen() and SetupGame()
    if (bGameOver == true)
    {
        EndGame();
    }
    // else Checking PlayerGuess
    else
    {
        // Check if the input is the same that the hidden word
        if (Input == HiddenWord)
        {
            PrintLine(TEXT("You guessed right, yaaay!!!!"));
            PrintLine(TEXT("Press enter to continue..."));
            bGameOver = true;
        }
        else
        {
            // Check the right number of letters, if not equal ask to guess again
            if (Input.Len() != HiddenWord.Len())
            {
                PrintLine(TEXT("Your number of letters are not right...\nPlease guess again."));
                return;
            }

            Lives --;
            
            if (Lives == 0)
            {
                PrintLine(TEXT("Sorry, you lost all your chances."));
                PrintLine(TEXT("The word is: %s"), *HiddenWord);
                PrintLine(TEXT("Press enter to continue..."));
                PrintLine(TEXT("### GAME OVER ###"));
                bGameOver = true;
            }
            else
            {
                PrintLine(TEXT("There's something wrong!\nPlease guess again."));
                PrintLine(TEXT("Chances: %i"), Lives);
            }
        }
    }
        
        // Array???
        // Check if it is an isogram, if no ask to guess again

        // Remove the life

        // Function???
        // Check if lives > 0
        // If yes guess again and show lives left
        // If no show GameOVer and the hidden word

        // Ask if the player want to play again, if no quit the game
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = TEXT("Algorism"); // Set the Hidden Word / Array?
    Lives = 3; // Set Lives
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Type in your guess and press enter..."));
    PrintLine(TEXT("Chances: %i"), Lives);
}

void UBullCowCartridge::EndGame()
{
    SetupGame();
}