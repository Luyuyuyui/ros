/* UTF-8 */
#include <cstddef>
#include <stdexcept>
#include <string>
/**
 * @brief 字符串类
 *
 * 实现了对字符串的一系列操作，包括复制字符串和访问字符串
 */
class MyString {
public:
    using Traits = std::char_traits<char>;  ///< 字符特性类型
    /**
     * @brief 默认构造函数
     *
     * 创建一个空的MyString对象
     */
    MyString() = default;
    /**
     * @brief 复制构造字符串
     *
     * 复制已有字符串内容来创建MyString对象
     *
     * @param str  指向原字符串的指针
     * @param len_ 原字符串长度
     * @param str_ 新的字符串
     */
    MyString(const char* str) {
        len_ = Traits::length(str);     ///< 计算字符串长度
        str_ = new char[len_];          ///< 根据字符串长度开辟一块内存
        Traits::copy(str_, str, len_);  ///< 把字符串内容复制
    }
    /**
     * @brief 创建指定长度的空字符串
     *
     * 创建具有指定长度的MyString对象并且所有字符初始化为空
     *
     * @param len 提供的字符串长度
     * @param str_ 新的字符串
     */
    explicit MyString(const std::size_t len) : len_(len) {
        str_ = new char[len_]{};  ///< 申请内存并且内容初始化为0
    }

    /**
     * @brief 析构函数
     */
    ~MyString() { delete[] str_; }
    /**
     * @brief 访问指定索引值的字符（不进行边界检查）
     *
     * @param index 想要访问的索引值
     * @return 返回该索引指向字符串对应内容
     * @warning 注意索引不能越界（不进行边界检查）
     */
    char& operator[](const std::size_t index) { return str_[index]; }
    /**
     * @brief 更加安全的访问指定索引值的字符
     *
     * @param index 想要访问的索引值
     * @throw std::out_of_range 如果位置超出字符串长度
     * @return 返回该索引指向字符串对应内容
     */
    char& at(const std::size_t index) {
        if (index >= len_) {
            throw std::out_of_range("try to get item out of range in MyString");
        }
        return str_[index];
    }
    /**
     * @brief 检查字符串是否为空
     *
     * 将字符串转换为布尔值，用于检查字符串是否非空
     *
     * @return true表示不为空，false表示为空
     */
    explicit operator bool() const { return len_ > 0; }

private:
    char* str_ = nullptr;  ///< 实际存储字符串内容的内存位置
    std::size_t len_ = 0;  ///< 字符串的长度
};
