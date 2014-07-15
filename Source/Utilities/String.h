#pragma once

#include <string>

#define MAX_DIGITS_IN_INT 12  // max number of digits in an int (-2147483647 = 11 digits, +1 for the '\0')

// Does a classic * & ? pattern match on a file name - this is case sensitive!
extern BOOL WildcardMatch(const char *pat, const char *str);

extern std::string ToStr(int num, int base = 10);
extern std::string ToStr(unsigned int num, int base = 10);
extern std::string ToStr(unsigned long num, int base = 10);
extern std::string ToStr(float num);
extern std::string ToStr(double num);
extern std::string ToStr(bool val);

extern std::string ws2s(const std::wstring& s);
extern std::wstring s2ws(const std::string &s);