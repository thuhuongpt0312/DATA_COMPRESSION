#include "fixed_length.h"
#include "utils.h"
#include <vector>
#include <algorithm>
#include <bitset>
#include <cmath>

pair<string, map<char, string>> FixedLengthEncoder::encode(const string& text) {
    // Thu thập ký tự duy nhất
    vector<char> chars;
    map<char, bool> seen;
    for (char c : text) {
        if (!seen[c]) { seen[c] = true; chars.push_back(c); }
    }
    sort(chars.begin(), chars.end());

    int n = chars.size();
    int bits = max(1, (int)ceil(log2(n + 1)));

    map<char, string> codeTable;
    for (int i = 0; i < n; i++) {
        string code = bitset<8>(i).to_string();
        codeTable[chars[i]] = code.substr(8 - bits);
    }

    string encoded = "";
    for (char c : text) encoded += codeTable[c];

    return { encoded, codeTable };
}

string FixedLengthEncoder::decode(const string& encoded,
    const map<char, string>& codeTable,
    int bitsPerChar) {
    map<string, char> rev;
    for (auto& [c, code] : codeTable) rev[code] = c;

    string decoded = "";
    for (int i = 0; i + bitsPerChar <= (int)encoded.size(); i += bitsPerChar) {
        string chunk = encoded.substr(i, bitsPerChar);
        if (rev.count(chunk)) decoded += rev[chunk];
    }
    return decoded;
}

void FixedLengthEncoder::demo(const string& text) {
    printHeader("THUAT TOAN 1: FIXED-LENGTH ENCODING");
    cout << YELLOW << "  Input: " << RESET << "\"" << text << "\"\n\n";

    auto [encoded, codeTable] = encode(text);
    int n = codeTable.size();
    int bits = max(1, (int)ceil(log2(n + 1)));

    cout << BOLD << "  Bang ma (Fixed " << bits << " bits/ky tu):\n" << RESET;
    printSeparator();
    cout << "  " << setw(10) << "Ky tu" << setw(10) << "Ma\n";
    printSeparator();
    for (auto& [c, code] : codeTable)
        cout << "  " << setw(10) << (c == ' ' ? "SPACE" : string(1, c))
        << setw(10) << code << "\n";
    printSeparator();

    cout << "\n  Chuoi da ma hoa:\n  " << MAGENTA << encoded << RESET << "\n\n";

    string decoded = decode(encoded, codeTable, bits);
    cout << "  Giai ma lai: \"" << decoded << "\"\n";
    cout << "  Kiem tra   : "
        << (decoded == text ? GREEN "OK - Khop!\n" : RED "Loi!\n") << RESET;

    cout << "\n";
    printStats(text, encoded, "bits");
}