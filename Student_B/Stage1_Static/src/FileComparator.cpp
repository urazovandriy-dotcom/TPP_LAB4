#include "FileComparator.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <cctype>

namespace FileCompare
{
    // Допоміжна функція нормалізації слів (нижній регістр, без пунктуації)
    static std::string normalizeWord(const std::string& raw)
    {
        std::string cleaned;
        for (char ch : raw)
        {
            if (std::isalnum(static_cast<unsigned char>(ch)))
            {
                cleaned += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
            }
        }
        return cleaned;
    }

    // Допоміжна функція зчитування всіх слів із файлу у множину
    static std::unordered_set< std::string > getUniqueWords(const std::string& path)
    {
        std::unordered_set< std::string > words;
        std::ifstream file(path);
        if (!file.is_open())
        {
            return words;
        }

        std::string token;
        while (file >> token)
        {
            std::string cleaned = normalizeWord(token);
            if (!cleaned.empty())
            {
                words.insert(cleaned);
            }
        }
        return words;
    }

    // Допоміжна функція зчитування всіх рядків файлу
    static std::vector< std::string > getLines(const std::string& path)
    {
        std::vector< std::string > lines;
        std::ifstream file(path);
        if (!file.is_open())
        {
            return lines;
        }

        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
        return lines;
    }

    // 1. Повне порівняння файлів
    bool compareFiles(const std::string& path1, const std::string& path2)
    {
        std::ifstream f1(path1, std::ios::binary);
        std::ifstream f2(path2, std::ios::binary);

        if (!f1.is_open() || !f2.is_open())
        {
            return false;
        }

        std::istreambuf_iterator< char > it1(f1), end1;
        std::istreambuf_iterator< char > it2(f2), end2;

        return std::equal(it1, end1, it2, end2);
    }

    // 2. Спільні слова
    std::vector< std::string > commonWords(const std::string& path1, const std::string& path2)
    {
        auto set1 = getUniqueWords(path1);
        auto set2 = getUniqueWords(path2);

        std::vector< std::string > common;
        for (const auto& w : set1)
        {
            if (set2.find(w) != set2.end())
            {
                common.push_back(w);
            }
        }
        std::sort(common.begin(), common.end());
        return common;
    }

    // 3. Коефіцієнт схожості за формулою Жаккара (Jaccard similarity)
    double similarity(const std::string& path1, const std::string& path2)
    {
        auto set1 = getUniqueWords(path1);
        auto set2 = getUniqueWords(path2);

        if (set1.empty() && set2.empty())
        {
            return 1.0;
        }
        if (set1.empty() || set2.empty())
        {
            return 0.0;
        }

        size_t intersectionCount = 0;
        for (const auto& w : set1)
        {
            if (set2.find(w) != set2.end())
            {
                intersectionCount++;
            }
        }

        // Розмір об'єднання: |A| + |B| - |A ∩ B|
        size_t unionCount = set1.size() + set2.size() - intersectionCount;
        return unionCount == 0 ? 0.0 : (static_cast<double>(intersectionCount) / unionCount);
    }

    // 4. Кількість рядків, які відрізняються
    int differenceCount(const std::string& path1, const std::string& path2)
    {
        auto lines1 = getLines(path1);
        auto lines2 = getLines(path2);

        int diff = 0;
        size_t minSize = std::min(lines1.size(), lines2.size());

        for (size_t i = 0; i < minSize; i++)
        {
            if (lines1[i] != lines2[i])
            {
                diff++;
            }
        }

        // Рядки, яких немає в одному з файлів, також вважаються різницею
        diff += static_cast<int>(std::max(lines1.size(), lines2.size()) - minSize);
        return diff;
    }
}