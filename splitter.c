/*
Trying to build a string splitter that takes 1 input and returns sub-strings based off symbols used.
In main() we ask for the input, then send to PurifyString(), that removes any char before 0x20 and
after 0x7E, replacing them with a blank space.
After that, the purified string is sent over to SplitString(), that checks where '.', '!' and '?' are
placed in the sentence, and splits the original string in such a way to create a string from the
last split point to the current separator symbol.
I don't know why it doesn't work.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for future dynamic allocation shit

// https:////www.youtube.com//watch?v=Zr93W_omd4M        ---> just in case

#define MAX_READ 512
#define SEPARATORS_COUNT 3

char separators[SEPARATORS_COUNT] = {'.', '!', '?'};

// used in main() to track how many actual chars the string was
unsigned int counter = 0;

/* Making sure that no unwanted characters are passed on. */
void PurifyString(char *str) {
    for (unsigned int i = 0; i < counter; i++) {
        if (!(str[i] >= 0x20 && str[i] <= 0x7E)) {
            str[i] = ' ';
        }
    }
}

typedef struct {
    unsigned int count; // Amount of substrings returned
    char **substrings; // Substrings returned
} splitResult;

/* Splitting input string in sub-strings. */
splitResult SplitString(char *str) {
    unsigned int last = 0; // last string-splitting-point
    char **substrings = NULL;
    unsigned int substrings_count = 0;
    
    for (unsigned int i = 0; str[i] != '\0'; i++) {
        if (i == last) continue;

        for (unsigned int j = 0; j < SEPARATORS_COUNT; j++) {
            if (str[i] == separators[j]) {
                char *substring = (char *)malloc((i-last+1)*sizeof(char));
                if (!substring) {
                    printf("Malloc failed. Continuing execution.\n");
                    continue;
                }
                memcpy(substring, str+last, i-last);
                substring[i-last] = '\0';

                char **temp = realloc(substrings, (substrings_count+1)*sizeof(char *));
                if (!temp) {
                    printf("Holy shit, realloc failed. Continuining execution.\n");
                    continue;
                }
                substrings = temp;
                substrings[substrings_count] = substring;

                substrings_count++;
                last = i+1;

                break;
            }
        }
    }

    splitResult result;
    result.count = substrings_count;
    result.substrings = substrings;
    return result;
}

int c;
char input[MAX_READ];
unsigned int sepended = 0; // used to check whether the input string terminates with a separator 
int main(void) {
    printf("Input a string. It will be separated in sub-strings based off\
the graphical signs in your sentence(s): '.', '!', '?'\n*Note: some characters are not permitted.\n");

    // reading input; adding a null-terminator
    while ((c = getc(stdin)) != EOF && counter < MAX_READ-1) {
        printf("%c", (char)c);
        input[counter] = (char)c;
        counter++;
    }
    input[counter] = '\0';
    printf("%s", input);
    
    // checking for a final symbol, to close the sub-string.
    for (unsigned int j = 0; j < SEPARATORS_COUNT; j++) {
        if (separators[j] == input[counter-1]) {
            sepended = 1;
            break;
        }
    }

    // if there is no final symbol and there are more than 0 elements:
    if (sepended != 1 && counter > 0) {
        input[counter] = '.';
        input[counter+1] = '\0'; // shifted all the shit to the right
        counter++;
        sepended = 1;
    }

    PurifyString(input);
    splitResult result = SplitString(input);
    if (result.count > 0) {
        for (unsigned int i = 0; i < result.count; i++) {
            printf("String #%u: %s\n", i, result.substrings[i]);
        }
    } else {
        printf("Couldn't find any sub-string.");
    }

    return 0;
}