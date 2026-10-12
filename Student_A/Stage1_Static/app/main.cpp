#include "TextAnalyzer.h"

#include <fstream>
#include <iostream>
#include <windows.h>

// створює невеликий тестовий файл
void createDemoFile()
{
    std::ofstream file("text.txt");
    file << "Привіт, світ! Це перший рядок.\n"
            "Другий рядок: привіт ще раз.\n"
            "C++ та CMake - це просто.\n"
            "Світ великий, а привіт короткий.\n";
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    createDemoFile();

    std::cout << "=== Тест статичної бібліотеки TextAnalyzer ===\n\n";

    std::cout << "1. Кількість рядків (lineCount): " << TextAnalyzer::lineCount("text.txt") << "\n";
    std::cout << "2. Слово \"привіт\" (wordFrequency): " << TextAnalyzer::wordFrequency("text.txt", "привіт") << "\n";
    std::cout << "3. Буква \"т\" (characterFrequency): " << TextAnalyzer::characterFrequency("text.txt", "т") << "\n";
    std::cout << "4. Перший рядок зі словом \"cmake\" (searchWord): " << TextAnalyzer::searchWord("text.txt", "cmake") << "\n";

    std::cout << "5. Три найчастіші слова (topWords):\n";
    for (const auto& [word, count] : TextAnalyzer::topWords("text.txt", 3))
        std::cout << "   " << word << " " << count << "\n";

    std::cout << "\n--- Граничні випадки ---\n";
    std::cout << "Неіснуючий файл: " << TextAnalyzer::lineCount("no_file.txt") << " (очікується -2)\n";
    std::cout << "Порожнє слово: " << TextAnalyzer::wordFrequency("text.txt", "") << " (очікується -1)\n";
    std::cout << "Слова немає у файлі: " << TextAnalyzer::searchWord("text.txt", "кіт") << " (очікується 0)\n";
    return 0;
}
