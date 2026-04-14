#include "utils.h"

void printHeader(const string& title) {
    cout << "\n" << BOLD << CYAN;
    cout << "========================================================\n";
    cout << "  " << left << setw(54) << title << "\n";
    cout << "========================================================\n";
    cout << RESET;
}

void printSeparator() {
    cout << CYAN << "--------------------------------------------------------\n" << RESET;
}

void printStats(const string& original, const string& compressed, const string& label) {
    double origSize = original.size() * 8.0;
    double compSize = compressed.size();
    double ratio = (origSize > 0) ? (1.0 - compSize / origSize) * 100.0 : 0;

    cout << GREEN << "  [OK] Kich thuoc goc  : " << RESET << original.size() << " ky tu (" << (int)origSize << " bits)\n";
    cout << GREEN << "  [OK] Kich thuoc nen  : " << RESET << compSize << " " << label << "\n";
    cout << GREEN << "  [OK] Ti le nen       : " << RESET << BOLD;
    if (ratio > 0)
        cout << GREEN << fixed << setprecision(1) << ratio << "%" << RESET << "\n";
    else
        cout << RED << fixed << setprecision(1) << ratio << "% (lon hon goc)" << RESET << "\n";
}