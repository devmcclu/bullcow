// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    ClearScreen();

    // If the game is over then ClearScreen() and SetupGame()
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Check player guess
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    //Welcome the player
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess.\nPress enter to continue.")); //Prompt the Player for guess    
    PrintLine(TEXT("The Hidden Word is: %s."), *HiddenWord); // Debug Line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{   
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You have won!"));
        EndGame();
        return;
    }

    // Check if right number of characters
    // Prompt to guess again
    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The Hidden Word is %i letters long."), HiddenWord.Len());
        PrintLine(TEXT("You have %i lives remaining. Guess again!"), Lives);
        return;
    }

    // Check if Isogram
    // Prompt to guess again
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("Not an isogram, guess again!"));
        PrintLine(TEXT("You have %i lives remaining."), Lives);
        return;
    }

    // Remove life
    --Lives;

    // Are the lives greater than 0?
    // If no, show GameOver and HiddenWord
    // Prompt to PlayAgain, Press Enter?
    if(Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have lost!"));
        PrintLine(TEXT("The hidden word was %s."), *HiddenWord);
        EndGame();
        return;
    }

    //Show the Player bulls and cows
    int32 Bulls, Cows;
    GetBullCows(Guess, Bulls, Cows);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);
    // If yes GuessAgain
    // Show lives left
    PrintLine(TEXT("You have lost a life!"));
    PrintLine(TEXT("You now have %i lives."), Lives);
    PrintLine(TEXT("Guess again!"));
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len() - 1; Index++)
    {
        for (int32 Comparision = Index + 1; Comparision < Word.Len(); Comparision++)
        {
            if (Word[Index] == Word[Comparision])
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
    return ValidWords;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    // For every index of Guess is same as index of the hidden word, BullCount++
    // If not a bull was it a cow> if yes CowCount++

    for (int32 Index = 0; Index < Guess.Len(); Index++)
    {
        if (Guess[Index] == HiddenWord[Index])
        {
            BullCount++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if(Guess[Index] == HiddenWord[HiddenIndex])
            {
                CowCount++;
                break;
            }
        }
    }
}