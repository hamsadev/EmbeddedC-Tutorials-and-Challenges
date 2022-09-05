#include <stdio.h>
#include <stdint.h>
#include <string.h>


typedef char (*CmpFn)(void* a, void* b, uint32_t len);

void memswap(void* a, void* b, uint32_t len);

char BIN_ASC(void* a, void* b, uint32_t len);
char BIN_DSC(void* a, void* b, uint32_t len);

char INT_ASC(void* a, void* b, uint32_t len);
char INT_DSC(void* a, void* b, uint32_t len);

void sort(void* array, uint32_t len, uint32_t itemLen, CmpFn cmp);

void printIntArray(int32_t* arr, uint32_t len);

int main()
{
    int32_t buff[10] = {1, 5, 9, -5, 7, 6, 4, 2, 8, 3};

    sort(buff, 10, sizeof(int32_t), INT_DSC);
    printIntArray(buff, 10);
}

void printIntArray(int32_t* arr, uint32_t len) {
    putchar('[');
    while (--len > 0) {
        printf("%d, ", *arr++);
    }
    printf("%d]\n", *arr);
}

void sort(void* array, uint32_t len, uint32_t itemLen, CmpFn cmp) {
    uint8_t* pArr = (uint8_t*) array;

    for (uint32_t i = 0; i < len; i++) {
        for (uint32_t j = i + 1; j < len; j++) {
            if (cmp(&pArr[i * itemLen], &pArr[j * itemLen], itemLen) > 0) {
                memswap(&pArr[i * itemLen], &pArr[j * itemLen], itemLen);
            }
        }
    }

}
void memswap(void* a, void* b, uint32_t len) {
    uint8_t temp;

    while (len-- > 0) {
        temp = *(uint8_t*) a;
        *(uint8_t*) a = *(uint8_t*) b;
        *(uint8_t*) b = temp;
        (uint8_t*) a++;
        (uint8_t*) b++;
    }
}
char INT_ASC(void* a, void* b, uint32_t len) {
    return *(int32_t*)a - *(int32_t*)b;
}
char INT_DSC(void* a, void* b, uint32_t len) {
    return (*(int32_t*)b - *(int32_t*)a);
}
char BIN_ASC(void* a, void* b, uint32_t len) {
    return memcmp(a, b, len);
}
char BIN_DSC(void* a, void* b, uint32_t len) {
    return memcmp(b, a, len);
}
