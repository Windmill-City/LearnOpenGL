#pragma once

#include <string>
#include <sstream>

struct CodeCvt
{
    using u16stringstream = std::basic_stringstream<char16_t>;

    /**
     * @note only support U+0000 ~ U+FFFF, others are ignored
     * @note invalid char are ignored
     * @return std::u16string
     */
    static std::u16string to_u16string(const std::wstring_view u16) noexcept;
    /**
     * @note only support U+0000 ~ U+FFFF, others are ignored
     * @note invalid char are ignored
     * @return std::u16string encoded in UTF-16
     */
    static std::u16string to_u16string(const std::string_view u8) noexcept;

    /**
     * @note only support U+0000 ~ U+FFFF, others are ignored
     * @return std::wstring encoded in UTF-16
     */
    static std::wstring to_wstring(const std::u16string_view u16) noexcept;
    /**
     * @note only support U+0000 ~ U+FFFF, others are ignored
     * @return std::wstring encoded in UTF-16
     */
    static std::wstring to_wstring(const std::string_view u8) noexcept;

    /**
     * @note only support U+0000 ~ U+FFFF, others are ignored
     * @return std::string encoded in UTF-8
     */
    static std::string to_string(const std::u16string_view u16) noexcept;
    /**
     * @note only support U+0000 ~ U+FFFF, others are ignored
     * @return std::string encoded in UTF-8
     */
    static std::string to_string(const std::wstring_view u16) noexcept;
};