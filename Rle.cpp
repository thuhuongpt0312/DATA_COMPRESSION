#include "rle.h"
#include "utils.h"

string RLEEncoder::encode(const string& text) {
    if (text.empty()) return "";
    string result = "";
    int count = 1;
    for (int i = 1; i < (int)text.size(); i++) {
        if (text[i] == text[i - 1]) {
            count++;
        }
        else {
            if (count > 1) result += to_string(count);
            result += text[i - 1];
            count = 1;
        }
    }
    if (count > 1) result += to_string(count);
    result += text.back();
    return result;
}

string RLEEncoder::decode(const string& encoded) {
    string result = "", numStr = "";
    for (char c : encoded) {
        if (isdigit(c)) {
            numStr += c;
        }
        else {
            int count = numStr.empty() ? 1 : stoi(numStr);
            for (int i = 0; i < count; i++) result += c;
            numStr = "";
        }
    }
    return result;
}

void RLEEncoder::demo(const string& text) {
    printHeader("THUAT TOAN 2: RUN-LENGTH ENCODING (RLE)");
    cout << YELLOW << "  Input: " << RESET << "\"" << text << "\"\n\n";

    cout << BOLD << "  Phan tich cac chuoi ky tu lien tiep:\n" << RESET;
    printSeparator();
    int i = 0;
    while (i < (int)text.size()) {
        int count = 1;
        while (i + count < (int)text.size() && text[i + count] == text[i]) count++;
        cout << "  '" << text[i] << "' x" << count;
        if (count > 1) cout << "  ->  " << count << text[i];
        else           cout << "  ->  " << text[i];
        cout << "\n";
        i += count;
    }
    printSeparator();

    string encoded = encode(text);
    string decoded = decode(encoded);

    cout << "\n  Chuoi da nen (RLE): " << MAGENTA << "\"" << encoded << "\"" << RESET << "\n";
    cout << "  Giai ma lai       : \"" << decoded << "\"\n";
    cout << "  Kiem tra          : "
        << (decoded == text ? GREEN "OK - Khop!\n" : RED "Loi!\n") << RESET;

    cout << "\n";
    double origSize = text.size();
    double compSize = encoded.size();
    double ratio = (1.0 - compSize / origSize) * 100.0;
    cout << GREEN << "  ✓ Kich thuoc goc  : " << RESET << origSize << " ky tu\n";
    cout << GREEN << "  ✓ Kich thuoc nen  : " << RESET << compSize << " ky tu\n";
    cout << GREEN << "  ✓ Ti le nen       : " << RESET << BOLD;
    if (ratio > 0) cout << GREEN << fixed << setprecision(1) << ratio << "%" << RESET;
    else           cout << RED << fixed << setprecision(1) << ratio << "% (lon hon goc)" << RESET;
    cout << "\n";
}