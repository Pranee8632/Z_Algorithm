#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getZarr(const char *str, int Z[]);

// prints all occurrences of pattern in text using Z algo
void search(const char *text, const char *pattern) {
    // Calculate lengths
    int textLen = strlen(text);
    int patternLen = strlen(pattern);

    // Create concatenated string "P$T"
    char *concat = (char *)malloc((textLen + patternLen + 2) * sizeof(char));
    if (concat == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    strcpy(concat, pattern);
    strcat(concat, "$");
    strcat(concat, text);
    int concatLen = strlen(concat);

    // Construct Z array
    int *Z = (int *)malloc(concatLen * sizeof(int));
    if (Z == NULL) {
        printf("Memory allocation failed!\n");
        free(concat);
        return;
    }

    getZarr(concat, Z);

    // now looping through Z array for matching condition
    int i;
    for(i = 0; i < concatLen; ++i) {
        // if Z[i] (matched region) is equal to pattern
        // length we got the pattern
        if (Z[i] == patternLen) {
            printf("Pattern found at index %d\n", i - patternLen - 1);
        }
    }

    // Free dynamically allocated memory
    free(concat);
    free(Z);
}

// Fills Z array for given string str[]
void getZarr(const char *str, int Z[]) {
    int n = strlen(str);
    int L, R, k;

    // [L,R] make a window which matches with prefix of s
    L = R = 0;
    int i;
    for (i = 1; i < n; ++i) {
        // if i>R nothing matches so we will calculate.
        // Z[i] using naive way.
        if (i > R) {
            L = R = i;

            // R-L = 0 in starting, so it will start
            // checking from 0'th index. For example,
            // for "ababab" and i = 1, the value of R
            // remains 0 and Z[i] becomes 0. For string
            // "aaaaaa" and i = 1, Z[i] and R become 5
            while (R < n && str[R - L] == str[R]) {
                R++;
            }
            Z[i] = R - L;
            R--;
        } else {
            // k = i-L so k corresponds to number which
            // matches in [L,R] interval.
            k = i - L;

            // if Z[k] is less than remaining interval
            // then Z[i] will be equal to Z[k].
            // For example, str = "ababab", i = 3, R = 5
            // and L = 2
            if (Z[k] < R - i + 1) {
                Z[i] = Z[k];
            } else {
                // For example str = "aaaaaa" and i = 2, R is 5,
                // L is 0
                // else start from R and check manually
                L = i;
                while (R < n && str[R - L] == str[R]) {
                    R++;
                }
                Z[i] = R - L;
                R--;
            }
        }
    }
}

int main() {
    FILE *textFile = fopen("text.txt", "r");
    FILE *patternFile = fopen("pattern.txt", "r");
    if (textFile == NULL || patternFile == NULL) {
        printf("Error opening files!\n");
        return 1;
    }

    // Read text
    char text[1000]; // Adjust size accordingly
    if (fgets(text, sizeof(text), textFile) == NULL) {
        printf("Error reading text file!\n");
        fclose(textFile);
        fclose(patternFile);
        return 1;
    }
    if (text[strlen(text) - 1] == '\n') {
        text[strlen(text) - 1] = '\0'; // Remove newline character
    }

    // Read pattern
    char pattern[100]; // Adjust size accordingly
    if (fgets(pattern, sizeof(pattern), patternFile) == NULL) {
        printf("Error reading pattern file!\n");
        fclose(textFile);
        fclose(patternFile);
        return 1;
    }
    if (pattern[strlen(pattern) - 1] == '\n') {
        pattern[strlen(pattern) - 1] = '\0'; // Remove newline character
    }

    fclose(textFile);
    fclose(patternFile);

    search(text, pattern);
    return 0;
}

