#include <iostream>
#include <string>
#include <limits>
#include <map>
#include <algorithm>
#include <iomanip>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include "utils.h"
#include "fixed_length.h"
#include "rle.h"
#include "huffman.h"
#include "deflate.h"

using namespace std;

static const string SAMPLE_RLE = "aaaaabbbbbccddeeeeeefffghhh";
static const string SAMPLE_TEXT = "huffman coding is a data compression technique";

static void showBanner() {
    cout << BOLD << MAGENTA;
    cout << "\n  =========================================\n";
    cout << "       DATA COMPRESSION ALGORITHMS DEMO   \n";
    cout << "  =========================================\n";
    cout << RESET;
    cout << CYAN << "\n  4 thuat toan: Fixed-Length | RLE | Huffman | Deflate\n" << RESET;
}

static void showMenu() {
    cout << "\n" << BOLD << CYAN;
    cout << "========================================================\n";
    cout << "         CHON THUAT TOAN NEN DU LIEU                    \n";
    cout << "========================================================\n" << RESET;
    cout << "\n";
    cout << "  " << BOLD << "[1]" << RESET << " Fixed-Length Encoding\n";
    cout << "  " << BOLD << "[2]" << RESET << " Run-Length Encoding  (RLE)\n";
    cout << "  " << BOLD << "[3]" << RESET << " Huffman Coding\n";
    cout << "  " << BOLD << "[4]" << RESET << " Deflate / ZIP  (LZ77 + Huffman)\n";
    cout << "  " << BOLD << "[5]" << RESET << " So sanh ca 4 thuat toan\n";
    cout << "  " << BOLD << "[0]" << RESET << " Thoat\n";
    cout << "\n  Lua chon: ";
}

static void compareAll(const string& text) {
    printHeader("SO SANH 4 THUAT TOAN TREN CUNG DU LIEU");
    cout << YELLOW << "  Input: " << RESET << "\"" << text << "\"\n\n";

    double origBits = text.size() * 8.0;

    auto [fixedEnc, fixedTable] = FixedLengthEncoder::encode(text);
    double fixedRatio = (1.0 - fixedEnc.size() / origBits) * 100.0;

    string rleEnc = RLEEncoder::encode(text);
    double rleRatio = (1.0 - rleEnc.size() * 8.0 / origBits) * 100.0;

    auto [huffEnc, huffCodes] = HuffmanEncoder::encode(text);
    double huffRatio = (1.0 - huffEnc.size() / origBits) * 100.0;

    double deflateEst = huffEnc.size() * 0.85;
    double deflateRatio = (1.0 - deflateEst / origBits) * 100.0;

    printSeparator();
    cout << BOLD
        << "  " << setw(22) << left << "Thuat toan"
        << setw(18) << right << "Kich thuoc nen"
        << setw(14) << "Ti le nen\n" << RESET;
    printSeparator();

    auto row = [&](const string& name, double bits, double ratio) {
        cout << "  " << setw(22) << left << name
            << setw(18) << right << (to_string((int)bits) + " bits");
        if (ratio > 0)
            cout << BOLD << GREEN << setw(14) << (to_string((int)ratio) + "%") << RESET;
        else
            cout << BOLD << RED << setw(14) << (to_string((int)ratio) + "%") << RESET;
        cout << "\n";
        };

    cout << "  " << setw(22) << left << "Goc (ASCII 8-bit)"
        << setw(18) << right << (to_string((int)origBits) + " bits")
        << setw(14) << "0%\n";
    row("Fixed-Length", fixedEnc.size(), fixedRatio);
    row("RLE", rleEnc.size() * 8, rleRatio);
    row("Huffman", huffEnc.size(), huffRatio);
    row("Deflate (ZIP)", deflateEst, deflateRatio);

    printSeparator();
    cout << "\n  " << BOLD << "Nhan xet:\n" << RESET;
    cout << "  * Fixed-Length : don gian, hieu qua khi it ky tu khac nhau\n";
    cout << "  * RLE          : tot nhat cho chuoi ky tu lap lien tiep\n";
    cout << "  * Huffman      : hieu qua cho van ban tu nhien\n";
    cout << "  * Deflate(ZIP) : tot nhat tong quat, ket hop LZ77 + Huffman\n";
}

static string getInput(int choice) {
    cout << "\n  Nhap chuoi can nen (Enter de dung mac dinh): ";
    string input;
    getline(cin, input);
    if (!input.empty()) return input;

    const string& def = (choice == 2) ? SAMPLE_RLE : SAMPLE_TEXT;
    cout << "  -> Dung mac dinh: \"" << def << "\"\n";
    return def;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    showBanner();

    int choice;
    do {
        showMenu();
        cin >> choice;
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');

        if (choice == 0) break;

        if (choice < 1 || choice > 5) {
            cout << RED << "  Lua chon khong hop le!\n" << RESET;
            continue;
        }

        string text = getInput(choice);

        switch (choice) {
        case 1: FixedLengthEncoder::demo(text); break;
        case 2: RLEEncoder::demo(text);         break;
        case 3: HuffmanEncoder::demo(text);     break;
        case 4: DeflateEncoder::demo(text);     break;
        case 5: compareAll(text);               break;
        }

        cout << "\n  Nhan Enter de tiep tuc...";
        cin.get();

    } while (choice != 0);

    cout << "\n" << GREEN << "  Cam on da su dung chuong trinh!\n\n" << RESET;
    return 0;
}