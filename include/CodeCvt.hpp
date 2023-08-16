#pragma once

#include <string>
#include <sstream>

struct CodeCvt
{
    /**
     * @brief Convert UTF-8 encoded string to UCS-2 encoded string
     *
     * @param u8 UTF-8 encoded string
     * @return std::wstring UCS-2 encoded string
     */
    static std::wstring to_wstring(const std::string_view u8) noexcept;
    /**
     * @brief Convert UTF-8 encoded string to UCS-2 encoded string
     *
     * @param u16 UCS-2 encoded string
     * @return std::string UTF-8 encoded string
     */
    static std::string to_string(const std::wstring_view u16) noexcept;
};