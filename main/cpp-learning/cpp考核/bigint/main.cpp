#include <iostream>
#include "bigint.hpp"
#include <vector>
/**
 * @brief 主函数 - 大整数计算器测试程序
 * @return 程序退出
 *
 * 程序流程：
 * 1. 提示用户输入两个大整数
 * 2. 创建BigInt对象
 * 3. 执行所有基本算术运算
 * 4. 显示运算结果
 */
int main() {
    std::string num1, num2;
    std::cout << "输入第一个大整数: ";
    std::cin >> num1;
    std::cout << "输入第二个大整数: ";
    std::cin >> num2;
    // 创建大整数对象
    BigInt a(num1);
    BigInt b(num2);
    // 显示基本信息和运算结果
    std::cout << "a = " << a.getStr() << std::endl;
    std::cout << "b = " << b.getStr() << std::endl;
    std::cout << "a的位数: " << a.getDigitsNum() << std::endl;
    std::cout << "b的位数: " << b.getDigitsNum() << std::endl;
    std::cout << "a + b = " << (a + b).getStr() << std::endl;
    std::cout << "a - b = " << (a - b).getStr() << std::endl;
    std::cout << "b - a = " << (b - a).getStr() << std::endl;
    std::cout << "a * b = " << (a * b).getStr() << std::endl;
    std::cout << "a / b = " << (a / b).getStr() << std::endl;

    return 0;
}