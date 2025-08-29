#pragma once

typedef struct {
    int count; // Amount of substrings returned
    char **substrings; // Substrings returned
} splitReturn;

/* Making sure that no unwanted characters are passed on. */
void PurifyString(char *str);

/* Splitting input string in sub-strings. */
splitResult SplitString(char *str);