#include "FileComparatorDLL.h"

#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <cstring>

namespace
{
    std::string normalize(const std::string& raw)
    {
        std::string res;
        for (char ch : raw)
        {
            if (std::isalnum(static_cast<unsigned char>(ch)))
            {
                res += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
            }
        }
        return res;
    }

    std::unordered_set<std::string> loadWords(const char* path, bool& ok)
    {
        std::unordered_set<std::string> words;
        if (!path || std::strlen(path) == 0)
        {
            ok = false;
            return words;
        }

        std::ifstream file(path);
        if (!file.is_open())
        {
            ok = false;
            return words;
        }

        std::string w;
        while (file >> w)
        {
            std::string cleaned = normalize(w);
            if (!cleaned.empty())
            {
                words.insert(cleaned);
            }
        }
        ok = true;
        return words;
    }

    std::vector<std::string> loadLines(const char* path, bool& ok)
    {
        std::vector<std::string> lines;
        if (!path || std::strlen(path) == 0)
        {
            ok = false;
            return lines;
        }

        std::ifstream file(path);
        if (!file.is_open())
        {
            ok = false;
            return lines;
        }

        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
        ok = true;
        return lines;
    }
}

int CompareFiles(const char* path1, const char* path2)
{
    if (!path1 || !path2 || std::strlen(path1) == 0 || std::strlen(path2) == 0)
    {
        return -1;
    }

    std::ifstream f1(path1, std::ios::binary);
    std::ifstream f2(path2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open())
    {
        return -1;
    }

    std::istreambuf_iterator<char> it1(f1), end1;
    std::istreambuf_iterator<char> it2(f2), end2;

    return std::equal(it1, end1, it2, end2) ? 1 : 0;
}

int GetCommonWords(const char* path1, const char* path2, char* outBuffer, int bufferSize)
{
    if (!path1 || !path2 || !outBuffer || bufferSize <= 0)
    {
        return -1;
    }

    bool ok1 = false, ok2 = false;
    auto set1 = loadWords(path1, ok1);
    auto set2 = loadWords(path2, ok2);

    if (!ok1 || !ok2)
    {
        return -1;
    }

    std::vector<std::string> common;
    for (const auto& w : set1)
    {
        if (set2.find(w) != set2.end())
        {
            common.push_back(w);
        }
    }
    std::sort(common.begin(), common.end());

    std::string joined;
    for (size_t i = 0; i < common.size(); ++i)
    {
        joined += common[i];
        if (i + 1 < common.size())
        {
            joined += " ";
        }
    }

    if (static_cast<int>(joined.size()) >= bufferSize)
    {
        std::strncpy(outBuffer, joined.c_str(), bufferSize - 1);
        outBuffer[bufferSize - 1] = '\0';
    }
    else
    {
        std::strcpy(outBuffer, joined.c_str());
    }

    return static_cast<int>(common.size());
}

double GetSimilarity(const char* path1, const char* path2)
{
    if (!path1 || !path2)
    {
        return -1.0;
    }

    bool ok1 = false, ok2 = false;
    auto set1 = loadWords(path1, ok1);
    auto set2 = loadWords(path2, ok2);

    if (!ok1 || !ok2)
    {
        return -1.0;
    }

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

    size_t unionCount = set1.size() + set2.size() - intersectionCount;
    return unionCount == 0 ? 0.0 : (static_cast<double>(intersectionCount) / unionCount);
}

int GetDifferenceCount(const char* path1, const char* path2)
{
    if (!path1 || !path2)
    {
        return -1;
    }

    bool ok1 = false, ok2 = false;
    auto lines1 = loadLines(path1, ok1);
    auto lines2 = loadLines(path2, ok2);

    if (!ok1 || !ok2)
    {
        return -1;
    }

    int diff = 0;
    size_t minSize = std::min(lines1.size(), lines2.size());

    for (size_t i = 0; i < minSize; ++i)
    {
        if (lines1[i] != lines2[i])
        {
            diff++;
        }
    }

    diff += static_cast<int>(std::max(lines1.size(), lines2.size()) - minSize);
    return diff;
}