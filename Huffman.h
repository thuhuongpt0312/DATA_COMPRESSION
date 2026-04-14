#pragma once
#include <string>
#include <map>
#include <memory>
using namespace std;

// ── Node của cây Huffman ──────────────────────────────────
struct HuffmanNode {
    char   ch;
    int    freq;
    shared_ptr<HuffmanNode> left, right;

    // Node lá (ký tự thực sự)
    HuffmanNode(char c, int f)
        : ch(c), freq(f), left(nullptr), right(nullptr) {
    }

    // Node nội (node gộp)
    HuffmanNode(int f,
        shared_ptr<HuffmanNode> l,
        shared_ptr<HuffmanNode> r)
        : ch('\0'), freq(f), left(l), right(r) {
    }
};

// ── Comparator cho min-heap ───────────────────────────────
struct CompareNode {
    bool operator()(shared_ptr<HuffmanNode> a,
        shared_ptr<HuffmanNode> b) {
        return a->freq > b->freq;
    }
};

// ── Encoder ──────────────────────────────────────────────
class HuffmanEncoder {
public:
    // Mã hóa: trả về {chuỗi bit, bảng mã Huffman}
    static pair<string, map<char, string>> encode(const string& text);

    // Giải mã từ chuỗi bit + bảng mã
    static string decode(const string& encoded,
        const map<char, string>& codes);

    // Demo đầy đủ: in cây, bảng mã, kết quả, thống kê
    static void demo(const string& text);

private:
    static void buildCodes(shared_ptr<HuffmanNode> node,
        const string& code,
        map<char, string>& codes);
};
