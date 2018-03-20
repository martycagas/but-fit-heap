#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

int isNumberOnly(char *s)
{
	while (*s != '\0')
	{
		if (isdigit(*s++) == 0)
		{
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[])
{
	struct tm timeStruct = { 0 };

	if (argc > 1)
	{
		printf("Program na zpracovani textu.\nTento program nacte text ze vstupu a analyzuje jej na slova, cisla a datum.\nU tech pak urci, zdali se jedna o palindrom, prvocislo a jaky je to den v tydnu.\n");
		return 1;
	}
	else
	{
		char str[101];
		int readChk;

		do
		{
			int palindromeChk = 1;
			int primeChk = 1;
			int dateChk = 0;

			readChk = scanf("%100s", str);

			int lenght = 0;
			while (str[lenght] != '\0')
			{
				lenght++;
			}

			if (isNumberOnly(str) == 1)
			{
				double m = atoi(str);
				int n = atoi(str);
				int a = 2;
				double squareRootN = 0;

				if (n > 2)
				{
					squareRootN = sqrt(m);

					do
					{
						if (n % a == 0)
						{
							primeChk = 0;
							break;
						}
						else
						{
							a++;
						}
					} while (primeChk == 1 && a < squareRootN);
				}

				if (primeChk == 0)
				{
					printf("number: %s\n", str);
				}
				else
				{
					printf("number: %s (prime)\n", str);
				}
			}

			if (lenght == 10)
			{
				dateChk = 1;

				for (int i = 0; i < 10; i++)
				{
					if (i == 4 || i == 7)
					{
						if (str[i] != '-')
							dateChk = 0;
					}
					else
					{
						if (isdigit(str[i]) == 0)
							dateChk = 0;
					}
				}

				if (dateChk == 1)
				{
					int year;
					int mon;
					int day;

					sscanf(str, "%d-%d-%d", &year, &mon, &day);

					timeStruct.tm_year = year - 1900;
					timeStruct.tm_mon = mon - 1;
					timeStruct.tm_mday = day;

					if (mktime(&timeStruct) != -1)
					{
						char buff[100];
						strftime(buff, 99, "%a %Y-%m-%d", &timeStruct);
						printf("date: %s\n", buff);
					}
				}
			}

			if (isNumberOnly(str) == 0 && dateChk == 0)
			{
				for (int i = 0; i < lenght; i++)
				{
					if (str[i] != str[lenght - i - 1])
					{
						palindromeChk = 0;
						break;
					}
				}

				if (palindromeChk == 0 || lenght < 2)
				{
					printf("word: %s\n", str);
				}
				else
				{
					printf("word: %s (palindrome)\n", str);
				}
			}
		} while (readChk != EOF);
	}
}
