#include "FileComparator.h"

#include <iostream>
#include <fstream>
#include <windows.h>

void createTestFiles()
{
    std::ofstream f1("file1.txt");
    f1 << "Hello world from C++ CMake\n";
    f1 << "This is line two\n";
    f1 << "Line three is unique\n";
    f1.close();

    std::ofstream f2("file2.txt");
    f2 << "Hello world from C++ Visual Studio\n";
    f2 << "This is line two\n";
    f2 << "Line four has appeared\n";
    f2.close();
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "=== Тест статичної бібліотеки: FileComparator (Студент Б) ===\n\n";

    createTestFiles();

    std::string p1 = "file1.txt";
    std::string p2 = "file2.txt";

    // 1. compareFiles
    bool identical = FileCompare::compareFiles(p1, p2);
    std::cout << "1. compareFiles(file1, file2): " << (identical ? "ОДНАКОВІ" : "РІЗНІ") << "\n";

    bool selfIdentical = FileCompare::compareFiles(p1, p1);
    std::cout << "   compareFiles(file1, file1): " << (selfIdentical ? "ОДНАКОВІ" : "РІЗНІ") << "\n\n";

    // 2. commonWords
    auto common = FileCompare::commonWords(p1, p2);
    std::cout << "2. commonWords count: " << common.size() << "\n   Спільнілова: ";
    for (const auto& w : common)
    {
        std::cout << "[" << w << "] ";
    }
    std::cout << "\n\n";

    // 3. similarity
    double sim = FileCompare::similarity(p1, p2);
    std::cout << "3. similarity(file1, file2): " << (sim * 100.0) << " %\n\n";

    // 4. differenceCount
    int diffs = FileCompare::differenceCount(p1, p2);
    std::cout << "4. differenceCount(file1, file2): " << diffs << " відмінних рядків.\n\n";

    std::cout << "Тестування статичної бібліотеки успішно завершено.\n";
    return 0;
}