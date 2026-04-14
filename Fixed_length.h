#pragma once
#include <string>
#include <map>
using namespace std;

class FixedLengthEncoder {
public:
    // Trả về: {chuỗi bit đã mã hóa, bảng mã}
    static pair<string, map<char, string>> encode(const string& text);

    // Giải mã từ chuỗi bit + bảng mã + số bit/ký tự
    static string decode(const string& encoded,
        const map<char, string>& codeTable,
        int bitsPerChar);

    // Demo đầy đủ: in bảng mã, kết quả, thống kê
    static void demo(const string& text);
};