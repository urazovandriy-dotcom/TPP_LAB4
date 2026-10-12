#include "TextAnalyzerDLL.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    // читає весь файл у рядок; повертає 0 або код помилки
    int readFile(const char* path, std::string& text)
    {
        if (!path || path[0] == '\0')
            return -1;

        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
            return -2;

        std::stringstream buffer;
        buffer << file.rdbuf();
        text = buffer.str();
        return 0;
    }

    // розбиває текст на слова в нижньому регістрі (латиниця і кирилиця в UTF-8)
    std::vector<std::string> splitWords(const std::string& text)
    {
        std::vector<std::string> words;
        std::string word;

        for (size_t i = 0; i < text.size(); i++)
        {
            unsigned char c = text[i];
            if (c < 0x80 && std::isalnum(c))
            {
                word += (char)std::tolower(c);
            }
            else if (c >= 0xD0 && c <= 0xD3 && i + 1 < text.size())
            {
                // кирилична буква займає два байти
                unsigned char a = c, b = text[i + 1];
                if (a == 0xD0 && b >= 0x90 && b <= 0x9F) b += 0x20;                  // А-П
                else if (a == 0xD0 && b >= 0xA0 && b <= 0xAF) { a = 0xD1; b -= 0x20; } // Р-Я
                else if (a == 0xD0 && b >= 0x80 && b <= 0x8F) { a = 0xD1; b += 0x10; } // Є, І, Ї
                else if (a == 0xD2 && b == 0x90) b = 0x91;                            // Ґ
                word += (char)a;
                word += (char)b;
                i++;
            }
            else if (!word.empty())
            {
                words.push_back(word);
                word.clear();
            }
        }
        if (!word.empty())
            words.push_back(word);

        return words;
    }

    // перетворює параметр word на одне слово для пошуку; false, якщо це не одне слово
    bool toKey(const char* word, std::string& key)
    {
        if (!word)
            return false;
        std::vector<std::string> words = splitWords(word);
        if (words.size() != 1)
            return false;
        key = words[0];
        return true;
    }

    // довжина символу UTF-8 за першим байтом
    size_t charLength(unsigned char c)
    {
        if (c < 0x80) return 1;
        if (c < 0xE0) return 2;
        if (c < 0xF0) return 3;
        return 4;
    }
}

int WordFrequency(const char* path, const char* word)
{
    std::string key;
    if (!toKey(word, key))
        return -1;

    std::string text;
    int error = readFile(path, text);
    if (error != 0)
        return error;

    int count = 0;
    for (const std::string& w : splitWords(text))
        if (w == key)
            count++;
    return count;
}

int CharacterFrequency(const char* path, const char* symbol)
{
    if (!symbol || symbol[0] == '\0' || std::strlen(symbol) != charLength(symbol[0]))
        return -1;

    std::string text;
    int error = readFile(path, text);
    if (error != 0)
        return error;

    size_t len = std::strlen(symbol);
    int count = 0;
    for (size_t pos = text.find(symbol); pos != std::string::npos; pos = text.find(symbol, pos + len))
        count++;
    return count;
}

int LineCount(const char* path)
{
    std::string text;
    int error = readFile(path, text);
    if (error != 0)
        return error;
    if (text.empty())
        return 0;

    int count = (int)std::count(text.begin(), text.end(), '\n');
    if (text.back() != '\n')
        count++;   // останній рядок без переходу на новий
    return count;
}

int TopWords(const char* path, int n, char* outBuffer, int bufferSize)
{
    if (!outBuffer || bufferSize <= 0)
        return -1;
    outBuffer[0] = '\0';
    if (n <= 0)
        return -1;

    std::string text;
    int error = readFile(path, text);
    if (error != 0)
        return error;

    std::map<std::string, int> counts;
    for (const std::string& w : splitWords(text))
        counts[w]++;

    // спочатку частіші слова, при рівній кількості - за кодами символів
    std::vector<std::pair<std::string, int>> sorted(counts.begin(), counts.end());
    std::stable_sort(sorted.begin(), sorted.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    // у буфер пишемо рядки "слово кількість", лише ті, що вміщаються повністю
    std::string result;
    int written = 0;
    for (const auto& [word, count] : sorted)
    {
        if (written == n)
            break;
        std::string entry = word + " " + std::to_string(count) + "\n";
        if (result.size() + entry.size() + 1 > (size_t)bufferSize)
            break;
        result += entry;
        written++;
    }
    std::memcpy(outBuffer, result.c_str(), result.size() + 1);
    return written;
}

int SearchWord(const char* path, const char* word)
{
    std::string key;
    if (!toKey(word, key))
        return -1;

    std::string text;
    int error = readFile(path, text);
    if (error != 0)
        return error;

    std::stringstream lines(text);
    std::string line;
    for (int number = 1; std::getline(lines, line); number++)
    {
        std::vector<std::string> words = splitWords(line);
        if (std::find(words.begin(), words.end(), key) != words.end())
            return number;
    }
    return 0;
}
