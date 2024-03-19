#ifndef __Html__
#define __Html__

namespace HTML {

void ShowHTML(const cStr &Html, int x = -1, int y = -1, int w = -1, int h = -1);
void ShowURL(const char *URL, int x = -1, int y = -1, int w = -1, int h = -1);
void ShowRelativePath(const char *RelativePath, int x = -1, int y = -1, int w = -1, int h = -1);
void Close();
bool IsActive();

void Test();

} // HTML

#endif // __Html__