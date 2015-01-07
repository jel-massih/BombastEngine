#pragma once

#include <string>

class Vec3;

#define MAX_DIGITS_IN_INT 12  // max number of digits in an int (-2147483647 = 11 digits, +1 for the '\0')

// Does a classic * & ? pattern match on a file name - this is case sensitive!
extern BOOL WildcardMatch(const char *pat, const char *str);

extern std::string ToStr(int num, int base = 10);
extern std::string ToStr(unsigned int num, int base = 10);
extern std::string ToStr(unsigned long num, int base = 10);
extern std::string ToStr(float num);
extern std::string ToStr(double num);
extern std::string ToStr(bool val);
extern std::string ToStr(const Vec3& val);

extern std::string ws2s(const std::wstring& s);
extern std::wstring s2ws(const std::string &s);

#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U

//Super simple 32 bit FNV Hashing implementation
extern uint32_t HashString(const char * s);