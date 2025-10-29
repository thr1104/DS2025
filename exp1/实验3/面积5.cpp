#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 计算最大矩形面积（单调栈）
int largestRectangleArea(int* heights, int n) {
    if (n == 0) return 0;

    int* stack = (int*)malloc((n + 1) * sizeof(int));
    int top = -1;
    int max_area = 0;

    for (int i = 0; i <= n; i++) {
        int current_height = (i == n) ? 0 : heights[i];

        while (top != -1 && heights[stack[top]] > current_height) {
            int h = heights[stack[top--]];
            int w = i;
            if (top != -1) {
                w -= stack[top] + 1;
            }
            int area = h * w;
            if (area > max_area) max_area = area;
        }

        stack[++top] = i;
    }

    free(stack);
    return max_area;
}

// 生成“非大面积相同”的随机高度数组
void generate_diverse_heights(int* heights, int len) {
    const int MAX_HEIGHT = 104;
    const int MAX_SAME_RETRY = 3; // 最多重试3次避免相同

    for (int i = 0; i < len; i++) {
        int attempts = 0;
        int val;
        do {
            val = rand() % (MAX_HEIGHT + 1);
            attempts++;
        } while (attempts < MAX_SAME_RETRY && i > 0 && val == heights[i - 1]);

        heights[i] = val;
    }
}

// 打印数组
void print_array(int* arr, int len) {
    printf("[");
    for (int i = 0; i < len; i++) {
        printf("%d", arr[i]);
        if (i < len - 1) printf(", ");
    }
    printf("]");
}

// 测试单个案例
void test_case(int* heights, int len) {
    printf("输入: ");
    print_array(heights, len);
    printf("\n输出: %d\n", largestRectangleArea(heights, len));
    printf("\n");
}

// 主函数
int main() {
    const int TEST_COUNT = 10;
    const int MAX_LENGTH = 105;

    srand((unsigned int)time(NULL)); // 种子只设一次

    printf("=== 柱状图最大矩形面积 —— 10组多样随机测试 ===\n\n");

    for (int t = 0; t < TEST_COUNT; t++) {
        int len = 1 + rand() % MAX_LENGTH; // 长度 1~105
        int* heights = (int*)malloc(len * sizeof(int));

        generate_diverse_heights(heights, len);

        test_case(heights, len);

        free(heights);
    }

    // 手动验证经典案例
    printf("=== 手动验证案例 ===\n");
    int ex1[] = {2, 1, 5, 6, 2, 3};
    test_case(ex1, 6);

    int ex2[] = {2, 4};
    test_case(ex2, 2);

    return 0;
}
