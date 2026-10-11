#pragma once

#include <string>
#include <vector>

namespace FileCompare
{
    // 1. Повне порівняння двох файлів: true, якщо вміст ідентичний
    bool compareFiles(const std::string& path1, const std::string& path2);

    // 2. Пошук спільних слів між двома текстовими файлами (без повторів)
    std::vector< std::string > commonWords(const std::string& path1, const std::string& path2);

    // 3. Коефіцієнт схожості файлів за спільними словами (від 0.0 до 1.0)
    double similarity(const std::string& path1, const std::string& path2);

    // 4. Підрахунок кількості рядків, які відрізняються
    int differenceCount(const std::string& path1, const std::string& path2);
}