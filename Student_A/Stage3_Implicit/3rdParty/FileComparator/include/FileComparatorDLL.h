#pragma once

#ifdef FILECOMPARATOR_EXPORTS
#define FILECOMP_API __declspec(dllexport)
#else
#define FILECOMP_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    FILECOMP_API int CompareFiles(const char* path1, const char* path2);


    FILECOMP_API int GetCommonWords(const char* path1, const char* path2, char* outBuffer, int bufferSize);

    FILECOMP_API double GetSimilarity(const char* path1, const char* path2);

    FILECOMP_API int GetDifferenceCount(const char* path1, const char* path2);

#ifdef __cplusplus
}
#endif