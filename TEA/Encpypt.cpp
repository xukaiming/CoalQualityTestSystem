#include "stdafx.h"
#include "Encpypt.h" 
Encpypt::Encpypt(void) 
{
	memset(Key,0,sizeof(Key));
	unsigned char myKey [KEY_LEN] = "xukaiming2013-8"; //√‹¬Î
	memcpy(Key,myKey,sizeof(Key));

	pTea = new TEA(Key,KEY_LEN);;//(key, 16, false);
}
 

Encpypt::~Encpypt(void)
{
	delete pTea;
}
