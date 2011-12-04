#include <stdio.h>

void printing_first_method(int count, int *ages, char **names)
{
        int i = 0;

        // first way using indexing
        for (i = count - 1; i >= 0; i--) {
                printf("%s has %d years alive.\n",
                       names[i], ages[i]);
        }
}

void printing_second_method(int count, int *ages, char **names)
{
        int i = 0;
        // second way using pointers
        for (i = 0; i < count; ++i) {
                printf("%s is %d years old.\n",
                       *(names+i), *(ages+i));
        }
}

void printing_third_method(int count, int *ages, char **names)
{
        int i = 0;
        // third way, pointers are just arrys
        for (i = 0; i < count; ++i) {
                printf("%s is %d years old again.\n", names[i], ages[i]);
        }
}

void printing_fourth_method(int count, int *ages, char **names)
{
        int *cur_age;
        char **cur_name;
        // fourth way with pointers in a stupid complex way
        for (cur_name = names, cur_age = ages;
             (cur_age - ages) < count;
             cur_name++, cur_age++) {
                printf("%s lived %d years so far.\n",
                       *cur_name, *cur_age);
        }
}

void printing_fifth_method(int count, int *ages, char **names)
{
        int i = 0;
        for (i = 0; i < count; ++i) {
                printf("%s is %d years old. Addresses: %p %p\n", names[i], ages[i], (names+i), (ages+i));
        }
}

int main(int argc, char *argv[])
{
        // create two arrays we care about
        int ages[] = {23, 43, 12, 89, 2};
        char *names[] = {
                "Alan", "Frank",
                "Mary", "John", "Lisa"
        };

        int count = sizeof(ages) / sizeof(int);

        printing_first_method(count, ages, names);

        printf("---\n");

        printing_second_method(count, ages, names);

        printf("---\n");

        printing_third_method(count, ages, names);

        printf("---\n");

        printing_fourth_method(count, ages, names);

        printf("---\n");

        printing_fifth_method(count, ages, names);

        return 0;
}
