#include "FileComparatorDLL.h"

#include <iostream>
#include <fstream>
#include <windows.h>

void createDemoFiles()
{
    std::ofstream f1("file1.txt");
    f1 << "Привіт світ від C++ та CMake\nЦе другий рядок\n";
    f1.close();

    std::ofstream f2("file2.txt");
    f2 << "Привіт світ від C++ та Visual Studio\nЦе другий рядок\n";
    f2.close();
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "=== Тестування Динамічної Бібліотеки (C-ABI / DLL) ===\n\n";

    createDemoFiles();

    int cmp = CompareFiles("file1.txt", "file2.txt");
    std::cout << "1. CompareFiles(file1, file2): " << (cmp == 1 ? "ОДНАКОВІ" : "РІЗНІ") << "\n";

    char words[512] = { 0 };
    int count = GetCommonWords("file1.txt", "file2.txt", words, sizeof(words));
    std::cout << "2. GetCommonWords: кількість = " << count << ", слова: [" << words << "]\n";

    double sim = GetSimilarity("file1.txt", "file2.txt");
    std::cout << "3. GetSimilarity: " << (sim * 100.0) << " %\n";

    int diff = GetDifferenceCount("file1.txt", "file2.txt");
    std::cout << "4. GetDifferenceCount: " << diff << " рядків\n\n";

    std::cout << "--- Перевірка граничних випадків (Edge Cases) ---\n";
    std::cout << "Передача nullptr до CompareFiles: " << CompareFiles(nullptr, "file2.txt") << " (очікувано -1)\n";
    std::cout << "Неіснуючий файл у GetSimilarity: " << GetSimilarity("not_exist.txt", "file2.txt") << " (очікувано -1.0)\n";
    std::cout << "nullptr буфер у GetCommonWords: " << GetCommonWords("file1.txt", "file2.txt", nullptr, 100) << " (очікувано -1)\n";

    std::cout << "\nУсі граничні випадки опрацьовано безпечно без збоїв програми.\n";
    return 0;
}