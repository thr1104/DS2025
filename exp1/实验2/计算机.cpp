#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N_OPTR 9
typedef enum { ADD, SUB, MUL, DIV, POM, FAC, L_P, R_P, EOE } Operator;

// 优先级表：pri[栈顶][当前]
const char pri[N_OPTR][N_OPTR] = {
    /*       +     -     *     /     ^     !     (     )     # */
    /* + */   '<', '<', '>', '>', '>', '>', '<', '>', '=',
    /* - */   '<', '<', '>', '>', '>', '>', '<', '>', '=',
    /* * */   '<', '<', '<', '<', '>', '>', '<', '>', '=',
    /* / */   '<', '<', '<', '<', '>', '>', '<', '>', '=',
    /* ^ */   '<', '<', '<', '<', '<', '>', '<', '>', '=',
    /* ! */   '<', '<', '<', '<', '<', '=', '<', '>', '=',
    /* ( */   '<', '<', '<', '<', '<', '<', '<', '>', '=',
    /* ) */   '>', '>', '>', '>', '>', '>', '>', '=', '=',
    /* # */   '<', '<', '<', '<', '<', '<', '<', '<', '='
};

#define MAX_SIZE 100

// 操作数栈
double opnd_stack[MAX_SIZE];
int opnd_top = -1;

// 运算符栈
Operator optr_stack[MAX_SIZE];
int optr_top = -1;

// 栈操作函数
void push_opnd(double x) {
    if (opnd_top >= MAX_SIZE - 1) {
        fprintf(stderr, "Operand stack overflow!\n");
        exit(1);
    }
    opnd_stack[++opnd_top] = x;
}

double pop_opnd() {
    if (opnd_top == -1) {
        fprintf(stderr, "Operand stack underflow!\n");
        exit(1);
    }
    return opnd_stack[opnd_top--];
}

void push_optr(Operator op) {
    if (optr_top >= MAX_SIZE - 1) {
        fprintf(stderr, "Operator stack overflow!\n");
        exit(1);
    }
    optr_stack[++optr_top] = op;
}

Operator pop_optr() {
    if (optr_top == -1) {
        fprintf(stderr, "Operator stack underflow!\n");
        exit(1);
    }
    return optr_stack[optr_top--];
}

Operator top_optr() {
    if (optr_top == -1) return EOE;
    return optr_stack[optr_top];
}

// 字符转运算符
Operator char_to_op(char c) {
    switch(c) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POM;
        case '!': return FAC;
        case '(': return L_P;
        case ')': return R_P;
        default: return EOE;
    }
}

// 判断是否为数字字符（包括小数点）
int is_digit_char(char c) {
    return (c >= '0' && c <= '9') || c == '.';
}

// 读取数字（支持小数）
double read_number(const char* expr, int* pos) {
    char buf[100];
    int i = 0;
    while (is_digit_char(expr[*pos])) {
        buf[i++] = expr[(*pos)++];
    }
    buf[i] = '\0';
    return atof(buf);
}

// 阶乘函数（仅支持非负整数）
double factorial(double n) {
    if (n < 0 || n != (int)n) {
        fprintf(stderr, "Factorial error: must be non-negative integer\n");
        exit(1);
    }
    if (n == 0 || n == 1) return 1.0;
    double res = 1.0;
    for (int i = 2; i <= (int)n; i++) {
        res *= i;
    }
    return res;
}

// 表达式求值主函数
double evaluate_expression(char* expr) {
    int pos = 0;
    opnd_top = -1;
    optr_top = -1;
    push_optr(EOE); // 压入结束符 '#'

    while (1) {
        char c = expr[pos];
        if (c == '\0') c = '#'; // 表达式结束符

        // 读取数字
        if (is_digit_char(c)) {
            double num = read_number(expr, &pos);
            push_opnd(num);
            continue; // 跳过 pos++，因为 read_number 已推进 pos
        }

        Operator op = char_to_op(c);

        // 遇到结束符，退出主循环
        if (op == EOE && c == '#') break;

        // ? 关键修复：确保栈顶不是 EOE 再进行优先级比较和计算
        while (top_optr() != EOE && pri[top_optr()][op] == '<') {
            Operator op1 = pop_optr();
            if (op1 == FAC) {
                double val = pop_opnd();
                push_opnd(factorial(val));
            } else if (op1 == L_P) {
                fprintf(stderr, "Mismatched parentheses!\n");
                exit(1);
            } else {
                double b = pop_opnd();
                double a = pop_opnd();
                double res;
                switch(op1) {
                    case ADD: res = a + b; break;
                    case SUB: res = a - b; break;
                    case MUL: res = a * b; break;
                    case DIV:
                        if (b == 0) {
                            fprintf(stderr, "Division by zero!\n");
                            exit(1);
                        }
                        res = a / b; break;
                    case POM: res = pow(a, b); break;
                    default:
                        fprintf(stderr, "Unknown operator!\n");
                        exit(1);
                }
                push_opnd(res);
            }
        }

        // 处理右括号
        if (op == R_P) {
            while (top_optr() != L_P) {
                if (top_optr() == EOE) {
                    fprintf(stderr, "Mismatched parentheses!\n");
                    exit(1);
                }
                Operator op1 = pop_optr();
                if (op1 == FAC) {
                    double val = pop_opnd();
                    push_opnd(factorial(val));
                } else {
                    double b = pop_opnd();
                    double a = pop_opnd();
                    double res;
                    switch(op1) {
                        case ADD: res = a + b; break;
                        case SUB: res = a - b; break;
                        case MUL: res = a * b; break;
                        case DIV:
                            if (b == 0) {
                                fprintf(stderr, "Division by zero!\n");
                                exit(1);
                            }
                            res = a / b; break;
                        case POM: res = pow(a, b); break;
                        default:
                            fprintf(stderr, "Unknown operator!\n");
                            exit(1);
                    }
                    push_opnd(res);
                }
            }
            pop_optr(); // 弹出 '('
        } else {
            // 压入当前运算符（包括 '(' 和普通运算符）
            push_optr(op);
        }

        pos++; // 推进位置（数字已由 read_number 推进，此处只处理非数字）
    }

    // 处理剩余运算符（直到 EOE）
    while (top_optr() != EOE) {
        Operator op1 = pop_optr();
        if (op1 == L_P || op1 == R_P) {
            fprintf(stderr, "Mismatched parentheses!\n");
            exit(1);
        }
        if (op1 == FAC) {
            double val = pop_opnd();
            push_opnd(factorial(val));
        } else {
            double b = pop_opnd();
            double a = pop_opnd();
            double res;
            switch(op1) {
                case ADD: res = a + b; break;
                case SUB: res = a - b; break;
                case MUL: res = a * b; break;
                case DIV:
                    if (b == 0) {
                        fprintf(stderr, "Division by zero!\n");
                        exit(1);
                    }
                    res = a / b; break;
                case POM: res = pow(a, b); break;
                default:
                    fprintf(stderr, "Unknown operator!\n");
                    exit(1);
            }
            push_opnd(res);
        }
    }

    // 最终检查：操作数栈应只剩一个结果
    if (opnd_top != 0) {
        fprintf(stderr, "Expression error: invalid syntax\n");
        exit(1);
    }

    return pop_opnd();
}

// 主函数
int main() {
    char expr[256];

    printf("请输入表达式（支持 + - * / ^ ! ( )，例如: 1+1 或 (2+3)!）:\n");
    if (!fgets(expr, sizeof(expr), stdin)) {
        fprintf(stderr, "输入错误！\n");
        return 1;
    }

    // 移除换行符
    expr[strcspn(expr, "\n")] = '\0';

    // 清理空格
    char clean_expr[256];
    int j = 0;
    for (int i = 0; expr[i]; i++) {
        if (expr[i] != ' ') {
            clean_expr[j++] = expr[i];
        }
    }
    clean_expr[j] = '\0';

    if (j == 0) {
        printf("表达式为空。\n");
        return 0;
    }

    printf("表达式: %s\n", clean_expr);

    // 计算并输出结果
    double result = evaluate_expression(clean_expr);
    printf("结果: %.6f\n", result);

    return 0;
}
