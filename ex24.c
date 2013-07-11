#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dbg.h"

#define MAX_DATA 100

typedef enum EyeColor {
        BLUE_EYES, GREEN_EYES, BROWN_EYES,
        BLACK_EYES, OTHER_EYES
} EyeColor;

const char *EYE_COLOR_NAMES[] = {
        "Blue", "Green", "Brown", "Black", "Other"
};

typedef struct Person {
        int age;
        char first_name[MAX_DATA];
        char last_name[MAX_DATA];
        EyeColor eyes;
        float income;
} Person;

size_t trimwhitespace(char *out, size_t len, const char *str)
{
        if(len == 0)
                return 0;

        const char *end;
        size_t out_size;

        // Trim leading space
        while(isspace(*str)) str++;

        if(*str == 0)  // All spaces?
        {
                *out = 0;
                return 1;
        }

        // Trim trailing space
        end = str + strlen(str) - 1;
        while(end > str && isspace(*end)) end--;
        end++;

        // Set output size to minimum of trimmed string length and buffer size minus 1
        out_size = (end - str) < len-1 ? (end - str) : len-1;

        // Copy trimmed string and add null terminator
        memcpy(out, str, out_size);
        out[out_size] = 0;

        return out_size;
}

int main(int argc, char *argv[])
{
        Person you = {.age = 0};
        int i = 0;
        char *in = NULL;
        char *buffer = malloc(sizeof(char) * MAX_DATA);

        printf("What's your First Name? ");
        in = fgets(you.first_name, MAX_DATA - 1, stdin);
        check(in != NULL, "Failed to read first name.");

        printf("What's your Last Name? ");
        in = fgets(you.last_name, MAX_DATA - 1, stdin);
        check(in != NULL, "Failed to read last name");

        printf("How old are you? ");
        in = fgets(buffer, MAX_DATA - 1, stdin);
        check(in != NULL, "Failed to read last name");
        you.age = atoi(buffer);

        printf("What color are your eyes:\n");
        for (i = 0; i <= OTHER_EYES; i++) {
                printf("%d) %s\n", i+1, EYE_COLOR_NAMES[i]);
        }
        printf(">");

        int eyes = -1;
        fscanf(stdin, "%d", &eyes);
        check((eyes - 1) <= OTHER_EYES && eyes > 0, "Do it right, that's not an option");
        you.eyes = eyes - 1;

        printf("How much do you make an hour? ");
        fscanf(stdin, "%f", &you.income);

        printf("----- RESULTS -----\n");

        printf("First Name: %s", you.first_name);
        printf("Last Name: %s", you.last_name);
        printf("Age: %d\n", you.age);
        printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
        printf("Income: %f\n", you.income);

        return 0;
error:
        return -1;
}

