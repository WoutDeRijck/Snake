#include <stdio.h>

int* getNum1() {
    int i = 10;
    int* i_ptr = &i;
    printf("i_ptr=%p\n", i_ptr);
    return i_ptr;
}

int getNum2() {
    int j = 20;
    int* j_ptr = &j;
    printf("j_ptr=%p\n", j_ptr);
    return j;
}

int getNum3() {
    int k = 30;
    int l = 40;
    int* k_ptr = &k;
    int* l_ptr = &l;
    printf("k_ptr=%p\n", k_ptr);
    printf("l_ptr=%p\n", l_ptr);
    return 50;
}

int main() {
    int* num1_ptr = getNum1();
    printf("num1_ptr=%p\n", num1_ptr);
    printf("num1=%d\n", *num1_ptr);
    printf("\n");

    int num2 = getNum2();
    printf("num1=%d\n", *num1_ptr);
    printf("num2=%d\n", num2);
    printf("\n");

    int num3 = getNum3();
    printf("num1=%d\n", *num1_ptr);
    printf("num2=%d\n", num2);
    printf("num3=%d\n", num3);
    
    return 0;
}