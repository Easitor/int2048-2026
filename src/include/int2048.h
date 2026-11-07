#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减

// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

// 请不要使用 using namespace std;
using Comp = std::complex<double>;  // STL complex

namespace sjtu {
    constexpr Comp I{0.0, 1.0};  // i
    constexpr double pi = 3.14159265358979323846;;
class int2048 {
    // todo
    std::vector<int> digits;
    bool is_negative;
    static constexpr int BASE = 10000; // 压4位

public:
    // 构造函数
    int2048();
    int2048(long long);
    int2048(const std::string &);
    int2048(const int2048 &);

    // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
    // 如果需要，可以自行增加其他所需的函数
    // ===================================
    // Integer1
    // ===================================

    // 读入一个大整数
    void read(const std::string &);
    // 输出储存的大整数，无需换行
    void print();

    // 加上一个大整数
    int2048 &add(const int2048 &);
    // 返回两个大整数之和
    friend int2048 add(int2048, const int2048 &);

    // 减去一个大整数
    int2048 &minus(const int2048 &);
    // 返回两个大整数之差
    friend int2048 minus(int2048, const int2048 &);

    // void fft(Comp *a, int len, int op, Comp *tmp);
    void fft(Comp *a, int len, int op);
    int2048 &multiply_fft(const int2048 &other);

    // 乘上一个大整数
    int2048 &multiply(const int2048 &);
    // 返回两个大整数之积
    friend int2048 multiply(int2048, const int2048 &);

    int2048 shl_blocks(int k);
    int2048 shr_blocks(int k);

    // 求整数倒数，要求当前数为正，且最高块 >= BASE / 2
    int2048 inv();

    // 牛迭除法
    int2048 &divide_newton(const int2048 &);

    bool greater(const std::vector<int> &a, const std::vector<int> &b, int len, int idx);
    // 除以一个大整数
    int2048 &divide(const int2048 &);
    // 返回两个大整数之商
    friend int2048 divide(int2048, const int2048 &);

    // 取模一个大整数
    int2048 &modulo(const int2048 &);
    // 返回两个大整数之模
    friend int2048 modulo(int2048, const int2048 &);

    // ===================================
    // Integer2
    // ===================================

    int2048 operator+() const;
    int2048 operator-() const;

    int2048 &operator=(const int2048 &);

    int2048 &operator+=(const int2048 &);
    friend int2048 operator+(int2048, const int2048 &);

    int2048 &operator-=(const int2048 &);
    friend int2048 operator-(int2048, const int2048 &);

    int2048 &operator*=(const int2048 &);
    friend int2048 operator*(int2048, const int2048 &);

    int2048 &operator/=(const int2048 &);
    friend int2048 operator/(int2048, const int2048 &);

    int2048 &operator%=(const int2048 &);
    friend int2048 operator%(int2048, const int2048 &);

    friend std::istream &operator>>(std::istream &, int2048 &);
    friend std::ostream &operator<<(std::ostream &, const int2048 &);

    friend bool operator==(const int2048 &, const int2048 &);
    friend bool operator!=(const int2048 &, const int2048 &);
    friend bool operator<(const int2048 &, const int2048 &);
    friend bool operator>(const int2048 &, const int2048 &);
    friend bool operator<=(const int2048 &, const int2048 &);
    friend bool operator>=(const int2048 &, const int2048 &);
};
} // namespace sjtu

#endif
