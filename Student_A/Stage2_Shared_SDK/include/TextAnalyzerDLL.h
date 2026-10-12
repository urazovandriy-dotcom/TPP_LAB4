#pragma once

// Аналіз текстових файлів, версія API 1.0 (Студент А, варіант 14)
// Опис функцій і правила використання є у SDK/README.md

#ifdef TEXTANALYZER_EXPORTS
#define TEXTANALYZER_API extern "C" __declspec(dllexport)
#else
#define TEXTANALYZER_API extern "C" __declspec(dllimport)
#endif

TEXTANALYZER_API int WordFrequency(const char* path, const char* word);
TEXTANALYZER_API int CharacterFrequency(const char* path, const char* symbol);
TEXTANALYZER_API int LineCount(const char* path);
TEXTANALYZER_API int TopWords(const char* path, int n, char* outBuffer, int bufferSize);
TEXTANALYZER_API int SearchWord(const char* path, const char* word);
