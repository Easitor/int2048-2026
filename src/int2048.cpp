#include "./include/int2048.h"

namespace sjtu {
    int2048::int2048() {
        digits.push_back(0);
        is_negative = false;
    }
    int2048::int2048(long long value) {
        is_negative = (value < 0);
        if (!value) {
            digits.push_back(0);
        }
        // value = abs(value);
        if (is_negative) {
            value = -value; 
        }
        while (value) {
            digits.push_back(value % BASE);
            value /= BASE;
        }
    }
    int2048::int2048(const std::string &s) {
        read(s);
    }
    int2048::int2048(const int2048 &) = default;
    int2048 minus(int2048 a, const int2048 &b);

    // 读入一个大整数
    void int2048::read(const std::string &s) {
        digits.clear();
        if (s[0] == '-') {
            is_negative = true;
        } 
        else {
            is_negative = false;
        }

        const int powers[4] = {1, 10, 100, 1000};
        int cnt = 0, res = 0;
        for (int i = s.size() - 1; i >= static_cast<int>(is_negative); i--) {
            // 注意不要把负号扔进去
            res += powers[cnt] * (s[i] - '0');
            if (++cnt == 4) {
                digits.push_back(res);
                cnt = res = 0;
            }
        }
        if (cnt) {
            digits.push_back(res);
        }

        // 删除前导0
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }

        // 小心-0
        if (digits.size() == 1 && digits[0] == 0) {
            is_negative = false;
        }
    }
    // 输出储存的大整数，无需换行
    void int2048::print() {
        if (is_negative) {
            std::cout << '-';
        }
        for (int i = digits.size() - 1; i >= 0; i--) {
            if (i != digits.size() - 1) {
                if (digits[i] < 1000) {
                    std::cout << '0';
                }
                if (digits[i] < 100) {
                    std::cout << '0';
                }
                if (digits[i] < 10) {
                    std::cout << '0';
                }    
            }
            std::cout << digits[i];
        }
    }
    // 加上一个大整数 a.add(b)
    int2048 &int2048::add(const int2048 &other) {
        // a, b 同号
        if (other.is_negative == is_negative) {
            for (int i = 0; i < other.digits.size(); i++) {
                if (i < digits.size()) {
                    digits[i] += other.digits[i];
                } 
                else {
                    digits.push_back(other.digits[i]);
                }
            }

            // 处理进位
            for (int i = 0; i < digits.size(); i++) {
                if (digits[i] >= BASE) {
                    digits[i] -= BASE;
                    if (i + 1 < digits.size()) {
                        digits[i + 1]++;
                    } 
                    else {
                        digits.push_back(1);
                    }
                }
            }
            return *this;    
        }
        // a, b 异号
        else {
            // 原数非负
            if (!is_negative) {
                auto temp = other;
                temp.is_negative = false;
                minus(temp);  // 5 + (-3) → 5 - 3
            } 
            // 原数为负
            // -5 + 3 = -2  ->  3 - 5 = -2
            else {
                // other - *this
                auto temp = *this;
                temp.is_negative = false;
                *this = sjtu::minus(other, temp);
            }
            return *this;
        }
    }   
    // 返回两个大整数之和
    int2048 add(int2048 a, const int2048 &b) {
        a.add(b);
        return a;
    }
    // 减去一个大整数
    int2048 &int2048::minus(const int2048 &other) {
        // a, b 同号
        if (other.is_negative == is_negative) {
            // 比较大小
            bool greater = false; // a >= b
            if (digits.size() > other.digits.size()) {
                greater = true;
            } 
            else if (digits.size() == other.digits.size()) {
                for (int i = digits.size() - 1; i >= 0; i--) {
                    if (digits[i] > other.digits[i]) {
                        greater = true;
                        break;
                    } 
                    else if (digits[i] < other.digits[i]) {
                        greater = false;
                        break;
                    }
                }
            }

            if (greater) {
                for (int i = 0; i < other.digits.size(); i++) {
                    digits[i] -= other.digits[i];
                }
                // 处理借位
                for (int i = 0; i < digits.size(); i++) {
                    if (digits[i] < 0) {
                        digits[i] += BASE;
                        digits[i + 1]--;
                    }
                }
            } 
            else {
                std::vector<int> temp = other.digits;
                for (int i = 0; i < digits.size(); i++) {
                    temp[i] -= digits[i];
                }
                // 处理借位
                for (int i = 0; i < temp.size(); i++) {
                    if (temp[i] < 0) {
                        temp[i] += BASE;
                        temp[i + 1]--;
                    }
                }
                digits = temp;
                is_negative = !is_negative;
            }
        }
        // a, b 异号
        else {
            auto temp = other;
            temp.is_negative = is_negative;
            add(temp);
        }
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
        // 零不能带负号
        if (digits.size() == 1 && digits[0] == 0) {
            is_negative = false;
        }
        return *this;
    }
    // 返回两个大整数之差
    int2048 minus(int2048 a, const int2048 &b) {
        a.minus(b);
        return a;
    }
    
    // void int2048::fft(Comp *a, int len, int op, Comp *tmp) {
    //     if (len == 1) return;
    //     for (int i = 0; i < len; i++) tmp[i] = a[i];
    //     for (int i = 0; i < len; i++) {
    //         if (i & 1) a[i >> 1 | len >> 1] = tmp[i];
    //         else a[i >> 1] = tmp[i];
    //     }
    //     Comp *g = a, *h = a + (len >> 1);
    //     fft(g, len >> 1, op, tmp);
    //     fft(h, len >> 1, op, tmp);
    //     Comp now(1, 0), step(cos(2 * pi / len), op * sin(2 * pi / len));
    //     for (int i = 0; i < (len >> 1); i++) {
    //         tmp[i] = g[i] + now * h[i];
    //         tmp[i + (len >> 1)] = g[i] - now * h[i];
    //         now *= step;
    //     }
    //     for (int i = 0; i < len; i++) a[i] = tmp[i];
    // }

    // 蝶形运算优化fft
    void int2048::fft(Comp *a, int len, int op) {
        for (int i = 1, j = 0; i < len; ++i) {
            int bit = len >> 1;
            // 模拟二进制倒着的加法
            while (j & bit) {
                j ^= bit;
                bit >>= 1;
            }
            j ^= bit;
            if (i < j) {
                Comp temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
        for (int width = 2; width <= len; width <<= 1) {
            int half = width >> 1;
            double angle = 2 * pi / width;
            Comp step(std::cos(angle), op * std::sin(angle));
            for (int start = 0; start < len; start += width) {
                Comp now(1, 0);
                for (int j = 0; j < half; ++j) {
                    Comp u = a[start + j];
                    Comp v = now * a[start + j + half];
                    a[start + j] = u + v;
                    a[start + j + half] = u - v;
                    now *= step;
                }
            }
        }
    }

    int2048 &int2048::multiply_fft(const int2048 &other) {
        bool negative = (is_negative != other.is_negative);
        int n = static_cast<int>(digits.size()) * 2;
        int m = static_cast<int>(other.digits.size()) * 2;

        // 0 ~ (n - 1) + (m - 1)
        // fft里压两位
        int len = 1;
        while (len < n + m - 1) {
            len <<= 1;
        }

        std::vector<Comp> f(len), g(len);
        for (int i = 0; i < static_cast<int>(digits.size()); ++i) {
            f[2 * i] = digits[i] % 100;
            f[2 * i + 1] = digits[i] / 100;
        }
        for (int i = 0; i < static_cast<int>(other.digits.size()); ++i) {
            g[2 * i] = other.digits[i] % 100;
            g[2 * i + 1] = other.digits[i] / 100;
        }

        fft(f.data(), len, 1);
        fft(g.data(), len, 1);
        for (int i = 0; i < len; ++i) {
            f[i] *= g[i];
        }
        fft(f.data(), len, -1); 

        std::vector<int> result;
        result.reserve(digits.size() + other.digits.size());
        long long up = 0;

        for (int i = 0; i < n + m - 1 || up > 0; ++i) {
            long long res = 0;
            if (i < n + m - 1) {
                res = static_cast<long long>(f[i].real() / len + 0.5);
            }
            long long cur = res + up;
            int block = cur % 100;
            up = cur / 100;
            if (i & 1) {
                result.back() += block * 100;  // 前两位
            } 
            else {
                result.push_back(block);       // 后两位
            }
        }

        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }

        digits = result;
        is_negative = negative && !(digits.size() == 1 && digits[0] == 0);
        return *this;
    }

    // 乘上一个大整数
    int2048 &int2048::multiply(const int2048 &other) {
        if (digits.size() >= 128 && other.digits.size() >= 128) {
            return multiply_fft(other);
        }
        is_negative = (is_negative != other.is_negative);
        std::vector<int> result(digits.size() + other.digits.size(), 0);
        int len = digits.size() + other.digits.size();
        for (int i = 0; i < len; i++) {
            for (int j = std::max(0, i - static_cast<int>(other.digits.size()) + 1); 
                     j <= i && j < static_cast<int>(digits.size()); j++) {
                result[i] += digits[j] * other.digits[i - j];
                if (result[i] >= BASE) {
                    result[i + 1] += result[i] / BASE;
                    result[i] %= BASE;
                }
            }
        }
        digits = result;
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
        if (digits.size() == 1 && digits[0] == 0) {
            is_negative = false;
        }
        return *this;
    }
    // 返回两个大整数之积
    int2048 multiply(int2048 a, const int2048 &b) {
        a.multiply(b);
        return a;
    }
    
    // 左移k位
    int2048 int2048::shl_blocks(int k) {
        int2048 res = *this;
        if (digits.size() == 1 && digits[0] == 0) {
            return res;
        }
        res.digits.insert(res.digits.begin(), k, 0);
        return res;
    }

    // 右移k位
    int2048 int2048::shr_blocks(int k) {
        int2048 res;
        if (digits.size() <= k) {
            return res;
        }
        res.digits.assign(digits.begin() + k, digits.end());
        return res;
    }

    // 取倒数 S = BASE ^ (2 * n) 算 [S / b]
    int2048 int2048::inv() {
        int n = digits.size();
        if (n == 1) {
            return int2048(1LL * BASE * BASE / digits[0]);
        }
        
        // 取当前数的高半部分
        int k = n / 2;
        int2048 high = shr_blocks(k);
        int2048 seed = high.inv(); // 用高半部分做初始估计值
        int2048 scale = int2048(1LL).shl_blocks(2 * n);
        int2048 scale_short = int2048(1LL).shl_blocks(2 * n - k);
        // cor = 2 * S - D * X
        int2048 cor = scale_short + scale_short - (*this) * seed;
        
        // X_new = floor(2 * X - X ^ 2 / T)    T = S / D 
        //       = floor(X * (2 * S - D * X) / S)
        int2048 res = (seed * cor).shr_blocks(2 * (n - k));
        
        // 修正误差 res会偏小
        int2048 remainder = scale - (*this) * res;
        while (remainder >= *this) {
            res.add(int2048(1LL));
            remainder.minus(*this);
        }
        return res;
    }

    int2048 &int2048::divide_newton(const int2048 &other) { 
        bool negative = (is_negative != other.is_negative);
        int2048 a = *this, b = other;
        a.is_negative = b.is_negative = 0;
        if (a < b) {
            if (a.digits.size() == 1 && a.digits[0] == 0) {
                *this = a;
            }
            else if (negative) {
                *this = int2048(-1LL);
            }
            else {
                *this = int2048(0LL);
            } 
            return *this;
        }
        
        // 保证b的最高 >= 5000 减小误差
        int top = b.digits.back();
        int f = BASE / (top + 1);
        int2048 factor(f);
        a.multiply(factor);
        b.multiply(factor);
        
        int n = a.digits.size(), m = b.digits.size();
        // a * (1 / b)  
        // 保证 a < S 可以保证最多调整1次
        int len = std::max(m, n - m);
        int shift = std::max(0, n - 2 * m);
        a = a.shl_blocks(shift);
        b = b.shl_blocks(shift);
        int2048 inverse = b.inv();
        int2048 quotient = (a * inverse).shr_blocks(2 * len);
        int2048 remainder = a - b * quotient;
        if (remainder >= b) {
            quotient.add(int2048(1LL));
            remainder.minus(b);
        }
        bool remaining = !(remainder.digits.size() == 1 && remainder.digits[0] == 0);
        if (remaining && negative) {
            quotient.add(int2048(1LL));
        }
        quotient.is_negative = negative && !(quotient.digits.size() == 1 && quotient.digits[0] == 0);
        *this = quotient;
        return *this;
    }

    bool int2048::greater(const std::vector<int> &a, const std::vector<int> &b, 
                          int len, int idx) {
        // a: {idx, idx+1, ..., idx+len-1}
        // b: {0, 1, ..., len-1}
        for (int i = a.size() - 1; i >= idx + len; i--) {
            if (a[i] != 0) {
                return true;
            }
        }
        for (int i = len + idx - 1; i >= idx; i--) {
            int cur = (i < a.size()) ? a[i] : 0;
            if (cur != b[i - idx]) {
                return cur > b[i - idx];
            }
        }
        return true;
    }
    // 除以一个大整数
    int2048 &int2048::divide(const int2048 &other) {
        int l1 = digits.size(), l2 = other.digits.size();
        if (l1 - l2 >= 256 && (l1 >= 4096 || (l1 >= 512 && l2 >= 128))) {
            return divide_newton(other);
        }

        bool negative = (is_negative != other.is_negative);
        std::vector<int> divisor = other.digits;
        

        std::vector<int> result(l1, 0);
        std::vector<int> remain = digits;
        for (int i = l1 - l2; i >= 0; i--) {
            int l = 0, r = BASE - 1;
            while (l < r) {
                int mid = l + r + 1 >> 1;
                std::vector<int> temp = divisor;
                for (int j = 0; j < temp.size(); j++) {
                    temp[j] *= mid;
                }
                for (int j = 0; j < temp.size(); j++) {
                    if (temp[j] >= BASE) {
                        if (j + 1 < temp.size()) {
                            temp[j + 1] += temp[j] / BASE;
                        } 
                        else {
                            temp.push_back(temp[j] / BASE);
                        }
                        temp[j] %= BASE;
                    }
                }
                if (greater(remain, temp, temp.size(), i)) {
                    l = mid;
                } 
                else {
                    r = mid - 1;
                }
            }

            result[i] = l;
            std::vector<int> temp = divisor;
            for (int j = 0; j < temp.size(); j++) {
                temp[j] *= l;
            }
            for (int j = 0; j < temp.size(); j++) {
                if (temp[j] >= BASE) {
                    if (j + 1 < temp.size()) {
                        temp[j + 1] += temp[j] / BASE;
                    } 
                    else {
                        temp.push_back(temp[j] / BASE);
                    }
                    temp[j] %= BASE;
                }
            }

            for (int j = 0; j < temp.size(); j++) {
                remain[i + j] -= temp[j];
                if (remain[i + j] < 0) {
                    remain[i + j] += BASE;
                    remain[i + j + 1]--;
                }
            }
        }

        bool remaining = false; // 有余数
        for (int digit : remain) {
            if (digit != 0) {
                remaining = true;
                break;
            }
        }
        
        digits = result;
        is_negative = false; // 方便负数取整

        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }

        // 负数向下取整
        if (negative && remaining) {
            add(int2048(1ll));
        }

        // 0 / (-1) = 0 !!
        is_negative = negative && !(digits.size() == 1 && digits[0] == 0);
        return *this;
    }
    // 返回两个大整数之商
    int2048 divide(int2048 a, const int2048 &b) {
        a.divide(b);
        return a;
    }

    // 取模一个大整数
    int2048 &int2048::modulo(const int2048 &other) {
        auto temp = *this;
        temp.divide(other);
        temp.multiply(other);
        minus(temp);
        return *this;
    }
    // 返回两个大整数之模
    int2048 modulo(int2048 a, const int2048 &b) {
        a.modulo(b);
        return a;
    }

    // ===================================
    // Integer2
    // ===================================

    int2048 int2048::operator+() const {
        return *this;
    }
    int2048 int2048::operator-() const {
        auto temp = *this;
        // 零不需要翻转符号!
        if (!(temp.digits.size() == 1 && temp.digits[0] == 0)) {
            temp.is_negative = !temp.is_negative;
        }
        return temp;
    }

    int2048 &int2048::operator=(const int2048 &) = default;

    int2048 &int2048::operator+=(const int2048 &other) {
        return add(other);
    }
    int2048 operator+(int2048 a, const int2048 &b) {
        a.add(b);
        return a;
    }

    int2048 &int2048::operator-=(const int2048 &other) {
        return minus(other);
    }
    int2048 operator-(int2048 a, const int2048 &b) {
        a.minus(b);
        return a;
    }

    int2048 &int2048::operator*=(const int2048 &other) {
        return multiply(other);
    }
    int2048 operator*(int2048 a, const int2048 &b) {
        a.multiply(b);
        return a;
    }

    int2048 &int2048::operator/=(const int2048 &other) {
        return divide(other);
    }
    int2048 operator/(int2048 a, const int2048 &b) {
        a.divide(b);
        return a;
    }

    int2048 &int2048::operator%=(const int2048 &other) {
        return modulo(other);
    }
    int2048 operator%(int2048 a, const int2048 &b) {
        a.modulo(b);
        return a;
    }

    std::istream &operator>>(std::istream &in, int2048 &value) {
        std::string s;
        if (in >> s) {
            value.read(s);
        }
        return in;
    }
    std::ostream &operator<<(std::ostream &out, const int2048 &value) {
        if (value.is_negative) {
            out << '-';
        }
        int n = static_cast<int>(value.digits.size());
        for (int i = n - 1; i >= 0; i--) {
            if (i == n - 1) {
                out << value.digits[i];
            }
            else {
                if (value.digits[i] < 1000) out << '0';
                if (value.digits[i] < 100)  out << '0';
                if (value.digits[i] < 10)   out << '0';
                out << value.digits[i];
            }
        }
        return out;
    }

    bool operator==(const int2048 &a, const int2048 &b) {
        return a.is_negative == b.is_negative && a.digits == b.digits;
    }
    bool operator!=(const int2048 &a, const int2048 &b) {
        return !(a == b);
    }
    bool operator<(const int2048 &a, const int2048 &b) {
        if (a.is_negative != b.is_negative) {
            return a.is_negative;
        }
        if (a.digits.size() < b.digits.size()) {
            return !a.is_negative;
        }
        if (a.digits.size() > b.digits.size()) {
            return a.is_negative;
        }
        for (int i = a.digits.size() - 1; i >= 0; i--) {
            if (a.digits[i] != b.digits[i]) {
                return a.is_negative ? (a.digits[i] > b.digits[i]) : (a.digits[i] < b.digits[i]);
            }
        }
        return false;
    }
    bool operator>(const int2048 &a, const int2048 &b) {
        return b < a;
    }
    bool operator<=(const int2048 &a, const int2048 &b) {
        return !(a > b);
    }
    bool operator>=(const int2048 &a, const int2048 &b) {
        return !(a < b);
    }
} // namespace sjtu
