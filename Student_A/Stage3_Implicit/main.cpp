#include "FileComparatorDLL.h"

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

// записує текст у файл як є (переходи рядків не змінюються)
void writeFile(const char* name, const std::string& text)
{
    std::ofstream file(name, std::ios::binary);
    file << text;
}

// створює тестові файли
void createTestFiles()
{
    writeFile("a.txt", "Hello world from CMake\nsecond line\nthird line\n");
    writeFile("a_copy.txt", "Hello world from CMake\nsecond line\nthird line\n");
    writeFile("b.txt", "Hello world from Visual Studio\nsecond line\nanother line\n");
    writeFile("a_crlf.txt", "Hello world from CMake\r\nsecond line\r\nthird line\r\n");
    writeFile("empty.txt", "");
    writeFile("ua1.txt", "Привіт світ\nЯк справи\n");
    writeFile("ua2.txt", "Добрий вечір\nДо побачення\n");

    // два великі файли по 200 000 рядків, кожен 10-й рядок відрізняється
    std::string big1, big2;
    for (int i = 0; i < 200000; i++)
    {
        std::string line = "line " + std::to_string(i) + " some text\n";
        big1 += line;
        big2 += (i % 10 == 0) ? "changed " + line : line;
    }
    writeFile("big1.txt", big1);
    writeFile("big2.txt", big2);
}

// порівнює результат функції з очікуваним
void check(const char* test, double got, double expected)
{
    std::cout << (got == expected ? "[OK]     " : "[НЕ OK]  ") << test
              << ": отримано " << got << ", очікувалось " << expected << "\n";
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    createTestFiles();

    std::cout << "=== Неявне зв'язування: FileComparatorDLL (Студент Б) ===\n\n";

    std::cout << "--- Звичайна робота ---\n";
    std::cout << "CompareFiles(a, a_copy) = " << CompareFiles("a.txt", "a_copy.txt") << "\n";
    std::cout << "CompareFiles(a, b) = " << CompareFiles("a.txt", "b.txt") << "\n";
    char words[256] = "";
    int count = GetCommonWords("a.txt", "b.txt", words, sizeof(words));
    std::cout << "GetCommonWords(a, b) = " << count << ": " << words << "\n";
    std::cout << "GetSimilarity(a, b) = " << GetSimilarity("a.txt", "b.txt") << "\n";
    std::cout << "GetDifferenceCount(a, b) = " << GetDifferenceCount("a.txt", "b.txt") << "\n";

    std::cout << "\n--- Граничні випадки з README ---\n";
    check("CompareFiles(nullptr, b)", CompareFiles(nullptr, "b.txt"), -1);
    check("CompareFiles(\"\", b)", CompareFiles("", "b.txt"), -1);
    check("CompareFiles(неіснуючий файл, b)", CompareFiles("no_file.txt", "b.txt"), -1);
    check("GetCommonWords(a, b, nullptr, 100)", GetCommonWords("a.txt", "b.txt", nullptr, 100), -1);
    check("GetCommonWords(a, b, буфер, 0)", GetCommonWords("a.txt", "b.txt", words, 0), -1);
    check("GetCommonWords(a, b, буфер, -5)", GetCommonWords("a.txt", "b.txt", words, -5), -1);
    check("GetSimilarity(nullptr, b)", GetSimilarity(nullptr, "b.txt"), -1.0);
    check("GetSimilarity(\"\", b)", GetSimilarity("", "b.txt"), -1.0);
    check("GetDifferenceCount(a, nullptr)", GetDifferenceCount("a.txt", nullptr), -1);
    check("GetDifferenceCount(неіснуючий файл, b)", GetDifferenceCount("no_file.txt", "b.txt"), -1);

    std::cout << "\n--- Випадки, яких README не описує ---\n";
    char smallBuffer[8] = "";
    count = GetCommonWords("a.txt", "b.txt", smallBuffer, sizeof(smallBuffer));
    std::cout << "[?]      Малий буфер (8 байт): повернуто " << count << ", у буфері \"" << smallBuffer << "\"\n";
    check("GetSimilarity(empty, empty)", GetSimilarity("empty.txt", "empty.txt"), 1.0);
    check("GetDifferenceCount(empty, a)", GetDifferenceCount("empty.txt", "a.txt"), 3);
    count = GetCommonWords("ua1.txt", "ua1.txt", words, sizeof(words));
    std::cout << "[?]      GetCommonWords(ua1, ua1), українські слова: повернуто " << count << "\n";
    std::cout << "[?]      GetSimilarity(ua1, ua2), українські тексти: повернуто " << GetSimilarity("ua1.txt", "ua2.txt") << "\n";
    check("GetDifferenceCount(a, a_crlf), той самий текст з \\r\\n", GetDifferenceCount("a.txt", "a_crlf.txt"), 0);
    check("CompareFiles(a, a_crlf), той самий текст з \\r\\n", CompareFiles("a.txt", "a_crlf.txt"), 0);

    std::cout << "\n--- Великі файли (200 000 рядків) ---\n";
    ULONGLONG start = GetTickCount64();
    check("GetDifferenceCount(big1, big2)", GetDifferenceCount("big1.txt", "big2.txt"), 20000);
    check("GetSimilarity(big1, big1)", GetSimilarity("big1.txt", "big1.txt"), 1.0);
    std::cout << "Час: " << GetTickCount64() - start << " мс\n";

    std::cout << "\nПрограма не впала на жодному тесті.\n";
    return 0;
}
