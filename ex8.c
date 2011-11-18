#include <stdio.h>

int main(int argc, char *argv[])
{
        int areas[] = {10,12,13,14,20};
        char name[] = "Zed";
        char full_name[] = {
                'Z', 'e', 'd',
                ' ', 'A', '.', ' ',
                'S','h','a','w', '\0'
        };

        printf("The size of an int: %ld\n", sizeof(int));
        printf("The size of areas (int[]): %ld\n",
               sizeof(areas));
        printf("The number of ints in areas: %ld\n",
               sizeof(areas) / sizeof(int));
        areas[0] = 1000;
        areas[1] = -27;
        printf("The first area is %d, the 2nd %d.\n",
               areas[0], areas[1]);

        printf("The size of a char: %ld\n", sizeof(char));
        printf("The size of name (char []): %ld\n",
               sizeof(name));
        printf("The number of chars: %ld\n",
               sizeof(name) / sizeof(char));

        printf("The size of full name (char []): %ld\n",
               sizeof(full_name));
        printf("The number of chars: %ld\n",
               sizeof(full_name) / sizeof(char));
        name[0] = 'N';
        full_name[0] = 'N';
        printf("name=\"%s\" and full name=\"%s\"\n",
               name, full_name);
        return 0;
}
