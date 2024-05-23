#ifndef FILEDIALOGS_H
#define FILEDIALOGS_H

#ifdef __cplusplus
extern "C" {
#endif

	const char* selectFolderDialog(const char* title, const char* defaultPath);
	const char* selectFileDialog(const char* title, const char* defaultPath, int numFilterPatterns, const char* const* filterPatterns, const char* singleFilterDescription);

#ifdef __cplusplus
}
#endif

#endif // FILEDIALOGS_H
