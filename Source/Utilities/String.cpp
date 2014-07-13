#include "String.h"

using std::string;

// WildcardMatch found on http://xoomer.virgilio.it/acantato/dev/wildcard/wildmatch.html, where it was attributed to 
// the C/C++ Users Journal, written by Mike Cornelison. Ugly, But fast as hell.

BOOL WildcardMatch(const char *pat, const char *str) {
	int i, star;

new_segment:

	star = 0;
	if (*pat == '*') {
		star = 1;
		do { pat++; } while (*pat == '*'); /* enddo */
	} /* endif */

test_match:

	for (i = 0; pat[i] && (pat[i] != '*'); i++) {
		if (str[i] != pat[i]) {
			if (!str[i]) return 0;
			if ((pat[i] == '?') && (str[i] != '.')) continue;
			if (!star) return 0;
			str++;
			goto test_match;
		}
	}
	if (pat[i] == '*') {
		str += i;
		pat += i;
		goto new_segment;
	}
	if (!str[i]) return 1;
	if (i && pat[i - 1] == '*') return 1;
	if (!star) return 0;
	str++;
	goto test_match;
}

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