#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief 大整数类，支持大整数的基本算术运算
 *
 * 数字以字符串形式输入，内部用向量存储各位数字，支持加减乘除等基本运算
 */
class BigInt final {
public:
    /**
     * @brief 默认构造函数
     *
     * 初始化为0
     */
    BigInt();
    /**
     * @brief 字符串构造函数
     * @param raw_atr 表示大整数的字符串，可为负
     */
    explicit BigInt(const std::string& raw_str);
    BigInt(const BigInt& rhs) = default;  ///< 默认拷贝构造函数
    BigInt(BigInt&& rhs) = default;       ///< 默认移动构造函数

    ~BigInt() = default;  ///< 默认析构函数
    /**
     * @brief 拷贝赋值运算符
     * @param rhs 要赋值的大整数
     * @return 当前对象的引用
     */
    BigInt& operator=(const BigInt& rhs);
    /**
     * @brief 移动赋值运算符
     * @param rhs 要移动的大整数
     * @return 当前对象的引用
     */
    BigInt& operator=(BigInt&& rhs);
    /**
     * @brief 加法复合赋值运算符
     * @param rhs 要相加的大整数
     * @return 当前对象的引用
     */
    BigInt& operator+=(const BigInt& rhs);
    /**
     * @brief 减法复合赋值运算符
     * @param rhs 要相减的大整数
     * @return 当前对象的引用
     */
    BigInt& operator-=(const BigInt& rhs);
    /**
     * @brief 乘法复合赋值运算符
     * @param rhs 要相乘的大整数
     * @return 当前对象的引用
     */
    BigInt& operator*=(const BigInt& rhs);
    /**
     * @brief 除法复合赋值运算符
     * @param rhs 要相除的大整数
     * @return 当前对象的引用
     */
    BigInt& operator/=(const BigInt& rhs);

    friend BigInt operator+(
        const BigInt& lhs, const BigInt& rhs
    );  ///< 加法运算符
    friend BigInt operator-(
        const BigInt& lhs, const BigInt& rhs
    );  ///< 减法运算符
    friend BigInt operator*(
        const BigInt& lhs, const BigInt& rhs
    );  ///< 乘法运算符
    friend BigInt operator/(
        const BigInt& lhs, const BigInt& rhs
    );  ///< 除法运算符
    /**
     * @brief 获取数字的位数
     * @return 数字的实际位数
     */
    std::size_t getDigitsNum() const;
    /**
     * @brief 获取数字的字符串表示
     * @return 表示大整数的字符串
     */
    std::string getStr() const;
    /**
     * @brief 负号运算符
     * @return 当前数的相反数
     */
    BigInt operator-() const;
    /**
     * @brief 布尔转换运算符
     * @return 如果数字不为0返回true，否则返回false
     */
    explicit operator bool() const;

private:
    std::vector<int> digit;  ///< 倒序存储各位数字
    bool isNegative;         ///< 符号标志，true表示负数
    /**
     * @brief 检查数字是否为0
     * @return 如果数字为0返回true，否则返回false
     */
    bool isZero() const;
    /**
     * @brief 比较两个数的绝对值大小
     * @param temp 要比较的另一个大整数
     * @return 如果当前数绝对值小于参数返回-1，等于返回0，大于返回1
     */
    int compareAbs(const BigInt& temp) const;
};

#endif