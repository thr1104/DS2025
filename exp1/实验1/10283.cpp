#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

// 定义复数结构体
typedef struct {
    double real;
    double imag;
} Complex;

// 函数原型声明
double modulus(Complex c);
void printComplex(Complex c);
void printArray(Complex arr[], int n);
Complex randomComplex();
Complex* generateRandomArray(int size);
void shuffle(Complex arr[], int n);
int find(Complex arr[], int n, Complex target);
void insert(Complex** arr, int* n, Complex c);
void deleteElement(Complex* arr, int* n, int index);
void uniquify(Complex* arr, int* n);
int compare(Complex a, Complex b);
void bubbleSort(Complex arr[], int n);
void merge(Complex arr[], int l, int m, int r);
void mergeSort(Complex arr[], int l, int r);
Complex* copyArray(Complex arr[], int n);
void reverseArray(Complex arr[], int n);
void performanceTest(Complex arr[], int n);
Complex* rangeSearch(Complex arr[], int n, double m1, double m2, int* resultSize);

// 复数的模
double modulus(Complex c) {
    return sqrt(c.real * c.real + c.imag * c.imag);
}

// 打印一个复数
void printComplex(Complex c) {
    if (c.imag >= 0)
        printf("%.2f+%.2fi", c.real, c.imag);
    else
        printf("%.2f%.2fi", c.real, c.imag);
}

// 打印复数数组
void printArray(Complex arr[], int n) {
    for (int i = 0; i < n; i++) {
        printComplex(arr[i]);
        printf(" ");
    }
    printf("\n");
}

// 随机生成复数（实部和虚部在 [-10, 10] 范围内）
Complex randomComplex() {
    Complex c;
    c.real = ((double)rand() / RAND_MAX) * 20 - 10;
    c.imag = ((double)rand() / RAND_MAX) * 20 - 10;
    return c;
}

// 生成随机复数向量
Complex* generateRandomArray(int size) {
    Complex* arr = (Complex*)malloc(size * sizeof(Complex));
    for (int i = 0; i < size; i++) {
        arr[i] = randomComplex();
    }
    return arr;
}

// 置乱：Fisher-Yates 洗牌算法
void shuffle(Complex arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Complex temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// 查找
int find(Complex arr[], int n, Complex target) {
    for (int i = 0; i < n; i++) {
        if (fabs(arr[i].real - target.real) < 1e-9 && fabs(arr[i].imag - target.imag) < 1e-9) {
            return i;
        }
    }
    return -1;
}

// 插入
void insert(Complex** arr, int* n, Complex c) {
    (*n)++;
    *arr = (Complex*)realloc(*arr, (*n) * sizeof(Complex));
    (*arr)[*n - 1] = c;
}

// 删除
void deleteElement(Complex* arr, int* n, int index) {
    if (index < 0 || index >= *n) return;
    for (int i = index; i < *n - 1; i++) {
        arr[i] = arr[i + 1];
    }
    (*n)--;
}

// 唯一化
void uniquify(Complex* arr, int* n) {
    int i, j;
    for (i = 0; i < *n; i++) {
        for (j = i + 1; j < *n;) {
            if (fabs(arr[i].real - arr[j].real) < 1e-9 && fabs(arr[i].imag - arr[j].imag) < 1e-9) {
                deleteElement(arr, n, j);
            } else {
                j++;
            }
        }
    }
}

// 比较函数：以模为基准，模相同时以实部为基准
int compare(Complex a, Complex b) {
    double modA = modulus(a), modB = modulus(b);
    if (fabs(modA - modB) < 1e-9) {
        if (fabs(a.real - b.real) < 1e-9) return 0;
        return a.real < b.real ? -1 : 1;
    }
    return modA < modB ? -1 : 1;
}

// 冒泡排序
void bubbleSort(Complex arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (compare(arr[j], arr[j + 1]) > 0) {
                Complex temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// 归并排序的 merge
void merge(Complex arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    Complex* left = (Complex*)malloc(n1 * sizeof(Complex));
    Complex* right = (Complex*)malloc(n2 * sizeof(Complex));

    for (int i = 0; i < n1; i++) left[i] = arr[l + i];
    for (int j = 0; j < n2; j++) right[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (compare(left[i], right[j]) <= 0) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }
    while (i < n1) arr[k++] = left[i++];
    while (j < n2) arr[k++] = right[j++];

    free(left);
    free(right);
}

// 归并排序
void mergeSort(Complex arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// 复制数组
Complex* copyArray(Complex arr[], int n) {
    Complex* newArr = (Complex*)malloc(n * sizeof(Complex));
    memcpy(newArr, arr, n * sizeof(Complex));
    return newArr;
}

// 逆序数组（用于构造逆序输入）
void reverseArray(Complex arr[], int n) {
    for (int i = 0; i < n / 2; i++) {
        Complex temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
}

// 性能测试：顺序、乱序、逆序
void performanceTest(Complex original[], int n) {
    // 先对原数组排序，得到顺序版本
    Complex* sorted = copyArray(original, n);
    mergeSort(sorted, 0, n - 1); // 使用归并排序得到顺序数组（稳定）

    // 构造三种输入
    Complex* asc = copyArray(sorted, n);          // 顺序
    Complex* desc = copyArray(sorted, n);
    reverseArray(desc, n);                        // 逆序
    Complex* random = copyArray(original, n);     // 乱序

    const char* cases[3] = {"顺序", "乱序", "逆序"};
    Complex* inputs[3] = {asc, random, desc};

    for (int idx = 0; idx < 3; idx++) {
        printf("\n--- %s输入 ---\n", cases[idx]);

        // 冒泡排序
        Complex* arr1 = copyArray(inputs[idx], n);
        clock_t start = clock();
        bubbleSort(arr1, n);
        clock_t end = clock();
        double time1 = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("冒泡排序时间: %.6f 秒\n", time1);
        free(arr1);

        // 归并排序
        Complex* arr2 = copyArray(inputs[idx], n);
        start = clock();
        mergeSort(arr2, 0, n - 1);
        end = clock();
        double time2 = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("归并排序时间: %.6f 秒\n", time2);
        free(arr2);
    }

    // 释放临时数组
    free(sorted);
    free(asc);
    free(desc);
    free(random);
}

// 区间查找
Complex* rangeSearch(Complex arr[], int n, double m1, double m2, int* resultSize) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        double mod = modulus(arr[i]);
        if (mod >= m1 && mod <= m2) {
            count++;
        }
    }

    Complex* result = (Complex*)malloc(count * sizeof(Complex));
    int idx = 0;
    for (int i = 0; i < n; i++) {
        double mod = modulus(arr[i]);
        if (mod >= m1 && mod <= m2) {
            result[idx++] = arr[i];
        }
    }
    *resultSize = count;
    return result;
}

// 主函数
int main() {
    srand((unsigned int)time(NULL));

    const int SIZE = 500; 
    Complex* arr = generateRandomArray(SIZE);
    int n = SIZE;

    printf("=== 1. 无序复数向量 ===\n");
    printArray(arr, n); 

    printf("\n=== 2. 置乱 ===\n");
    shuffle(arr, n);
    printArray(arr, n);

    printf("\n=== 3. 查找测试 ===\n");
    Complex target = {1.0, 1.0};
    int pos = find(arr, n, target);
    if (pos != -1) {
        printf("找到目标值，位置：%d\n", pos);
    } else {
        printf("未找到目标值\n");
    }

    printf("\n=== 4. 插入测试 ===\n");
    insert(&arr, &n, target);
    printArray(arr, n);

    printf("\n=== 5. 删除测试 ===\n");
    deleteElement(arr, &n, 0);
    printArray(arr, n);

    printf("\n=== 6. 唯一化 ===\n");
    uniquify(arr, &n);
    printArray(arr, n);

    printf("\n=== 7. 排序性能测试（顺序 / 乱序 / 逆序）===\n");
    performanceTest(arr, n);

    printf("\n=== 8. 区间查找测试 ===\n");
    double m1 = 1.0, m2 = 5.0;
    int resultSize;
    Complex* result = rangeSearch(arr, n, m1, m2, &resultSize);
    printf("模在 [%.1f, %.1f] 区间的元素有 %d 个\n", m1, m2, resultSize);
    printArray(result, resultSize);

    free(arr);
    free(result);

    return 0;
}
