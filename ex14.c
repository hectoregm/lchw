#include <stdio.h>
#include <ctype.h>
#include <string.h>

// forward declarations

void print_letters(size_t slen, char arg[]);

void print_arguments(int argc, char *argv[])
{
        int i = 0;
        for (i = 0; i < argc; ++i) {
                size_t slen = strlen(argv[i]);
                print_letters(slen, argv[i]);
        }
}

void print_letters(size_t slen, char arg[])
{
        int i = 0;
        for (i = 0; i < slen; ++i) {
                char ch = arg[i];

                if (isalpha(ch) || isblank(ch)) {
                        printf("'%c' == %d ", ch, ch);
                }
        }

        printf("\n");
}

int main(int argc, char *argv[])
{
        print_arguments(argc, argv);
        return 0;
}

