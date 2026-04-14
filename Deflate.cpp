#include "deflate.h"
#include "huffman.h"
#include "utils.h"
#include <algorithm>

// ── Bước 1: LZ77 ─────────────────────────────────────────
vector<LZ77Token> DeflateEncoder::lz77Encode(const string& text) {
    vector<LZ77Token> tokens;
    int i = 0;

    while (i < (int)text.size()) {
        int bestLen = 0, bestOff = 0;
        int start = max(0, i - WINDOW_SIZE);

        // Tìm chuỗi khớp dài nhất trong sliding window
        for (int j = start; j < i; j++) {
            int len = 0;
            while (len < MAX_MATCH
                && i + len < (int)text.size()
                && text[j + len] == text[i + len]) {
                len++;
            }
            if (len > bestLen) {
                bestLen = len;
                bestOff = i - j;
            }
        }

        if (bestLen >= 3) {
            // Back-reference: đủ dài để tiết kiệm
            char next = (i + bestLen < (int)text.size())
                ? text[i + bestLen] : '\0';
            tokens.push_back({ bestOff, bestLen, next });
            i += bestLen + 1;
        }
        else {
            // Literal: ghi nguyên ký tự
            tokens.push_back({ 0, 0, text[i] });
            i++;
        }
    }
    return tokens;
}

// ── Demo ─────────────────────────────────────────────────
void DeflateEncoder::demo(const string& text) {
    printHeader("THUAT TOAN 4: DEFLATE / ZIP (LZ77 + Huffman)");
    cout << YELLOW << "  Input: " << RESET << "\"" << text << "\"\n\n";

    // ── Bước 1: LZ77 ─────────────────────────────────────
    cout << BOLD << "  [Buoc 1] LZ77 - Tim chuoi lap trong sliding window:\n" << RESET;
    printSeparator();
    cout << "  " << setw(10) << "Loai"
        << setw(12) << "Offset"
        << setw(12) << "Length"
        << setw(10) << "Ky tu\n";
    printSeparator();

    auto tokens = lz77Encode(text);
    int literalCount = 0, matchCount = 0;

    for (auto& t : tokens) {
        if (t.length == 0) {
            literalCount++;
            cout << "  " << setw(10) << GREEN "LITERAL" RESET
                << setw(12) << "-"
                << setw(12) << "-"
                << setw(10) << (t.next == '\0' ? "EOF" : string(1, t.next)) << "\n";
        }
        else {
            matchCount++;
            cout << "  " << setw(10) << CYAN "MATCH  " RESET
                << setw(12) << t.offset
                << setw(12) << t.length
                << setw(10) << (t.next == '\0' ? "EOF" : string(1, t.next)) << "\n";
        }
    }
    printSeparator();
    cout << "  Tong token: " << tokens.size()
        << " (" << literalCount << " literal, " << matchCount << " match)\n";

    // ── Bước 2: Huffman trên output LZ77 ─────────────────
    cout << "\n" << BOLD << "  [Buoc 2] Huffman Coding tren du lieu LZ77:\n" << RESET;

    // Xây dựng chuỗi ký hiệu để đưa vào Huffman
    string huffInput = "";
    for (auto& t : tokens) {
        if (t.length == 0) {
            huffInput += t.next;
        }
        else {
            huffInput += (char)('0' + (t.offset % 10));
            huffInput += (char)('0' + (t.length % 10));
            if (t.next != '\0') huffInput += t.next;
        }
    }

    auto [huffEncoded, huffCodes] = HuffmanEncoder::encode(
        huffInput.empty() ? text : huffInput);

    cout << "  So ky hieu Huffman : " << huffCodes.size() << "\n";
    cout << "  Bit stream (Huffman output):\n  " << MAGENTA;
    if (huffEncoded.size() > 80)
        cout << huffEncoded.substr(0, 80) << "..." << RESET << "\n";
    else
        cout << huffEncoded << RESET << "\n";

    // ── Thống kê ─────────────────────────────────────────
    cout << "\n";
    double origBits = text.size() * 8.0;
    double finalBits = huffEncoded.size();
    double compRatio = (1.0 - finalBits / origBits) * 100.0;

    cout << GREEN << "  ✓ Kich thuoc goc        : " << RESET
        << text.size() << " ky tu (" << (int)origBits << " bits)\n";
    cout << GREEN << "  ✓ Sau LZ77 (tokens)     : " << RESET
        << tokens.size() << " tokens\n";
    cout << GREEN << "  ✓ Sau Huffman (bits)    : " << RESET
        << (int)finalBits << " bits\n";
    cout << GREEN << "  ✓ Ti le nen tong        : " << RESET << BOLD;
    if (compRatio > 0)
        cout << GREEN << fixed << setprecision(1) << compRatio << "%" << RESET;
    else
        cout << RED << fixed << setprecision(1) << compRatio
        << "% (tang kich thuoc)" << RESET;
    cout << "\n";

    cout << "\n  " << BOLD << "Quy trinh DEFLATE:\n" << RESET;
    cout << "  Du lieu goc  ->  [LZ77: tim & thay the chuoi lap]\n";
    cout << "               ->  [Huffman: ma hoa theo tan suat]\n";
    cout << "               ->  Bit stream nen cuoi cung\n";
}
