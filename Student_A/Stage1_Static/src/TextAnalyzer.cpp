#include "TextAnalyzer.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <map>
#include <sstream>

namespace
{
    // читає весь файл у рядок; повертає 0 або код помилки
    int readFile(const std::string& path, std::string& text)
    {
        if (path.empty())
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

    // довжина символу UTF-8 за першим байтом
    size_t charLength(unsigned char c)
    {
        if (c < 0x80) return 1;
        if (c < 0xE0) return 2;
        if (c < 0xF0) return 3;
        return 4;
    }
}

namespace TextAnalyzer
{
    int wordFrequency(const std::string& path, const std::string& word)
    {
        std::vector<std::string> key = splitWords(word);
        if (key.size() != 1)
            return -1;

        std::string text;
        int error = readFile(path, text);
        if (error != 0)
            return error;

        int count = 0;
        for (const std::string& w : splitWords(text))
            if (w == key[0])
                count++;
        return count;
    }

    int characterFrequency(const std::string& path, const std::string& symbol)
    {
        if (symbol.empty() || symbol.size() != charLength(symbol[0]))
            return -1;

        std::string text;
        int error = readFile(path, text);
        if (error != 0)
            return error;

        int count = 0;
        for (size_t pos = text.find(symbol); pos != std::string::npos; pos = text.find(symbol, pos + symbol.size()))
            count++;
        return count;
    }

    int lineCount(const std::string& path)
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

    std::vector<std::pair<std::string, int>> topWords(const std::string& path, int n)
    {
        std::vector<std::pair<std::string, int>> result;
        std::string text;
        if (n <= 0 || readFile(path, text) != 0)
            return result;

        std::map<std::string, int> counts;
        for (const std::string& w : splitWords(text))
            counts[w]++;

        result.assign(counts.begin(), counts.end());
        // спочатку частіші слова, при рівній кількості - за кодами символів
        std::stable_sort(result.begin(), result.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
        if ((int)result.size() > n)
            result.resize(n);
        return result;
    }

    int searchWord(const std::string& path, const std::string& word)
    {
        std::vector<std::string> key = splitWords(word);
        if (key.size() != 1)
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
            if (std::find(words.begin(), words.end(), key[0]) != words.end())
                return number;
        }
        return 0;
    }
}
