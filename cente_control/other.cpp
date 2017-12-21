#include "other.h"


u_short charToShort(char *a)
{
	int i = 0;
	u_short port = 0;
	
	for(i = 0;i < (int)strlen(a);i++)
	{
		port += (a[i]-'0')*product(strlen(a)-i-1);
	}
	return port;
}

u_short product(int i)
{
	u_short pro = 1;
	for(;i != 0;i--)
	{
		pro *= 10;
	}
	return pro;
}