#include "tinyfiledialogs.c"

const char* selectFolderDialog(const char* title, const char* defaultPath) {
    return tinyfd_selectFolderDialog(title, defaultPath);
}

const char* selectFileDialog(const char* title, const char* defaultPath, int numFilterPatterns, const char* const* filterPatterns, const char* singleFilterDescription) {
    return tinyfd_openFileDialog(title, defaultPath, numFilterPatterns, filterPatterns, singleFilterDescription, 0);
}
