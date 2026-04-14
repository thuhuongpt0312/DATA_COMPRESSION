#pragma once
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ANSI colors
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"

void printHeader(const string& title);
void printSeparator();
void printStats(const string& original, const string& compressed, const string& label = "bits");

