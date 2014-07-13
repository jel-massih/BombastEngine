#include "String.h"

using std::string;

string ToStr(int num, int base)
{
	char str[MAX_DIGITS_IN_INT];
	memset(str, 0, MAX_DIGITS_IN_INT);
	_itoa_s(num, str, MAX_DIGITS_IN_INT, base);
	return (string(str));
}  // end ToStr()

string ToStr(unsigned int num, int base)
{
	char str[MAX_DIGITS_IN_INT];
	memset(str, 0, MAX_DIGITS_IN_INT);
	_ultoa_s((unsigned long)num, str, MAX_DIGITS_IN_INT, base);
	return (string(str));
}

string ToStr(unsigned long num, int base)
{
	char str[MAX_DIGITS_IN_INT];
	memset(str, 0, MAX_DIGITS_IN_INT);
	_ultoa_s(num, str, MAX_DIGITS_IN_INT, base);
	return (string(str));
}

string ToStr(float num)
{
	char str[64];  // I'm sure this is overkill
	memset(str, 0, 64);
	_sprintf_p(str, 64, "%f", num);
	return (string(str));
}

string ToStr(double num)
{
	char str[64];  // I'm sure this is overkill
	memset(str, 0, 64);
	_sprintf_p(str, 64, "%fL", num);
	return (string(str));
}

string ToStr(bool val)
{
	return (string((val == true ? "true" : "false")));
}

std::wstring s2ws(const std::string &s)
{
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}