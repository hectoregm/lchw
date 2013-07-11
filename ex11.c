#include <stdio.h>

int main(int argc, char *argv[])
{
        // go through each string in argv
        
        int i = argc;
        while (i > 0) {
                printf("arg %d: %s\n", argc - i, argv[argc - i]);
                i--;
        }

        // let's make our own array of strings
        char *states[argc - 1];
        for (i = 1; i < argc; ++i) {
                states[i] = argv[i];
        }

        i = 1;
        argv[3][0] = 'V';
        argv[3][1] = 'i';
        while (i < argc) {
                printf("state %d: %s\n", i, states[i]);
                i++;
        }

        return 0;
}
