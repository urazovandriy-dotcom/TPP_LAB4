#include "TextAnalyzerDLL.h"

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

    std::cout << "=== Тест динамічної бібліотеки TextAnalyzerDLL ===\n\n";

    std::cout << "1. Кількість рядків (LineCount): " << LineCount("text.txt") << "\n";
    std::cout << "2. Слово \"привіт\" (WordFrequency): " << WordFrequency("text.txt", "привіт") << "\n";
    std::cout << "3. Буква \"т\" (CharacterFrequency): " << CharacterFrequency("text.txt", "т") << "\n";
    std::cout << "4. Перший рядок зі словом \"cmake\" (SearchWord): " << SearchWord("text.txt", "cmake") << "\n";

    char buffer[256];
    int count = TopWords("text.txt", 3, buffer, sizeof(buffer));
    std::cout << "5. Три найчастіші слова (TopWords), записано " << count << ":\n" << buffer;

    std::cout << "\n--- Граничні випадки ---\n";
    std::cout << "nullptr замість шляху: " << LineCount(nullptr) << " (очікується -1)\n";
    std::cout << "Порожній шлях: " << WordFrequency("", "привіт") << " (очікується -1)\n";
    std::cout << "Неіснуючий файл: " << SearchWord("no_file.txt", "світ") << " (очікується -2)\n";
    std::cout << "Два символи замість одного: " << CharacterFrequency("text.txt", "ab") << " (очікується -1)\n";

    char smallBuffer[20];
    count = TopWords("text.txt", 3, smallBuffer, sizeof(smallBuffer));
    std::cout << "Малий буфер (20 байт), записано " << count << ":\n" << smallBuffer;
    return 0;
}
