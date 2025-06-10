#include "chicken.h"

char *strdup_wrapper(char *(*f)(const char *), const char *s)
{
	char *result = f(s);
	if (!result)
		return NULL;
	return result;
}
