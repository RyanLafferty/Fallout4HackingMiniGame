/*Ryan Lafferty*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define DICT_LOCATION "enable1.txt"

void game(int difficulty, char * word);
char * pickWordLen(char * dictLocation, int wordLen);
int checkGuess(char * word, char * guess);

int main()
{
    char * word;
    char * input;
    char buffer[1000];
    int choice;
    int wordLen;

    word = NULL;
    input = NULL;
    choice = 0;
    wordLen = 0;

    printf("Difficulty (1-5)?: ");
    input = fgets(buffer, 1000, stdin);

    if(input == NULL)
    {
        printf("Bad value, difficulty set to 1\n");
        choice = 1;
    }
    else
    {
        choice = atoi(input);
        if(choice > 5 || choice < 1)
        {
            printf("Bad value, difficulty set to 1\n");
            choice = 1;
        }
    }

    switch(choice)
    {
        case 1:
        {
            wordLen = 4;
            break;
        }
        case 2:
        {
            wordLen = 8;
            break;
        }
        case 3:
        {
            wordLen = 10;
            break;
        }
        case 4:
        {
            wordLen = 12;
            break;
        }
        case 5:
        {
            wordLen = 15;
            break;
        }
        default:
        {
            wordLen = 4;
            break;
        }
    }
    wordLen += 2; //handle's dictionary format

    word = pickWordLen(DICT_LOCATION, wordLen);
    game(choice, word);

    return 0;
}

//game method, gets random word and word list then starts the fallout 4 hacking game
void game(int difficulty, char * word)
{
    int guesses;
    int position;
    int wordCount;
    int matches;
    int len;
    int i;
    char buffer[1000];
    char * returnVal;
    char ** wordList;
    time_t cTime;

    guesses = 4;
    position = 0;
    wordCount = 0;
    matches = 0;
    len = 0;
    i = 0;
    returnVal = NULL;
    wordList = NULL;
    cTime = time(NULL);

    if(difficulty > 5 || difficulty < 1)
    {
        fprintf(stderr, "Error: Bad difficulty value!\n");
        free(word);
        return;
    }
    if(word == NULL)
    {
        fprintf(stderr, "Error: No word given!\n");
        free(word);
        return;
    }

    switch(difficulty)
    {
        case 1:
        {
            wordCount = 5;
            break;
        }
        case 2:
        {
            wordCount = 8;
            break;
        }
        case 3:
        {
            wordCount = 10;
            break;
        }
        case 4:
        {
            wordCount = 12;
            break;
        }
        case 5:
        {
            wordCount = 15;
            break;
        }
        default:
        {
            wordCount = 5;
            break;
        }
    }

    len = strlen(word);
    len -= 2; //handle's dictionary format

    wordList = malloc(sizeof(char *) * wordCount);
    if(wordList == NULL)
    {
        free(word);
        fprintf(stderr, "Error: Failed to allocate memory for word list!\n");
        return;
    }

    for(i = 0; i < wordCount; i++)
    {
        wordList[i] = NULL;
    }

    position = (rand() % wordCount);
    wordList[position] = word;

    for(i = 0; i < wordCount; i++)
    {
        if(wordList[i] == NULL)
        {
            wordList[i] = pickWordLen(DICT_LOCATION, strlen(word));
        }
    }

    for(i = 0; i < wordCount; i++)
    {
        if(wordList[i] != NULL)
        {
            printf("%s", wordList[i]);
        }
    }

    while(guesses > 0)
    {
        printf("\nGuess (%d left)? ", guesses);
        returnVal = fgets(buffer, 1000, stdin);
        if(returnVal != NULL)
        {
            matches = checkGuess(word, returnVal);
            if(matches == len)
            {
                printf("You win!\n");
                guesses = 0;
            }
            else
            {
                guesses--;
                if(guesses == 0)
                {
                    printf("You lose!\n");
                }
            }
        }
    }

    for(i = 0; i < wordCount; i++)
    {
        if(wordList[i] != NULL)
        {
            free(wordList[i]);
        }
    }
    free(wordList);
}

//generates a random word from a specified dictionary of length wordLen
char * pickWordLen(char * dictLocation, int wordLen)
{
    FILE * dict;
    char * word;
    char * returnVal;
    char buffer[1000];
    int val;
    int nVal;
    int len;
    int cTime;

    dict = NULL;
    word = NULL;
    returnVal = NULL;
    val = 0;
    nVal = 0;
    len = 0;

    if(dictLocation == NULL)
    {
        fprintf(stderr, "Error: No Dict given!\n");
        return NULL;
    }

    dict = fopen(dictLocation, "r");
    if(dict == NULL)
    {
        fprintf(stderr, "Error: Could not open dict!\n");
        return NULL;
    }

    cTime = (int) clock();
    srand(cTime);

    returnVal = fgets(buffer, 1000, dict);
    if(returnVal != NULL)
    {
        word = strdup(returnVal);
        len = strlen(returnVal);
        if(len == wordLen)
        {
            val = (rand() % RAND_MAX); 
        }
        else
        {
            val = 0;
        }
    }


    while(returnVal != NULL)
    {
        nVal = (rand() % RAND_MAX);
        returnVal = fgets(buffer, 1000, dict);

        if(nVal >= val && returnVal != NULL && strlen(returnVal) == wordLen)
        {
            val = nVal;

            free(word);
            word = strdup(returnVal);
        }
    }

    return word;
}

//checks a user entered guess.
int checkGuess(char * word, char * guess)
{
    int i;
    int j;
    int matches;
    int len;
    int matched[(strlen(word) - 2)];
    int matchedJ[(strlen(word) - 2)];
    char ch;

    i = 0;
    j = 0;
    matches = 0;
    len = 0;
    ch = 0;

    if(word == NULL)
    {
        fprintf(stderr, "Error: No word given to checkGuess!\n");
        return -1;
    }
    if(guess == NULL)
    {
        fprintf(stderr, "Error: No guess word given to checkGuess!\n");
        return -1;
    }

    len = strlen(word);
    len -= 2; //handle's dictionary format

    for(i = 0; i < len; i++)
    {
        matched[i] = 0;
        matchedJ[i] = 0;
    }

    for(i = 0; i < len; i++)
    {
        for(j = 0; j < len; j++)
        {
            ch = tolower(guess[i]);
            //printf("%d: %c %c\n", matched[j], ch, word[j]);
            if(ch == word[j] && matched[i] != 1 && matchedJ[j] != 1)
            {
                matches++;
                matched[i] = 1;
                matchedJ[j] = 1;
                j = len;
            }
        }
    }

    printf("%d/%d correct\n", matches, len);

    return matches;
}