#include "huffman.h"
#include "utils.h"
#include <queue>
#include <vector>
#include <algorithm>

// ── Sinh mã đệ quy theo cây ──────────────────────────────
void HuffmanEncoder::buildCodes(shared_ptr<HuffmanNode> node,
    const string& code,
    map<char, string>& codes) {
    if (!node) return;
    if (!node->left && !node->right) {       // Node lá
        codes[node->ch] = code.empty() ? "0" : code;
        return;
    }
    buildCodes(node->left, code + "0", codes);
    buildCodes(node->right, code + "1", codes);
}

// ── Mã hóa ───────────────────────────────────────────────
pair<string, map<char, string>> HuffmanEncoder::encode(const string& text) {
    // Đếm tần suất
    map<char, int> freq;
    for (char c : text) freq[c]++;

    // Xây dựng min-heap
    priority_queue<shared_ptr<HuffmanNode>,
        vector<shared_ptr<HuffmanNode>>,
        CompareNode> pq;
    for (auto& [c, f] : freq)
        pq.push(make_shared<HuffmanNode>(c, f));

    // Gộp từng cặp node nhỏ nhất cho đến khi còn 1 gốc
    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();
        pq.push(make_shared<HuffmanNode>(
            left->freq + right->freq, left, right));
    }

    // Sinh bảng mã từ cây
    map<char, string> codes;
    if (!pq.empty()) buildCodes(pq.top(), "", codes);

    // Mã hóa chuỗi
    string encoded = "";
    for (char c : text) encoded += codes[c];

    return { encoded, codes };
}

// ── Giải mã ──────────────────────────────────────────────
string HuffmanEncoder::decode(const string& encoded,
    const map<char, string>& codes) {
    map<string, char> rev;
    for (auto& [c, code] : codes) rev[code] = c;

    string decoded = "", current = "";
    for (char bit : encoded) {
        current += bit;
        if (rev.count(current)) {
            decoded += rev[current];
            current = "";
        }
    }
    return decoded;
}

// ── Demo ─────────────────────────────────────────────────
void HuffmanEncoder::demo(const string& text) {
    printHeader("THUAT TOAN 3: HUFFMAN CODING");
    cout << YELLOW << "  Input: " << RESET << "\"" << text << "\"\n\n";

    // Đếm tần suất để hiển thị
    map<char, int> freq;
    for (char c : text) freq[c]++;

    auto [encoded, codes] = encode(text);

    // Bảng tần suất + mã - sắp xếp theo tần suất giảm dần
    cout << BOLD << "  Bang tan suat & ma Huffman:\n" << RESET;
    printSeparator();
    cout << "  " << setw(10) << "Ky tu"
        << setw(10) << "Tan suat"
        << setw(16) << "Ma Huffman"
        << setw(10) << "Do dai\n";
    printSeparator();

    vector<pair<int, char>> sorted;
    for (auto& [c, f] : freq) sorted.push_back({ f, c });
    sort(sorted.rbegin(), sorted.rend());

    for (auto& [f, c] : sorted) {
        cout << "  " << setw(10) << (c == ' ' ? "SPACE" : string(1, c))
            << setw(10) << f
            << setw(16) << codes[c]
            << setw(10) << codes[c].size() << " bits\n";
    }
    printSeparator();

    cout << "\n  Chuoi da ma hoa:\n  " << MAGENTA << encoded << RESET << "\n\n";

    string decoded = decode(encoded, codes);
    cout << "  Giai ma lai: \"" << decoded << "\"\n";
    cout << "  Kiem tra   : "
        << (decoded == text ? GREEN "OK - Khop!\n" : RED "Loi!\n") << RESET;

    cout << "\n";
    printStats(text, encoded, "bits");
}