/*
 * Modified 21 May 2013 for rpi-boot by github.org/JamesC1
 */

#include "ctype.h"

int isspace(int c)
/* dervied from rpi-boot strtol.c */
{
        if((c == ' ') || (c == '\f') || (c =='\n') || (c == '\r') ||
                        (c == '\t') || (c == '\v'))
                return 1;
        else
                return 0;
}

int isdigit(int c)
{
	if ((c < '0' || (c > '9')))
		return 0;
	else
		return 1;
}

int isalpha(int c)
{
	if ((c >= 'a') && (c <= 'z'))
		return 1;
	else if ((c >= 'A') && (c <= 'Z'))
		return 1;
	else
		return 0;
}

int isupper(int c)
{
	if ((c >= 'A') && (c <= 'Z'))
		return 1;
	else
		return 0;
}

