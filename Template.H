#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_
#include <algorithm>     //º¯ÊýÄ£°å
using namespace std;
 
template <typename COLLECTION_TYPE>
COLLECTION_TYPE Sum(COLLECTION_TYPE* Collection, int cnt)
{
	COLLECTION_TYPE sum = 0;
	for(int i=0;i<cnt;i++)
		sum += Collection[i];
	return sum;
}
#endif