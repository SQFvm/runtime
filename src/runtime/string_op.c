#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "string_op.h"


unsigned int str_sw(const char* lString, const char* rString)
{
	unsigned int i;
	char lc, rc;
	for (i = 0; lString[i] != '\0'; i++)
	{
		lc = lString[i];
		rc = rString[i];
		if (rc == '\0')
			return 0;
		if (lc != rc)
			return i + 1;
	}
	return i;
}
unsigned int str_swi(const char* lString, const char* rString)
{
	unsigned int i;
	char lc, rc;
	for (i = 0; lString[i] != '\0'; i++)
	{
		lc = tolower(lString[i]);
		rc = tolower(rString[i]);
		if (rc == '\0')
			return 0;
		if (lc != rc)
			return i + 1;
	}
	return i;
}

unsigned int str_ew(const char* lString, const char* rString)
{
	unsigned int i, j = 0;
	unsigned int rlen = (unsigned int)strlen(rString);
	char lc, rc;
	for (i = 0; lString[i] != '\0'; i++);
	if (i < rlen)
		return 0;
	i -= rlen;
	for (; lString[i] != '\0'; i++)
	{
		lc = lString[i];
		rc = rString[j++];
		if (rc == '\0')
			return 1;
		if (lc != rc)
			return 0;
	}
	return 1;
}
unsigned int str_ewi(const char* lString, const char* rString)
{
	unsigned int i, j = 0;
	unsigned int rlen = (unsigned int)strlen(rString);
	char lc, rc;
	for (i = 0; lString[i] != '\0'; i++);
	if (i < rlen)
		return 0;
	i -= rlen;
	for (; lString[i] != '\0'; i++)
	{
		lc = tolower(lString[i]);
		rc = tolower(rString[j++]);
		if (rc == '\0')
			return 1;
		if (lc != rc)
			return 0;
	}
	return 1;
}

const char* str_strwrd(const char* lString, const char* rString, const char* letters)
{
	int i, j;
	char lc, rc;
	bool flag = false;
	bool isSeparated = true;

	if (letters == NULL)
		letters = " ,-_\t.?!+:;<>#";
	for (i = 0, j = 0;; i++, j++)
	{
		lc = lString[i];
		rc = rString[j];
		if (flag)
		{
			if (rc == '\0')
			{
				if (chr_is(lc, letters) || lc == '\0')
					return lString + i - j;
				flag = false;
				isSeparated = false;
			}
			if (lc != rc)
			{
				flag = false;
				isSeparated = chr_is(lc, letters);
			}
		}
		else if (!isSeparated)
		{
			if (chr_is(lc, letters))
				isSeparated = true;
			j = -1;
		}
		else if (lc == rc)
		{
			flag = true;
		}
		else
		{
			isSeparated = chr_is(lc, letters);
			j = -1;
		}
		if (lc == '\0')
			return NULL;
	}
}

const char* str_strwrdi(const char* lString, const char* rString, const char* letters)
{
	int i, j;
	char lc, rc;
	bool flag = false;
	bool isSeparated = true;

	if (letters == NULL)
		letters = " ,-_\t";
	for (i = 0, j = 0;; i++, j++)
	{
		lc = tolower(lString[i]);
		rc = tolower(rString[j]);
		if (flag)
		{
			if (rc == '\0')
			{
				if (chr_is(lc, letters) || lc == '\0')
					return lString + i - j;
				flag = false;
				isSeparated = false;
			}
			if (lc != rc)
			{
				flag = false;
				isSeparated = chr_is(lc, letters);
			}
		}
		else if (!isSeparated)
		{
			if (chr_is(lc, letters))
				isSeparated = true;
			j = -1;
		}
		else if (lc == rc)
		{
			flag = true;
		}
		else
		{
			isSeparated = chr_is(lc, letters);
			j = -1;
		}
		if (lc == '\0')
			return NULL;
	}
}

int chr_is(const char c, const char* isArr)
{
	unsigned int i;
	for (i = 0; isArr[i] != '\0'; i++)
	{
		if (isArr[i] == c)
			return 1;
	}
	return 0;
}


unsigned int str_repchr(char* str, char toFind, char toReplace, int length)
{
	unsigned int i, j = 0;
	if (length == -1)
	{
		length = (int)strlen(str);
	}
	for (i = 0; i < (unsigned int)length; i++)
	{
		if (str[i] == toFind)
		{
			str[i] = toReplace;
			j++;
		}
	}
	return j;
}


unsigned int str_cmp(const char* lString, int lLen, const char* rString, int rLen)
{
	int i;
	if (lLen != -1 && rLen != -1 && lLen != rLen)
		return -1;
	i = 0;
	while (lLen != -1 ? i < lLen : lString[i] != '\0' || rString[i] != '\0')
	{
		if (lString[i] != rString[i])
		{
			return -1;
		}
		i++;
	}

	return 0;
}
unsigned int str_cmpi(const char* lString, int lLen, const char* rString, int rLen)
{
	int i;
	if (lLen != -1 && rLen != -1 && lLen != rLen)
		return -1;
	i = 0;
	while (lLen != -1 ? i < lLen : lString[i] != '\0' || rString[i] != '\0')
	{
		if (tolower(lString[i]) != tolower(rString[i]))
		{
			return -1;
		}
		i++;
	}

	return 0;
}
