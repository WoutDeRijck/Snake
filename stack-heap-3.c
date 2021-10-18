#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int* a = malloc(sizeof(int));
    *a = 10;
    printf("a=%d\n", *a);
    free(a);
    printf("a=%d\n", *a);

    for (int i = 0; i < 1000; i++) {
        int* b = malloc(sizeof(int));
        if (b != NULL) {
           *b = 5;
        }
    }

    printf("a=%d\n", *a);
    
    return 0;
}