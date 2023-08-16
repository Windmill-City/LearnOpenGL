#include <gtest/gtest.h>

#include "CodeCvt.hpp"

TEST(to_u16string, wstring)
{
    std::wstring hello_1 = L"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::wstring hello_2 = L"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    std::u16string hello_1_r = u"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::u16string hello_2_r = u"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    EXPECT_EQ(CodeCvt::to_u16string(hello_1), hello_1_r);
    EXPECT_EQ(CodeCvt::to_u16string(hello_2), hello_2_r);
}

TEST(to_u16string, string)
{
    std::string hello_1 = (const char*)u8"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::string hello_2 = (const char*)u8"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    std::u16string hello_1_r = u"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::u16string hello_2_r = u"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    EXPECT_EQ(CodeCvt::to_u16string(hello_1), hello_1_r);
    EXPECT_EQ(CodeCvt::to_u16string(hello_2), hello_2_r);
}

TEST(to_wstring, u16string)
{
    std::u16string hello_1 = u"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::u16string hello_2 = u"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    std::wstring hello_1_r = L"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::wstring hello_2_r = L"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    EXPECT_EQ(CodeCvt::to_wstring(hello_1), hello_1_r);
    EXPECT_EQ(CodeCvt::to_wstring(hello_2), hello_2_r);
}

TEST(to_wstring, string)
{
    std::string hello_1 = (const char*)u8"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::string hello_2 = (const char*)u8"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    std::wstring hello_1_r = L"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::wstring hello_2_r = L"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    EXPECT_EQ(CodeCvt::to_wstring(hello_1), hello_1_r);
    EXPECT_EQ(CodeCvt::to_wstring(hello_2), hello_2_r);
}

TEST(to_string, u16string)
{
    std::u16string hello_1 = u"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::u16string hello_2 = u"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    std::string hello_1_r = (const char*)u8"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::string hello_2_r = (const char*)u8"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    EXPECT_EQ(CodeCvt::to_string(hello_1), hello_1_r);
    EXPECT_EQ(CodeCvt::to_string(hello_2), hello_2_r);
}

TEST(to_string, wstring)
{
    std::wstring hello_1 = L"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::wstring hello_2 = L"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    std::string hello_1_r = (const char*)u8"Hello World! 你好世界！\u0000\u007f\u0080\u07ff\u0800\uffff";
    std::string hello_2_r = (const char*)u8"你好世界！ Hello World!\u0000\u007f\u0080\u07ff\u0800\uffff";

    EXPECT_EQ(CodeCvt::to_string(hello_1), hello_1_r);
    EXPECT_EQ(CodeCvt::to_string(hello_2), hello_2_r);
}

int main(int argc, char** argv)
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}