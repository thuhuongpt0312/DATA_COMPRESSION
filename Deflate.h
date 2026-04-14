#pragma once
#include <string>
#include <vector>
using namespace std;

// ── Token LZ77 ───────────────────────────────────────────
// Mỗi token là một trong hai dạng:
//   - Literal  : offset=0, length=0, next=ký_tự
//   - Back-ref : offset>0, length>0, next=ký_tự_tiếp_sau
struct LZ77Token {
    int  offset;   // Khoảng lùi về cửa sổ trước
    int  length;   // Độ dài chuỗi khớp
    char next;     // Ký tự ngay sau chuỗi khớp

    LZ77Token(int o, int l, char n)
        : offset(o), length(l), next(n) {
    }
};

// ── Deflate = LZ77 + Huffman ─────────────────────────────
class DeflateEncoder {
public:
    // Demo đầy đủ: in bảng token LZ77, kết quả Huffman, thống kê
    static void demo(const string& text);

private:
    static const int WINDOW_SIZE = 255;  // Sliding window
    static const int MAX_MATCH = 255;  // Độ dài khớp tối đa

    // Bước 1 – LZ77: tìm chuỗi lặp trong sliding window
    static vector<LZ77Token> lz77Encode(const string& text);
};