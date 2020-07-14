// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

// Try to break lines in big names


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    // Welcome the player
    PrintLine(TEXT("Welcome to Bull&Cows!"));
    PrintLine(TEXT("Press tab to start typing."));
    PrintLine(TEXT("Type a letter per time to find the word."));
    
    // Loading Words At Runtime
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(WordList, *WordListPath);
    ValidWordsList = GetValidWords(WordList);

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
        ProcessGuess(PlayerInput.ToUpper());
    }
}

void UBullCowCartridge::SetupGame()
{
    FString hidden = ValidWordsList[FMath::RandRange(0, ValidWordsList.Num())]; // Set the Hidden Word«
    HiddenWord = hidden.ToUpper();
    Lives = HiddenWord.Len(); // Set Lives
    bGameOver = false;
    ForcaLetter = ""; // Clear the words used before

    PrintLine(TEXT("Word has %i letters!"), HiddenWord.Len());
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
        PrintLine(TEXT("You completed the word, yaaay!!!!"));
        EndGame();
        return;
    }

    // Check if the number of letters are > 0
    if (Guess.Len() == 0 || Guess.Len() > 1)
    {
        PrintLine(TEXT("Please input a letter per time...\nOr the whole word!"));
        // PrintLine(TEXT("The hidden word is %i letter long."), HiddenWord.Len());
        return;
    }

    // Check if the ForcaLetter has the guess letter
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("What do you call the feeling that you have\nheard this letter before?\nDeja-moo!"));
        return;
    }

    if (GetForca(Guess))
    {
        bNoEmpty = false;
        return;
    }
    
    
    
    if (--Lives == 0) // Decrement lives by one and check if are equal to zero
    {
        PrintLine(TEXT("\nCowtastrophic, you lost all your chances."));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        PrintLine(TEXT("### GAME OVER ###"));
        EndGame();
        return;
    }
    
    // The number of bulls and cows
    // FBullCowCount Score = GetBullCows(Guess);

    // PrintLine(TEXT("\nYou have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    if (!bWinGame)
    {
        PrintLine(TEXT("\nThe word doesn't have this letter, sorry.\nPlease guess again."));
        PrintLine(TEXT("Chances: %i"), Lives);
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    // Each letter is compare to another in order to know if the guess letter was already used
    for (int32 i = 0; i < ForcaLetter.Len(); i++)
    {
        // for (int32 j = i+1; j < Word.Len(); j++) 
        // {
            if (Word[0] == ForcaLetter[i])
            {
                return false;
            }
        // }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& ListOfWords) const
{
    TArray<FString> ValidWords;

    for (FString Word : ListOfWords)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8)
        {
            ValidWords.Emplace(Word);
        }
    }
    // PrintLine(TEXT("%i"), FMath::RandRange(0, ValidWords.Num()));
    // PrintLine(TEXT("The number of valid words is: %i"), ValidWords.Num());

    return ValidWords;
}

bool UBullCowCartridge::GetForca(const FString& Guess)
{
    bool bEqual = false;
    FString ForcaWord;
    FString Space = " ";
    ForcaLetter += Guess[0];

    // Passe por cada letra do hidden word e veja se já existe no "banco de dados" de letras usadas
    for (int32 ii = 0; ii < HiddenWord.Len(); ii++)
    {
        // Para cada letra dentro do "banco de dados" check if is equal to the current hidden word letter index
        FString Letter;
        Letter += HiddenWord[ii];
        for (int32 i = 0; i < ForcaLetter.Len(); i++)
        {
            // If our ForcaLetter letter is equal to the hidden word add into the Forca/ForcaWord
            if (ForcaLetter[i] == HiddenWord[ii])
            {                
                ForcaWord += Letter;
                ForcaWord += " ";
                bEqual = true; // It is true that exist a equal letter in that index
                break;
            }
        }

        if (Guess[0] == HiddenWord[ii])
        {
            bNoEmpty = true;
        } 

        if (HiddenWord[ii] == Space[0])
        {   
            Letter = "-";
            // Forca.Emplace(Letter);
            ForcaWord += Letter;
            ForcaWord += " ";
            continue;
        }

        // If doesn't exists a equal letter in the current index will have a vague space
        if (!bEqual)
        {
            Letter = "_";
            // Forca.Emplace(Letter);
            ForcaWord += Letter;
            ForcaWord += " ";
        }      

        bEqual = false; // Set false 'cause the next index we don't know yet if exists a equal letter
    }
    // PrintLine(TEXT("%s"), *Letter);
    PrintLine(TEXT("%s"), *ForcaWord);
    WinProcess(ForcaWord);
    ForcaWord = "";
    
    if (bNoEmpty && !bWinGame)
    {
        PrintLine(TEXT("\nYou guessed right the letter."));
    }
    return bNoEmpty;
}

void UBullCowCartridge::WinProcess(const FString& Letters)
{
    FString Empty = "_";
    for (int32 i = 0; i < Letters.Len(); i++)
    {
        if (Letters[i] == Empty[0])
        {
            // PrintLine(TEXT("%s"), *Letters);
            // PrintLine(TEXT("%s"), *Empty);
            bWinGame = false;
            return;
        }
    }
    bWinGame = true;
    PrintLine(TEXT("\nYou completed the word, yaaay!!!!"));
    EndGame();
}




// FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
// {
//     FBullCowCount Count;
//     TArray<FString> Forca;
//     FString ForcaWord;

//     for(int32 i = 0; i < Guess.Len(); i++)
//     {
//         Forca.Emplace("_");
//         ForcaWord += Forca[i];
//         if (Guess[i] == HiddenWord[i])
//         {
//             Count.Bulls++;
//             continue;
//         }

//         for (int32 ii = 0; ii < HiddenWord.Len(); ii++)
//         {
//             if (Guess[i] == HiddenWord[ii])
//             {
//                 Count.Cows++;
//                 break;
//             }
//         }
//     }
//     PrintLine(TEXT("%s"), *Forca[0]);
//     PrintLine(TEXT("%s"), *ForcaWord);
//     return Count;
// }