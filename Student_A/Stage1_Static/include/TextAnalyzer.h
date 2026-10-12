#pragma once

#include <string>
#include <utility>
#include <vector>

// Аналіз текстових файлів (Студент А, варіант 14)
// Файли читаються як UTF-8. Коди помилок: -1 (неправильні параметри), -2 (файл не відкрився).
namespace TextAnalyzer
{
    // скільки разів слово трапляється у файлі (без урахування регістру)
    int wordFrequency(const std::string& path, const std::string& word);

    // скільки разів символ трапляється у файлі (з урахуванням регістру)
    int characterFrequency(const std::string& path, const std::string& symbol);

    // кількість рядків у файлі
    int lineCount(const std::string& path);

    // n найчастіших слів: пари (слово, кількість), від найчастішого; при помилці порожній список
    std::vector<std::pair<std::string, int>> topWords(const std::string& path, int n);

    // номер першого рядка, де є слово (рахуємо з 1); 0, якщо слова немає
    int searchWord(const std::string& path, const std::string& word);
}
