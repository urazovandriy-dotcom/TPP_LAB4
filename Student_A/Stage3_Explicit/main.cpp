#include <fstream>
#include <iostream>
#include <windows.h>

// типи функцій з dll напарника (сигнатури взяті з його README)
typedef int (*CompareFilesFunc)(const char*, const char*);
typedef int (*GetCommonWordsFunc)(const char*, const char*, char*, int);
typedef double (*GetSimilarityFunc)(const char*, const char*);
typedef int (*GetDifferenceCountFunc)(const char*, const char*);

// записує текст у файл
void writeFile(const char* name, const char* text)
{
    std::ofstream file(name, std::ios::binary);
    file << text;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    writeFile("a.txt", "Hello world from CMake\nsecond line\nthird line\n");
    writeFile("b.txt", "Hello world from Visual Studio\nsecond line\nanother line\n");

    std::cout << "=== Явне зв'язування: LoadLibrary / GetProcAddress ===\n\n";

    // dll, якої немає: LoadLibrary поверне nullptr, а програма працює далі
    HMODULE missing = LoadLibrary(TEXT("NoSuchLibrary.dll"));
    std::cout << "LoadLibrary(\"NoSuchLibrary.dll\"): " << (missing ? "завантажено" : "не знайдено, програма працює далі") << "\n";

    // 1. завантажуємо dll напарника
    HMODULE lib = LoadLibrary(TEXT("FileComparatorDLL.dll"));
    if (!lib)
    {
        std::cout << "Не вдалося завантажити FileComparatorDLL.dll\n";
        return 1;
    }
    std::cout << "LoadLibrary(\"FileComparatorDLL.dll\"): завантажено\n";

    // 2. шукаємо функції за іменами
    auto compareFiles = (CompareFilesFunc)GetProcAddress(lib, "CompareFiles");
    auto getCommonWords = (GetCommonWordsFunc)GetProcAddress(lib, "GetCommonWords");
    auto getSimilarity = (GetSimilarityFunc)GetProcAddress(lib, "GetSimilarity");
    auto getDifferenceCount = (GetDifferenceCountFunc)GetProcAddress(lib, "GetDifferenceCount");

    // ім'я з помилкою (з малої букви) не знайдеться
    FARPROC wrongName = GetProcAddress(lib, "compareFiles");
    std::cout << "GetProcAddress(\"compareFiles\"): " << (wrongName ? "знайдено" : "nullptr, такої функції немає") << "\n\n";

    // 3. викликаємо функції через покажчики
    if (compareFiles && getCommonWords && getSimilarity && getDifferenceCount)
    {
        char words[256] = "";
        int count = getCommonWords("a.txt", "b.txt", words, sizeof(words));
        std::cout << "CompareFiles(a, b) = " << compareFiles("a.txt", "b.txt") << "\n";
        std::cout << "GetCommonWords(a, b) = " << count << ": " << words << "\n";
        std::cout << "GetSimilarity(a, b) = " << getSimilarity("a.txt", "b.txt") << "\n";
        std::cout << "GetDifferenceCount(a, b) = " << getDifferenceCount("a.txt", "b.txt") << "\n";
        std::cout << "CompareFiles(nullptr, b) = " << compareFiles(nullptr, "b.txt") << "\n";
    }
    else
    {
        std::cout << "Не всі функції знайдено в dll\n";
    }

    // 4. вивантажуємо dll
    FreeLibrary(lib);
    std::cout << "\nFreeLibrary: FileComparatorDLL.dll вивантажено\n";
    return 0;
}
