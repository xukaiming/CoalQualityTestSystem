#pragma once
#ifndef _GYFXY_RDB_H_
#define _GYFXY_RDB_H_
#include "RDB.h"
class CGyfxyRDB  : public CRDB
{ 
public:
	CGyfxyRDB()
	{

	}
	~CGyfxyRDB()
	{

	}

	static long TemptoAD(int Temp); 
	static double VolToTemp(double voltage); 
	static double ADToVol(int ADValue);
private:
	static int		TempTable[ ]; 
	static INT64	VREF ;//= 5000000L; 
};

#endif