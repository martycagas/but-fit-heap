#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double cfrac_func(double n, double z2, unsigned int lim);

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		if (strcmp(argv[1], "--log") == 0 && argc > 3)
		{
			double x = 1;
			unsigned int n = 1;

			if (sscanf(argv[2], "%lf", &x) != EOF && sscanf(argv[3], "%u", &n) != EOF)
			{
				printf("log(%g) = %.12g\n", x, log(x));
				printf("cf_log(%g) = %.12g\n", x, cfrac_log(x, n));
				printf("taylor_log(%g) = %.12g\n", x, taylor_log(x, n));

				return 0;
			}
		}
		else if (strcmp(argv[1], "--iter") == 0 && argc > 4)
		{
			double MIN = 1;
			double MAX = 1;
			double EPS = 1e-12;
			double log_lib_min;
			double log_lib_max;
			unsigned int counter1 = 1;
			unsigned int counter2 = 1;

			if (sscanf(argv[2], "%lf", &MIN) != EOF && sscanf(argv[3], "%lf", &MAX) != EOF && sscanf(argv[4], "%lf", &EPS) != EOF)
			{
				log_lib_min = log(MIN);
				log_lib_max = log(MAX);

				printf("log_min(%g) = %.12g\n", MIN, log_lib_min);
				printf("log_max(%g) = %.12g\n", MAX, log_lib_max);

				while (fabs(log_lib_min - cfrac_log(MIN, counter1)) > EPS)
				{
					counter1++;
				}
				while (fabs(log_lib_max - cfrac_log(MAX, counter2)) > EPS)
				{
					counter2++;
				}

				printf("continued fraction iterations: %u\n", (counter1 >= counter2) ? counter1 : counter2);
				printf("cf_log(%g) = %.12g\n", MIN, cfrac_log(MIN, counter1));
				printf("cf_log(%g) = %.12g\n", MAX, cfrac_log(MAX, counter2));

				counter1 = 1;
				counter2 = 1;
				
				while (fabs(log_lib_min - taylor_log(MIN, counter1)) > EPS)
				{
					counter1++;
				}
				while (fabs(log_lib_max - taylor_log(MAX, counter2)) > EPS)
				{
					counter2++;
				}

				printf("taylor polynomial iterations: %u\n", (counter1 >= counter2) ? counter1 : counter2);
				printf("taylor_log(%g) = %.12g\n", MIN, taylor_log(MIN, counter1));
				printf("taylor_log(%g) = %.12g\n", MAX, taylor_log(MAX, counter2));

				return 0;
			}
		}
		else
		{
			printf("Prosim zadejte argumenty --log s hodnotou a poctem iteraci, nebo --iter s intervalem a presnosti.\n");
			return 1;
		}
	}
	else
	{
		printf("Prosim zadejte argumenty --log s hodnotou a poctem iteraci, nebo --iter s intervalem a presnosti.\n");
		return 1;
	}
	return 1;
}

double taylor_log(double x, unsigned int n)
{
	unsigned int i = 1;
	double divider = 1;
	double log_x;

	if (x > 0 && x < 1)
	{
		x = 1 - x;
		double x_pow = x;
		log_x = -x;

		while (i < n)
		{
			x_pow = x_pow * x;
			divider += 1;
			log_x = log_x - (x_pow / divider);

			i++;
		}

		return log_x;
	}
	else if (x >= 1)
	{
		double y = (x - 1) / x;
		double y_pow = y;
		log_x = y;

		while (i < n)
		{
			y_pow = y_pow * y;
			divider += 1;
			log_x = log_x + y_pow / divider;

			i++;
		}

		return log_x;
	}
	else
	{
		printf("Prosim zadejte x >= 0.\n");
		return -1;
	}
}

double cfrac_log(double x, unsigned int n)
{
	if (x >= 0)
	{
		double z = (1 - x) / (1 + x);

		return -(2 * z / cfrac_func(1, z * z, n));
	}
	else
	{
		printf("Prosim zadejte x >= 0.\n");
		return -1;
	}
}

double cfrac_func(double i, double z2, unsigned int lim)
{
	if (i <= lim)
	{
		return (2 * i - 1) - (i * i * z2 / cfrac_func(i + 1, z2, lim));
	}
	else
	{
		return 2 * i - 1;
	}
}