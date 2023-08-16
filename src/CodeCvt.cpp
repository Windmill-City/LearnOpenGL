#include "CodeCvt.hpp"

std::wstring CodeCvt::to_wstring(const std::string_view u8) noexcept
{
    // extra codepoint size
    size_t             cp_s = 0;
    wchar_t            ch16;
    std::wstringstream ss;
    for (char ch8 : u8)
    {
        // UTF-8 CodePoint Masks
        char cp_7_m  = (char)0b10000000;
        char cp_11_m = (char)0b11100000;
        char cp_16_m = (char)0b11110000;
        char cp_21_m = (char)0b11111000;
        char cp_26_m = (char)0b11111100;
        char cp_31_m = (char)0b11111110;
        // UTF-8 CodePoints(Byte1)
        char cp_7    = (char)0b00000000;
        char cp_11   = (char)0b11000000;
        char cp_16   = (char)0b11100000;
        char cp_21   = (char)0b11110000;
        char cp_26   = (char)0b11111000;
        char cp_31   = (char)0b11111100;
        // Extra Codepoint(Byte2~6)
        char cp_v_m  = (char)0b11000000;
        char cp_v    = (char)0b10000000;

        if (!cp_s && (ch8 & cp_v_m) == cp_v) continue;
        // Process extra codepoints - 0b10xxxxxx
        if (cp_s && (ch8 & cp_v_m) == cp_v)
        {
            cp_s -= 6;
            ch16 |= ((char16_t)ch8 & 0b00111111) << cp_s;
            if (!cp_s) ss << ch16;
            continue;
        }

        // Codepoint 7
        if ((ch8 & cp_7_m) == cp_7)
        {
            cp_s = 0;
            ch16 = ch8 & 0b01111111;
            ss << ch16;
            continue;
        }

        // Codepoint 11
        if ((ch8 & cp_11_m) == cp_11)
        {
            cp_s = 11 - 5;
            ch16 = ch8 << cp_s;
            continue;
        }

        // Codepoint 16
        if ((ch8 & cp_16_m) == cp_16)
        {
            cp_s = 16 - 4;
            ch16 = ch8 << cp_s;
            continue;
        }

        // Codepoint 21 ~ 31 ignored
        // 0xFF & 0xFE are ignored
    }

    return ss.str();
}

std::string CodeCvt::to_string(const std::wstring_view u16) noexcept
{
    std::stringstream ss;
    for (char16_t ch16 : u16)
    {
        // Codepoint 7
        if (ch16 >= 0x00 && ch16 <= 0x7F)
        {
            ss << (char)ch16;
        }

        // Codepoint 11
        if (ch16 >= 0x80 && ch16 <= 0x7FF)
        {
            char ch8_1 = 0b11000000 | ch16 >> 6;
            char ch8_2 = 0b10000000 | (ch16 & 0b00111111);
            ss << ch8_1 << ch8_2;
        }

        // Codepoint 16
        if (ch16 >= 0x800 && ch16 <= 0xFFFF)
        {
            char ch8_1 = 0b11100000 | ch16 >> 12;
            char ch8_2 = 0b10000000 | (ch16 >> 6 & 0b00111111);
            char ch8_3 = 0b10000000 | (ch16 & 0b00111111);
            ss << ch8_1 << ch8_2 << ch8_3;
        }
    }

    return ss.str();
}
