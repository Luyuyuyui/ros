#include "bigint.hpp"
#include <algorithm>
/**
 * @brief 默认构造函数
 *  初始化数字为0，符号为正
 */
BigInt::BigInt() { digit.push_back(0); }
/**
 * @brief 字符串构造实现
 * @param raw_str 表示大整数的字符串
 * 1. 处理符号位（+/-）
 * 2. 跳过前导零
 * 3. 将数字字符倒序存储
 */
BigInt::BigInt(const std::string& raw_str) {
    if (raw_str.empty())  // 如果字符串为空
    {
        digit.push_back(0);
        return;
    }
    int start = 0;       // 数字部分的起始位置
    isNegative = false;  // 初始化符号
    if (raw_str[0] == '-') {
        isNegative = true;
        start = 1;
    } else if (raw_str[0] == '+') {
        start = 1;
    }  // 跳过前导零，找到第一个非零数字的位置
    while (start < raw_str.size() && raw_str[start] == '0') {
        start++;
    }
    // 从低位到高位存储数字
    digit.resize(raw_str.size() - start);  // 分配空间
    // 逆序存储数字
    for (int i = start; i < raw_str.size(); i++) {
        digit[raw_str.size() - 1 - i] = raw_str[i] - '0';
    }
}
/**
 * @brief 比较两个数绝对值大小
 * @param temp 要比较的另一个大整数
 * @return 比较结果：-1(小于), 0(等于), 1(大于)
 * 先比较位数，位数多的绝对值大；位数相同时，从高位到低位逐位比较
 */
int BigInt::compareAbs(const BigInt& temp) const {
    if (digit.size() != temp.digit.size())  // 如果两个数长度不同，直接比较
    {
        return digit.size() < temp.digit.size() ? -1 : 1;
    }
    for (int i = digit.size() - 1; i >= 0; i--)  // 长度相同则逐位比较
    {
        if (digit[i] != temp.digit[i]) {
            return digit[i] < temp.digit[i] ? -1 : 1;
        }
    }
    return 0;  // 绝对值完全相等
}
/**
 * @brief 拷贝赋值运算符实现
 * @param rhs 要赋值的对象
 * @return 当前对象的引用
 */
BigInt& BigInt::operator=(const BigInt& rhs) {
    if (this != &rhs)  // 防止自赋值
    {
        digit = rhs.digit;            // 移动rhs的数字数据到当前对象
        isNegative = rhs.isNegative;  // 复制符号
    }
    return *this;
}
/**
 * @brief 移动赋值运算符实现
 * @param rhs 要移动的对象
 * @return 当前对象的引用
 */
BigInt& BigInt::operator=(BigInt&& rhs) {
    if (this != &rhs)  // 如果出现自己给自己赋值
    {
        digit = std::move(rhs.digit);  // 移动rhs的数字数据到当前对象
        isNegative = rhs.isNegative;   // 复制符号
        rhs.digit = {0};               // 将源对象的数字设为0
        rhs.isNegative = false;        // 将源对象的符号设为正
    }
    return *this;
}
/**
 * @brief 获取数字的位数
 * @return 数字的实际位数
 */
std::size_t BigInt::getDigitsNum() const {
    int count = digit.size();
    if (digit.size() == 1 && digit[0] == 0)  // 特殊情况：数字为0
    {
        count = 1;
    } else {  // 从高位向低位遍历，去除前导零
        for (int i = digit.size() - 1; i >= 0; i--) {
            if (digit[i] != 0)  // 找到第一个非零的最高位数字
            {
                break;  // 停止遍历，此时count已经是有效位数
            }
            count--;  // 删去前导零
        }
    }
    return count;
}

/**
 * @brief 检查数字是否为0
 * @return 如果数字为0返回true，否则返回false
 */
bool BigInt::isZero() const { return digit.size() == 1 && digit[0] == 0; }
/**
 * @brief 负号运算符
 * @return 当前数的相反数
 */
BigInt BigInt::operator-() const {
    BigInt res = *this;  // 创建当前对象的副本
    if (!isZero())       // 只有不为0时取反
    {
        res.isNegative = !isNegative;  // 符号取反
    }
    return res;
}
/**
 * @brief 加法复合赋值运算符实现
 * @param rhs 要相加的右操作数
 * @return 当前对象的引用
 * 同号：直接相加
 * 异号：转换为减法
 */
BigInt& BigInt::operator+=(const BigInt& rhs) {
    if (isNegative == rhs.isNegative)  // 如果同号，直接相加
    {
        int len = std::max(digit.size(), rhs.digit.size());  // 结果的长度
        std::vector<int> res(len + 1, 0);                    // 存放结果，多一位用于进位
                                                             // 逐位相加
        for (int i = 0; i < len; i++) {
            if (i < digit.size())  // 不超过此向量长度，逐位相加
            {
                res[i] += digit[i];
            }
            if (i < rhs.digit.size())  // 不超过此向量长度，逐位相加
            {
                res[i] += rhs.digit[i];
            }
            if (res[i] > 9)  // 如果超过了9，需要进位
            {
                res[i + 1] += res[i] / 10;
                res[i] %= 10;
            }
        }
        while (res.size() > 1 && res.back() == 0)  // 移除前导0
        {
            res.pop_back();
        }
        digit = std::move(res);  // 移动res的数字数据到当前对象
    } else                       // 如果异号，变为减法
    {
        *this -= (-rhs);
    }
    return *this;
}
/**
 * @brief 减法复合赋值运算符实现
 * @param rhs 要相减的右操作数
 * @return 当前对象的引用
 * 异号：转换为加法
 * 同号：比较绝对值，用大数减小数
 */
BigInt& BigInt::operator-=(const BigInt& rhs) {
    if (isNegative != rhs.isNegative)  // 如果异号，变为加法
    {
        return *this += (-rhs);
    }
    // 同号相减：比较绝对值大小
    int cmp = compareAbs(rhs);  // 设置绝对值是否相等的判断标志
    if (cmp == 0)               // 如果两个数相等
    {
        digit = {0};
        isNegative = false;
        return *this;
    }
    // 标记是否交换了操作数顺序
    bool swapped = (cmp < 0);               // 如果前面数小于后面数，标记交换过（保证大数减小数）
    BigInt larger = swapped ? rhs : *this;  // 给larger赋值
    const BigInt& smaller = swapped ? *this : rhs;
    std::vector<int> res(larger.digit.size(), 0);  // 存放结果
    int borrow = 0;                                // 借位标记
                                                   // 逐位相减
    for (int i = 0; i < larger.digit.size(); i++) {
        int diff = larger.digit[i] - borrow;  // larger对应数位实际大小
        if (i < smaller.digit.size()) {
            diff -= smaller.digit[i];  // 进行对应位置相减
        }
        if (diff < 0)  // 如果不够减需要借位
        {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res[i] = diff;  // 存入结果
    }  // 删除前导0
    while (res.size() > 1 && res.back() == 0) {
        res.pop_back();
    }
    digit = std::move(res);
    if (swapped)  // 如果交换了，符号变化
    {
        isNegative = !isNegative;
    }
    if (digit.size() == 1 && digit[0] == 0)  // 如果结果为0 ，确保0没有负号
    {
        isNegative = false;
    }
    return *this;
}
/**
 * @brief 乘法复合赋值运算符实现
 * @param rhs 要相乘的右操作数
 * @return 当前对象的引用
 * 模拟竖式乘法
 */
BigInt& BigInt::operator*=(const BigInt& rhs) {
    if (isZero() || rhs.isZero())  // 如果两个数其中一个为0，结果为0
    {
        *this = BigInt("0");
        return *this;
    }  // 结果向量，长度最多为两数位数之和
    std::vector<int> res(digit.size() + rhs.digit.size(), 0);
    // 模拟竖式乘法
    for (int i = 0; i < digit.size(); i++) {
        for (int j = 0; j < rhs.digit.size(); j++) {
            res[j + i] += digit[j] * (rhs.digit[i]);  // 对应位置相乘
            if (res[j + i] > 9)                       // 判断进位
            {
                res[j + i + 1] += res[j + i] / 10;
                res[j + i] %= 10;
            }
        }
    }
    while (res.size() > 1 && res.back() == 0)  // 移除前导0
    {
        res.pop_back();
    }
    digit = std::move(res);
    isNegative = (isNegative != rhs.isNegative);  // 如果符号不相等，结果为负
    return *this;
}
/**
 * @brief 除法复合赋值运算符实现
 * @param rhs 要相除的右操作数
 * @return 当前对象的引用
 * 模拟竖式除法
 */
BigInt& BigInt::operator/=(const BigInt& rhs)

{
    if (rhs.isZero())  // 特别的：除数不能为0
    {
        std::cout << "除数不能为0！" << "\n";
    }
    if (isZero())  // 如果被除数为0，结果为0
    {
        return *this;
    }  // 比较绝对值大小
    int cmp = compareAbs(rhs);  // 绝对值相等判断标志
    if (cmp < 0)                // 小数除大数，结果为0
    {
        *this = BigInt("0");
        return *this;
    }  // 绝对值相等，结果为1或-1
    if (cmp == 0) {
        *this = BigInt("1");
        isNegative = (isNegative != rhs.isNegative);
        return *this;
    }  // 使用绝对值进行除法运算
    BigInt dividend = *this;      // 被除数
    BigInt divisor = rhs;         // 除数
    dividend.isNegative = false;  // 用绝对值除法
    divisor.isNegative = false;
    std::vector<int> res;                                 // 商的结果
    BigInt temp;                                          // 临时变量，用于存储当前处理的部分被除数
                                                          // 从被除数的高位开始逐位处理
    for (int i = dividend.digit.size() - 1; i >= 0; i--)  // 复制被除数
    {
        temp.digit.insert(temp.digit.begin(), dividend.digit[i]);  // 将被除数的当前位插入到temp的最低位
        // 去除temp中的前导零
        while (temp.digit.size() > 1 && temp.digit.back() == 0) {
            temp.digit.pop_back();
        }
        // 计算当前部分被除数temp中可以包含多少个除数divisor
        int count = 0;
        // 当temp的绝对值大于等于除数的绝对值时，继续减法
        while (temp.compareAbs(divisor) >= 0) {
            temp -= divisor;  // 从temp中减去一个除数
            count++;          // 商增加1
        }
        res.push_back(count);
    }
    std::reverse(res.begin(), res.end());      // 反转结果向量
    while (res.size() > 1 && res.back() == 0)  // 去除商中的前导零
    {
        res.pop_back();
    }
    digit = std::move(res);
    isNegative = (isNegative != rhs.isNegative);  // 设置结果的符号：同号得正，异号得负
    return *this;
}
/**
 * @brief 加法运算符实现
 * @param lhs 左操作数
 * @param rhs 右操作数
 * @return 两个大整数的和
 */
BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
    BigInt result = lhs;
    result += rhs;  // 使用+=
    return result;
}
/**
 * @brief 减法运算符实现
 * @param lhs 左操作数
 * @param rhs 右操作数
 * @return 两个大整数的差
 */
BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
    BigInt result = lhs;
    result -= rhs;
    return result;
}
/**
 * @brief 乘法运算符实现
 * @param lhs 左操作数
 * @param rhs 右操作数
 * @return 两个大整数的积
 */
BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
    BigInt result = lhs;
    result *= rhs;
    return result;
}
/**
 * @brief 除法运算符实现
 * @param lhs 左操作数
 * @param rhs 右操作数
 * @return 两个大整数的商
 */
BigInt operator/(const BigInt& lhs, const BigInt& rhs) {
    BigInt result = lhs;
    result /= rhs;
    return result;
}
/**
 * @brief 获取数字的字符串表示
 * @return 表示大整数的字符串
 * 将逆序存储的数字转换回正常顺序的字符串
 */
std::string BigInt::getStr() const {
    if (isZero())  // 如果是0
    {
        return "0";
    }
    std::string res;
    if (isNegative)  // 如果是负数
    {
        res += '-';
    }  // 从高位到低位拼接数字
    for (int i = digit.size() - 1; i >= 0; i--)  // 倒序存储
    {
        res += char(digit[i] + '0');
    }
    return res;
}
/**
 * @brief 布尔转换运算符实现
 * @return 如果数字不为0返回true，否则返回false
 */
BigInt::operator bool() const { return !isZero(); }