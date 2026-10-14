#include <iostream>
#include <fstream>
#include <windows.h>

// Оголошення типів покажчиків на функції бібліотеки (C-ABI)
typedef int (*WordFrequencyFunc)(const char*, const char*);
typedef int (*CharacterFrequencyFunc)(const char*, const char*);
typedef int (*LineCountFunc)(const char*);
typedef int (*TopWordsFunc)(const char*, int, char*, int);
typedef int (*SearchWordFunc)(const char*, const char*);

void createSampleFile(const char* filename)
{
    std::ofstream out(filename);
    out << "Привіт світ! Лабораторна робота з C++ та CMake.\n";
    out << "Тестування явного зв'язування бібліотеки TextAnalyzer.\n";
    out << "Привіт ще раз усім розробникам.\n";
    out.close();
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "ЯВНЕ зв'язування (Explicit Linking / Run-time) [Студент Б]\n";
    std::cout << "Тестування бібліотеки TextAnalyzerDLL (Студент А)\n";

    const char* dllName = "TextAnalyzerDLL.dll";
    std::cout << "Спроба завантаження бібліотеки '" << dllName << "' через LoadLibrary...\n";

    // 1. Динамічне завантаження DLL у пам'ять
    HMODULE hDll = LoadLibraryA(dllName);
    if (!hDll)
    {
        std::cerr << "ПОМИЛКА: Не вдалося завантажити " << dllName << "! Код: " << GetLastError() << "\n";
        return 1;
    }
    std::cout << "Бібліотеку успішно завантажено в пам'ять процесу!\n\n";

    // 2. Отримання адрес функцій через GetProcAddress
    auto pWordFrequency = (WordFrequencyFunc)GetProcAddress(hDll, "WordFrequency");
    auto pCharacterFrequency = (CharacterFrequencyFunc)GetProcAddress(hDll, "CharacterFrequency");
    auto pLineCount = (LineCountFunc)GetProcAddress(hDll, "LineCount");
    auto pTopWords = (TopWordsFunc)GetProcAddress(hDll, "TopWords");
    auto pSearchWord = (SearchWordFunc)GetProcAddress(hDll, "SearchWord");

    if (!pWordFrequency || !pCharacterFrequency || !pLineCount || !pTopWords || !pSearchWord)
    {
        std::cerr << "ПОМИЛКА: Не вдалося отримати адресу однієї з функцій через GetProcAddress!\n";
        FreeLibrary(hDll);
        return 1;
    }

    const char* testFile = "sample_explicit.txt";
    createSampleFile(testFile);

    // 3. Виклики отриманих функцій через покажчики
    std::cout << "--- 1. Результати викликів через покажчики ---\n";
    std::cout << "[LineCount] Рядків: " << pLineCount(testFile) << "\n";
    std::cout << "[WordFrequency] Частота 'привіт': " << pWordFrequency(testFile, "привіт") << "\n";
    std::cout << "[CharacterFrequency] Частота 'с': " << pCharacterFrequency(testFile, "с") << "\n";
    std::cout << "[SearchWord] Слово 'програмування' у рядку: " << pSearchWord(testFile, "програмування") << "\n";

    char topBuf[1024] = { 0 };
    int topCount = pTopWords(testFile, 2, topBuf, sizeof(topBuf));
    std::cout << "[TopWords] Топ слів (" << topCount << "):\n" << topBuf << "\n";

    // 4. Перевірка Edge Cases
    std::cout << "--- 2. Перевірка контракту на граничних даних ---\n";
    std::cout << "WordFrequency(nullptr, 'test'): " << pWordFrequency(nullptr, "test") << " (очікувано -1)\n";
    std::cout << "LineCount('absent_file.txt'): " << pLineCount("absent_file.txt") << " (очікувано -2)\n";

    // 5. Вивантаження бібліотеки
    FreeLibrary(hDll);
    std::cout << "\nБібліотеку успішно вивантажено з пам'яті через FreeLibrary.\n";
    std::cout << "Тестування явного зв'язування успішно завершено!\n";

    return 0;
}