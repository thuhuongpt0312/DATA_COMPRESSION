#pragma once
#include <string>
using namespace std;

class RLEEncoder {
public:
    // Nén: aaabbc  ->  3a2bc
    static string encode(const string& text);

    // Giải nén: 3a2bc  ->  aaabbc
    static string decode(const string& encoded);

    // Demo đầy đủ: in phân tích chuỗi lặp, kết quả, thống kê
    static void demo(const string& text);
};

