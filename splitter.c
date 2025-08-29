#pragma once
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

    while ((c = getc(stdin)) != EOF && counter < MAX_READ-1) {
        input[counter] = (char)c;
        counter++;
    }
    input[counter] = '\0';
    
    for (unsigned int j = 0; j < counter; j++) {
        if (separators[j] == input[counter-1]) {
            sepended = 1;
            break;
        }
    }
    if (sepended != 1 && counter > 0) { // if there is in fact
        input[counter-1] = '.';
        sepended = 1;
    }

    PurifyString(input);
    SplitString(input);

    return 0;
}