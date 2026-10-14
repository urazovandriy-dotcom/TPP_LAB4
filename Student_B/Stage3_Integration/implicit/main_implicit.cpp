#include "TextAnalyzerDLL.h"
#include <iostream>
#include <fstream>
#include <windows.h>

void createSampleFile(const char* filename)
{
    std::ofstream out(filename);
    out << "Привіт світ! Лабораторна робота з C++ та CMake.\n";
    out << "Тестування неявного зв'язування бібліотеки TextAnalyzer.\n";
    out << "Привіт ще раз усім розробникам.\n";
    out.close();
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "НЕЯВНЕ зв'язування (Implicit Linking) [Студент Б]\n";
    std::cout << "Тестування бібліотеки TextAnalyzerDLL (Студент А)\n";


    const char* testFile = "sample_test.txt";
    createSampleFile(testFile);

    // 1. Стандартні виклики функцій
    std::cout << "--- 1. Базові тести функцій на валідному файлі ---\n";

    int lines = LineCount(testFile);
    std::cout << "[LineCount] Кількість рядків: " << lines << "\n";

    int freqWorld = WordFrequency(testFile, "світ");
    std::cout << "[WordFrequency] Частота слова 'світ': " << freqWorld << "\n";

    int freqChar = CharacterFrequency(testFile, "а");
    std::cout << "[CharacterFrequency] Частота символу 'а': " << freqChar << "\n";

    int linePos = SearchWord(testFile, "динамічний");
    std::cout << "[SearchWord] Слово 'динамічний' знайдено у рядку: " << linePos << "\n";

    char topBuf[1024] = { 0 };
    int topCount = TopWords(testFile, 3, topBuf, sizeof(topBuf));
    std::cout << "[TopWords] Записано топ слів (" << topCount << "):\n" << topBuf << "\n";

    // 2. Тестування границь (Edge Cases за контрактом)
    std::cout << "--- 2. Тестування граничних випадків (Edge Cases / Contract) ---\n";

    std::cout << "WordFrequency(nullptr, 'тест'): " << WordFrequency(nullptr, "тест") << " (очікувано -1)\n";
    std::cout << "WordFrequency(testFile, nullptr): " << WordFrequency(testFile, nullptr) << " (очікувано -1)\n";
    std::cout << "LineCount('non_existing.txt'): " << LineCount("non_existing.txt") << " (очікувано -2)\n";
    std::cout << "TopWords із буфером розміром 0: " << TopWords(testFile, 5, topBuf, 0) << " (очікувано -1)\n";

    std::cout << "\nТестування неявного зв'язування успішно завершено!\n";
    return 0;
}