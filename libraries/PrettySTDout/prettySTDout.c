#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "prettySTDout.h"

// [/] is smallest pattern to search for, next smallest is [RED]
#define MIN_PATTERN_LENGTH 3
#define SUPPORTED_KEYWORDS 30

// #define DEBUG

int main() {
    char msg[] = "Escaped [[, [BOLD]bold[/], [ITALIC]italic[/], [UNDER]under[/], [STRIKE]strike[/], [DIM]dim[/], [ITALIC][UNDER]it-und[/]";

    printf("Original msg:\n | %s\n", msg);
    enrich(msg);
    printf("Enriched msg:\n | %s\n", msg);

    return 0;
}

void enrich(char* msg) {
    char beenOpened = 'F';
    unsigned int msgLength = strlen(msg);

    // pOri will be used for the iteration,
    // pNew will be where I put the new characters
    // (replacements will always be smaller then the pattern it relaces)
    unsigned int pOri = 0;
    unsigned int pNew = 0;

    const char* actions[] = {
        "/",
        "NUMBER",
        "BOLD",
        "ITALIC",
        "UNDER",
        "STRIKE",
        "DIM",
        "black",
        "Black",
        "BLACK",
        "blue",
        "Blue",
        "BLUE",
        "cyan",
        "Cyan",
        "CYAN",
        "green",
        "Green",
        "GREEN",
        "magenta",
        "Magenta",
        "MAGENTA",
        "red",
        "Red",
        "RED",
        "white",
        "White",
        "WHITE",
        "yellow",
        "Yellow",
        "YELLOW"
    };

    // Iterate over msg-string
    while (msg[pOri] != '\0') {

        // If '[' encountered, see if it is a pattern we need to replace
        if (pOri + MIN_PATTERN_LENGTH <= msgLength && msg[pOri] == '[') {

            int keyword;
            if (msg[pOri + 1] == '[') {
                // Escaped '['
                keyword = SUPPORTED_KEYWORDS;

            } else {
                // Find closing ']'
                unsigned int j = pOri + 1;
                while (msg[j] != '\0' && msg[j] != ']') {
                    j++;
                }
                // Enforce closing the brackets
                assert(j != msgLength);

                // Extract substring
                char substring[j - pOri];
                strncpy(substring, msg + pOri + 1, j - pOri - 1);
                substring[j - pOri - 1] = '\0';

                #ifdef DEBUG
                printf("\nFound keyword: %s\n", substring);
                #endif

                keyword = 0;
                while (strcmp(substring, actions[keyword]) != 0 && keyword < SUPPORTED_KEYWORDS) {
                    keyword++;
                }
                // Enforce valid keyword
                assert(strcmp(substring, actions[keyword]) == 0);

            }

            switch (keyword) {
                case SUPPORTED_KEYWORDS:
                    // [[
                    pOri++;
                    msg[pNew] = msg[pOri];
                    break;

                case 0:
                    // [/]: 0
                    assert(beenOpened == 'T');
                    strncpy(msg + pNew, "\e[0m", 4);
                    pOri += 2;
                    pNew += 3;
                    beenOpened = 'F';
                    break;

                case 1:
                    // [NUMBER]: 1;32
                    strncpy(msg + pNew, "\e[1;36m", 7);
                    pOri += 7;
                    pNew += 6;
                    beenOpened = 'T';
                    break;

                case 2:
                    // [BOLD]: 1
                    strncpy(msg + pNew, "\e[1m", 4);
                    pOri += 5;
                    pNew += 3;
                    beenOpened = 'T';
                    break;

                case 3:
                    // [ITALIC]: 3
                    strncpy(msg + pNew, "\e[3m", 4);
                    pOri += 7;
                    pNew += 3;
                    beenOpened = 'T';
                    break;

                case 4:
                    // [UNDER]: 4
                    strncpy(msg + pNew, "\e[4m", 4);
                    pOri += 6;
                    pNew += 3;
                    beenOpened = 'T';
                    break;

                case 5:
                    // [STRIKE]: 9
                    strncpy(msg + pNew, "\e[9m", 4);
                    pOri += 7;
                    pNew += 3;
                    beenOpened = 'T';
                    break;

                case 6:
                    // [DIM]: 2
                    strncpy(msg + pNew, "\e[2m", 4);
                    pOri += 4;
                    pNew += 3;
                    beenOpened = 'T';
                    break;

                case 7:
                    // [black]: 30
                    strncpy(msg + pNew, "\e[30m", 5);
                    pOri += 6;
                    pNew += 4;
                    beenOpened = 'T';
                    break;

                case 17:
                    // [GREEN]: 32
                    strncpy(msg + pNew, "\e[32m", 5);
                    pOri += 6;
                    pNew += 4;
                    beenOpened = 'T';
                    break;

                default:
                    break;
            }
        } else if (pOri > pNew) {
            msg[pNew] = msg[pOri];
        }


        pNew++;
        pOri++;

        #ifdef DEBUG
        printf("After %i/%i: '%s'\e[0m\n", pOri, msgLength, msg);
        #endif
    }

    if (pOri > pNew) {
        msg[pNew] = '\0';
    }

#ifdef DEBUG
    printf("After loop: %s\e[0m\n\n", msg);
#endif

    // Enforce closed
    assert(beenOpened == 'F');
}
