// ClyRDB.cpp: implementation of the CClyRDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lryctrl.h"
#include "ClyRDB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//FOUT =VIN/((10V)*R*CT) 
//VIN = FOUT *(10V*R*C) = 100000*10*1*(1E-6)*2*(1E+3)
//=1000000*2*(1E-3)
//=2000
//=400*5
#define MAX_FREQ	100000			//100KHz
#define MAX_CURRENT 400				//400mA	
double CClyRDB::TempTable[]=
{
		0.000,0.005,0.011,0.016,0.022,0.027,0.033,0.038,0.044,0.050,
		0.055,0.061,0.067,0.072,0.078,0.084,0.090,0.095,0.101,0.107,
		0.113,0.119,0.125,0.131,0.137,0.143,0.149,0.155,0.161,0.167,
		0.173,0.179,0.185,0.191,0.197,0.204,0.210,0.216,0.222,0.229,
		0.235,0.241,0.248,0.254,0.260,0.267,0.273,0.280,0.286,0.292,
		0.299,0.305,0.312,0.319,0.325,0.332,0.338,0.345,0.352,0.358,
		0.365,0.372,0.378,0.385,0.392,0.399,0.405,0.412,0.419,0.426,
		0.433,0.440,0.446,0.453,0.460,0.467,0.474,0.481,0.488,0.495,
		0.502,0.509,0.516,0.523,0.530,0.538,0.545,0.552,0.559,0.566,
		0.573,0.580,0.588,0.595,0.602,0.609,0.617,0.624,0.631,0.639,

		0.646,0.653,0.661,0.668,0.68,0.683,0.690,0.698,0.705,0.713,
		0.720,0.727,0.735,0.743,0.750,0.758,0.765,0.773,0.780,0.788,
		0.795,0.803,0.811,0.818,0.830,0.834,0.841,0.849,0.857,0.865,
		0.872,0.880,0.888,0.896,0.903,0.911,0.919,0.927,0.935,0.942,
		0.950,0.958,0.966,0.974,0.982,0.990,0.998,1.006,1.013,1.021,
		1.029,1.037,1.045,1.053,1.061,1.069,1.077,1.085,1.094,1.102,
		1.110,1.118,1.126,1.134,1.142,1.150,1.158,1.167,1.175,1.183,
		1.191,1.199,1.207,1.216,1.224,1.232,1.240,1.249,1.257,1.265,
		1.273,1.282,1.290,1.298,1.307,1.315,1.323,1.332,1.340,1.348,
		1.357,1.365,1.373,1.382,1.390,1.399,1.407,1.415,1.424,1.432,

		1.441,1.449,1.458,1.466,1.475,1.483,1.492,1.500,1.509,1.517,
		1.526,1.534,1.543,1.551,1.560,1.569,1.577,1.586,1.594,1.603,
		1.612,1.620,1.629,1.638,1.646,1.655,1.663,1.672,1.681,1.690,
		1.698,1.707,1.716,1.724,1.733,1.742,1.751,1.759,1.768,1.777,
		1.786,1.794,1.803,1.812,1.821,1.829,1.838,1.847,1.856,1.865,
		1.874,1.882,1.891,1.900,1.909,1.918,1.927,1.936,1.944,1.953,
		1.962,1.971,1.980,1.989,1.998,2.007,2.016,2.025,2.034,2.043,
		2.052,2.061,2.070,2.078,2.087,2.096,2.105,2.114,2.123,2.132,
		2.141,2.151,2.160,2.169,2.178,2.187,2.196,2.205,2.214,2.223,
		2.232,2.241,2.250,2.259,2.268,2.277,2.287,2.296,2.305,2.314,

		2.323,2.332,2.341,2.350,2.360,2.369,2.378,2.387,2.396,2.405,
		2.415,2.424,2.433,2.442,2.451,2.461,2.470,2.479,2.488,2.497,
		2.507,2.516,2.525,2.534,2.544,2.553,2.562,2.571,2.581,2.590,
		2.599,2.609,2.618,2.627,2.636,2.646,2.655,2.664,2.674,2.683,
		2.692,2.702,2.711,2.720,2.730,2.739,2.748,2.758,2.767,2.776,
		2.786,2.795,2.805,2.814,2.823,2.833,2.842,2.851,2.861,2.870,
		2.880,2.889,2.899,2.908,2.917,2.927,2.936,2.946,2.955,2.965,
		2.974,2.983,2.993,3.002,3.012,3.021,3.031,3.040,3.050,3.059,
		3.069,3.078,3.088,3.097,3.107,3.116,3.126,3.135,3.145,3.154,
		3.164,3.173,3.183,3.192,3.202,3.212,3.221,3.231,3.240,3.250,
		3.259,3.269,3.279,3.288,3.298,3.307,3.317,3.326,3.336,3.346,

		3.355,3.365,3.374,3.384,3.394,3.403,3.413,3.423,3.432,3.442,
		3.451,3.461,3.471,3.480,3.490,3.500,3.509,3.519,3.529,3.538,
		3.548,3.558,3.567,3.577,3.587,3.596,3.606,3.616,3.626,3.635,
		3.645,3.655,3.664,3.674,3.684,3.694,3.703,3.713,3.723,3.732,
		3.742,3.752,3.762,3.771,3.781,3.791,3.801,3.810,3.820,3.830,
		3.840,3.850,3.859,3.869,3.879,3.889,3.898,3.908,3.918,3.928,
		3.938,3.947,3.957,3.967,3.977,3.987,3.997,4.006,4.016,4.026,
		4.036,4.046,4.056,4.065,4.075,4.085,4.095,4.105,4.115,4.125,
		4.134,4.144,4.154,4.164,4.174,4.184,4.194,4.204,4.213,4.223,
		4.233,4.243,4.253,4.263,4.273,4.283,4.293,4.303,4.313,4.323,

		4.332,4.342,4.352,4.362,4.372,4.382,4.392,4.402,4.412,4.422,
		4.432,4.442,4.452,4.462,4.472,4.482,4.492,4.502,4.512,4.522,
		4.532,4.542,4.552,4.562,4.572,4.582,4.592,4.602,4.612,4.622,
		4.632,4.642,4.652,4.662,4.672,4.682,4.692,4.702,4.712,4.722,
		4.732,4.742,4.752,4.762,4.772,4.782,4.793,4.803,4.813,4.823,
		4.833,4.843,4.853,4.863,4.873,4.883,4.893,4.904,4.914,4.924,
		4.934,4.944,4.954,4.964,4.974,4.984,4.995,5.005,5.015,5.025,		
		5.035,5.045,5.055,5.066,5.076,5.086,5.096,5.106,5.116,5.127,
		5.137,5.147,5.157,5.167,5.178,5.188,5.198,5.208,5.218,5.228,
		5.239,5.249,5.259,5.269,5.280,5.290,5.300,5.310,5.320,5.331,

		5.341,5.351,5.361,5.372,5.382,5.392,5.402,5.413,5.423,5.433,
		5.443,5.454,5.464,5.474,5.485,5.495,5.505,5.515,5.526,5.536,
		5.546,5.557,5.567,5.577,5.588,5.598,5.608,5.618,5.629,5.639,
		5.649,5.660,5.670,5.680,5.691,5.701,5.712,5.722,5.732,5.743,
		5.753,5.763,5.774,5.784,5.794,5.805,5.815,5.826,5.836,5.846,
		5.857,5.867,5.878,5.888,5.898,5.909,5.919,5.930,5.940,5.950,
		5.961,5.971,5.982,5.992,6.003,6.013,6.024,6.034,6.044,6.055,
		6.065,6.076,6.086,6.097,6.107,6.118,6.128,6.139,6.149,6.160,
		6.170,6.181,6.191,6.202,6.212,6.223,6.233,6.244,6.254,6.265,
		6.275,6.286,6.296,6.307,6.317,6.328,6.338,6.349,6.360,6.370,

		6.381,6.391,6.402,6.412,6.423,6.434,6.444,6.455,6.465,6.476,
		6.486,6.497,6.508,6.518,6.529,6.539,6.550,6.561,6.571,6.582,
		6.593,6.603,6.614,6.624,6.635,6.646,6.656,6.667,6.678,6.688,
		6.699,6.710,6.720,6.731,6.742,6.752,6.763,6.774,6.784,6.795,
		6.806,6.817,6.827,6.838,6.849,6.859,6.870,6.881,6.892,6.902,
		6.913,6.924,6.934,6.945,6.956,6.967,6.977,6.988,6.999,7.010,
		7.020,7.031,7.042,7.053,7.064,7.074,7.085,7.096,7.107,7.117,
		7.128,7.139,7.150,7.161,7.172,7.182,7.193,7.204,7.215,7.226,
		7.236,7.247,7.258,7.269,7.280,7.291,7.302,7.312,7.323,7.334,
		7.345,7.356,7.367,7.378,7.388,7.399,7.410,7.421,7.432,7.443,

		7.454,7.465,7.476,7.487,7.497,7.508,7.519,7.530,7.541,7.552,
		7.563,7.574,7.585,7.596,7.607,7.618,7.629,7.640,7.651,7.662,
		7.673,7.684,7.695,7.706,7.717,7.728,7.739,7.750,7.761,7.772,
		7.783,7.794,7.805,7.816,7.827,7.838,7.849,7.860,7.871,7.882,
		7.893,7.904,7.915,7.926,7.937,7.948,7.959,7.970,7.981,7.992,
		8.003,8.014,8.026,8.037,8.048,8.059,8.070,8.081,8.092,8.103,
		8.114,8.125,8.137,8.148,8.159,8.170,8.181,8.192,8.203,8.214,
		8.226,8.237,8.248,8.259,8.270,8.281,8.293,8.304,8.315,8.326,
		8.337,8.348,8.360,8.371,8.382,8.393,8.404,8.416,8.427,8.438,
		8.449,8.460,8.472,8.483,8.494,8.505,8.517,8.528,8.539,8.550,

		8.562,8.573,8.584,8.595,8.607,8.618,8.629,8.640,8.652,8.663,
		8.674,8.685,8.697,8.708,8.719,8.731,8.742,8.753,8.765,8.776,
		8.787,8.798,8.810,8.821,8.832,8.844,8.855,8.866,8.878,8.889,
		8.900,8.912,8.923,8.935,8.946,8.957,8.969,8.980,8.991,9.003,
		9.014,9.025,9.037,9.048,9.060,9.071,9.082,9.094,9.105,9.117,
		9.128,9.139,9.151,9.162,9.174,9.185,9.197,9.208,9.219,9.231,
		9.242,9.254,9.265,9.277,9.288,9.300,9.311,9.323,9.334,9.345,
		9.357,9.368,9.380,9.391,9.403,9.414,9.426,9.437,9.449,9.460,
		9.472,9.483,9.495,9.506,9.518,9.529,9.541,9.552,9.564,9.576,
		9.587,9.599,9.610,9.622,9.633,9.645,9.656,9.668,9.680,9.691,

		9.703,9.714,9.726,9.737,9.749,9.761,9.772,9.784,9.795,9.807,
		9.819,9.830,9.842,9.853,9.865,9.877,9.888,9.900,9.911,9.923,
		9.935,9.946,9.958,9.970,9.981,9.993,10.005,10.016,10.028,10.040,
		10.051,10.063,10.075,10.086,10.098,10.110,10.121,10.133,10.145,10.156,
		10.168,10.180,10.191,10.203,10.215,10.227,10.238,10.250,10.262,10.273,
		10.285,10.297,10.309,10.320,10.332,10.344,10.356,10.367,10.379,10.391,
		10.403,10.414,10.426,10.438,10.450,10.461,10.473,10.485,10.497,10.509,
		10.520,10.532,10.544,10.556,10.567,10.579,10.591,10.603,10.615,10.626,
		10.638,10.650,10.662,10.674,10.686,10.697,10.709,10.721,10.733,10.745,
		10.757,10.768,10.780,10.792,10.804,10.816,10.828,10.839,10.851,10.863,

		10.875,10.887,10.899,10.911,10.922,10.934,10.946,10.958,10.970,10.982,
		10.994,11.006,11.017,11.029,11.041,11.053,11.065,11.077,11.089,11.101,
		11.113,11.125,11.136,11.148,11.160,11.172,11.184,11.196,11.208,11.220,
		11.232,11.244,11.256,11.268,11.280,11.291,11.303,11.315,11.327,11.339,
		11.351,11.363,11.375,11.387,11.399,11.411,11.423,11.435,11.447,11.459,
		11.471,11.483,11.495,11.507,11.519,11.531,11.542,11.554,11.566,11.578,
		11.590,11.602,11.614,11.626,11.638,11.650,11.662,11.674,11.686,11.698,
		11.710,11.722,11.734,11.746,11.758,11.770,11.782,11.794,11.806,11.818,
		11.830,11.842,11.854,11.866,11.878,11.890,11.902,11.914,11.926,11.939,
		11.951,11.963,11.975,11.987,11.999,12.011,12.023,12.035,12.047,12.059,
		12.071,12.083,12.095,12.107,12.119,12.131,12.143,12.155,12.167,12.179,

		12.191,12.203,12.216,12.228,12.240,12.252,12.264,12.276,12.288,12.300,
		12.312,12.324,12.336,12.348,12.360,12.372,12.384,12.397,12.409,12.421,
		12.433,12.445,12.457,12.469,12.481,12.493,12.505,12.517,12.529,12.542,
		12.554,12.566,12.578,12.590,12.602,12.614,12.626,12.638,12.650,12.662,
		12.675,12.687,12.699,12.711,12.723,12.735,12.747,12.759,12.771,12.783,
		12.796,12.808,12.820,12.832,12.844,12.856,12.868,12.880,12.892,12.905,
		12.917,12.929,12.941,12.953,12.965,12.977,12.989,13.001,13.014,13.026,
		13.038,13.050,13.062,13.074,13.086,13.098,13.111,13.123,13.135,13.147,
		13.159,13.171,13.183,13.195,13.208,13.220,13.232,13.244,13.256,13.268,
		13.280,13.292,13.305,13.317,13.329,13.341,13.353,13.365,13.377,13.390,
		13.402,13.414,13.426,13.438,13.450,13.462,13.474,13.487,13.499,13.511,

		13.523,13.535,13.547,13.559,13.572,13.584,13.596,13.608,13.620,13.632,
		13.644,13.657,13.669,13.681,13.693,13.705,13.717,13.729,13.742,13.754,
		13.766,13.778,13.790,13.802,13.814,13.826,13.839,13.851,13.863,13.875,
		13.887,13.899,13.911,13.924,13.936,13.948,13.960,13.972,13.984,13.996,
		14.009,14.021,14.033,14.045,14.057,14.069,14.081,14.094,14.106,14.118,
		14.130,14.142,14.154,14.166,14.178,14.191,14.203,14.215,14.227,14.239,
		14.251,14.263,14.276,14.288,14.300,14.312,14.324,14.336,14.348,14.360,
		14.373,14.385,14.397,14.409,14.421,14.433,14.445,14.457,14.470,14.482,
		14.494,14.506,14.518,14.530,14.542,14.554,14.567,14.579,14.591,14.603,
		14.615,14.627,14.639,14.651,14.664,14.676,14.688,14.700,14.712,14.724,

		14.736,14.748,14.760,14.773,14.785,14.797,14.809,14.821,14.833,14.845,
		14.857,14.869,14.881,14.894,14.906,14.918,14.930,14.942,14.954,14.966,
		14.978,14.990,15.002,15.015,15.027,15.039,15.051,15.063,15.075,15.087,
		15.099,15.111,15.123,15.135,15.148,15.160,15.172,15.184,15.196,15.208,
		15.220,15.232,15.244,15.256,15.268,15.280,15.292,15.304,15.317,15.329,
		15.341,15.353,15.365,15.377,15.389,15.401,15.413,15.425,15.437,15.449,
		15.461,15.473,15.485,15.497,15.509,15.521,15.534,15.546,15.558,15.570,
		15.582,15.594,15.606,15.618,15.630,15.642,15.654,15.666,15.678,15.690,
		15.702,15.714,15.726,15.738,15.750,15.762,15.774,15.786,15.798,15.810,
		15.822,15.834,15.846,15.858,15.870,15.882,15.894,15.906,15.918,15.930,
		15.942,15.954,15.966,15.978,15.990,16.002,16.014,16.026,16.038,16.050,

		16.062,16.074,16.086,16.098,16.110,16.122,16.134,16.146,16.158,16.170,
		16.182,16.194,16.205,16.217,16.229,16.241,16.253,16.265,16.277,16.289,
		16.301,16.313,16.325,16.337,16.349,16.361,16.373,16.385,16.396,16.408,
		16.420,16.432,16.444,16.456,16.468,16.480,16.492,16.504,16.516,16.527,
		16.539,16.551,16.563,16.575,16.587,16.599,16.611,16.623,16.634,16.646,
		16.658,16.670,16.682,16.694,16.706,16.718,16.729,16.741,16.753,16.765,
		16.777,16.789,16.801,16.812,16.824,16.836,16.848,16.860,16.872,16.883,
		16.895,16.907,16.919,16.931,16.943,16.954,16.966,16.978,16.990,17.002,
		17.013,17.025,17.037,17.049,17.061,17.072,17.084,17.096,17.108,17.120,
		17.131,17.143,17.155,17.167,17.178,17.190,17.202,17.214,17.225,17.237,
		17.249,17.261,17.272,17.284,17.296,17.308,17.319,17.331,17.343,17.355,

		17.366,17.378,17.390,17.401,17.413,17.425,17.437,17.448,17.460,17.472,
		17.483,17.495,17.507,17.518,17.530,17.542,17.553,17.565,17.577,17.588,
		17.600,17.612,17.623,17.635,17.647,17.658,17.670,17.682,17.693,17.705,
		17.717,17.728,17.740,17.751,17.763,17.775,17.786,17.798,17.809,17.821,
		17.832,17.844,17.855,17.867,17.878,17.890,17.901,17.913,17.924,17.936,
		17.947,17.959,17.970,17.982,17.993,18.004,18.016,18.027,18.039,18.050,
		18.061,18.073,18.084,18.095,18.107,18.118,18.129,18.140,18.152,18.163,
		18.174,18.185,18.196,18.208,18.219,18.230,18.241,18.252,18.263,18.274,		
		18.285,18.297,18.308,18.319,18.330,18.341,18.352,18.362,18.373,18.384,
		18.395,18.406,18.417,18.428,18.439,18.449,18.460,18.471,18.482,18.493,

		18.503,18.514,18.525,18.535,18.546,18.557,18.567,18.578,18.588,18.599,
		18.609,18.620,18.630,18.641,18.651,18.661,18.672,18.682,18.693
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CClyRDB, CRDB)

CClyRDB::CClyRDB()
{
	status.TempAdValue		= 0;		//炉子温度AD值
	status.TempVoltage		= 0;		//炉子温度电压			
	status.Temperature		= 0;		//炉子温度
	//////////////////////////////////////
	status.SensorAdValue	= 0;	//小电极AD
	status.SensorVoltage	= 0;   //小电极电压			
	//////////////////////////////////////
	status.DJFrequency		= 0;		//电解电流频率
	status.DJCurrent		= 0	;		//电解电流	
	
	status.DJPoolAdValue	= 0;	//电解池电压	
	status.DJPoolVoltage	= 0;	//电解池电压	
	status.SQuality			= 0;	//硫含量
	////////////////////////////////////////			
	status.cCurrPos			= 0;	
	status.strCurrPos		= _T("未知");
	status.m_bStartTest		= FALSE;
	result.m_bReadResult	= FALSE;  
	status.m_bReseted		= FALSE;
}

CClyRDB::~CClyRDB()
{

}

void CClyRDB::Result::CalResult(CImageSlave *pClyImageSlave)
{ 
	CClyImageSlave *pImageSlave = (CClyImageSlave*)pClyImageSlave;
	//电流库仑

	#ifdef _USE_DAC8560
		m_fCoulomb_ByDA		= ((double)pImageSlave->Freq.lDJSum)/0xFFFFL*1.25f*DJ_PERIOD/DJ_REF_RES*m_sDJCoff;
 
	#else
		m_fCoulomb_ByDA		= ((double)pImageSlave->Freq.lDJSum)/0xFFFL*2.5f*DJ_PERIOD/DJ_REF_RES*m_sDJCoff;
						//ADVALUE/0xFFF*电源电压/基准电阻 = 电流
						//电流XS = 库仑
	#endif
	m_fCoulomb_ByVF		= ((double)pImageSlave->Freq.cCurrFreq)*m_sDJCoff*MAX_CURRENT/MAX_FREQ;		//250

	
	CalResultS();
	int min,sec;
	min = pImageSlave->cTestTimer/60%60;
	sec = pImageSlave->cTestTimer%60;
	m_dtAnalysisTime.SetTime(0,min,sec);    //实验时间   
	m_lPingXingNO;			//平行样编号
	m_bReadResult = TRUE;

}

void CClyRDB::Result::CalResultS()
{
	m_dStad			= (m_fCoulomb_ByVF)/5.690f/10/m_dSampleWeight;  
	//m_dStad			= (m_fCoulomb_ByDA)/5.690f/10/m_dSampleWeight;   
	if(m_bFix)
	{
		//求修正
		m_dStad = m_slope*m_dStad+m_intercept;
		if(m_dStad<0) m_dStad = 0;			
	} 
	m_dStd			= m_dStad*100/(100-m_dWater);					//干基硫    
}

void CClyRDB::Result::DelResultFromDB(CString strAutoNO)
{
	HRESULT hr= S_OK;
	CString strSQL;
	_RecordsetPtr	pRsResult;

	try
	{ 
		pRsResult.CreateInstance("ADODB.Recordset");
		strSQL.Format(_T("delete from 测硫仪实验数据 where 试验编号='%s'"),strAutoNO);
		hr = pRsResult->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);	 
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	if(pRsResult->State==adStateOpen)
		pRsResult->Close();
}

void CClyRDB::Result::SaveResult2DB()
{
	HRESULT hr= S_OK;
	CString strSQL;
	

	try
	{ 
		if(pRsResult->State==adStateOpen)
			pRsResult->Close();
		strSQL.Format(_T("select * from 测硫仪实验数据 where 试验日期>NOW()"));
		hr = pRsResult->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic,
			-1);	 
		pRsResult->AddNew(); 
		UpdateResultToDB();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	if(pRsResult->State==adStateOpen)
		pRsResult->Close();
}

void CClyRDB::Result::UpdateResultToDB()
{
	try
	{ 
		pRsResult->PutCollect("样品编号",_variant_t(m_szSampleNO));		//样品编号	
		pRsResult->PutCollect("试验日期",_variant_t(m_PickSampleDate));		//取样日期
		pRsResult->PutCollect("全硫Stad",_variant_t(m_dStad));				//全硫Stad
		pRsResult->PutCollect("全硫Std",_variant_t(m_dStd));					//全硫Std
		pRsResult->PutCollect("取样日期",_variant_t(m_PickSampleDate));		//样品质量
		pRsResult->PutCollect("样品质量",_variant_t(m_dSampleWeight));		//样品质量
		pRsResult->PutCollect("水分",_variant_t(m_dWater));				//水分
		pRsResult->PutCollect("电量",_variant_t(m_fCoulomb_ByVF));				//电量
		pRsResult->PutCollect("电量DA",_variant_t(m_fCoulomb_ByDA));				//电量2
		pRsResult->PutCollect("分析时间",_variant_t(m_dtAnalysisTime));		//分析时间
		pRsResult->PutCollect("设备名称",_variant_t(m_szName));				//设备名称
		pRsResult->PutCollect("修正",_variant_t(m_bFix));					//修正
		pRsResult->PutCollect("试验日期",_variant_t(m_dtTestDate));			//试验日期
		pRsResult->PutCollect("试验编号",_variant_t(m_szTestAutoNO));			//试验编号
		pRsResult->PutCollect("平行样号",_variant_t( m_lPingXingNO));			//平行样号
		pRsResult->Update();
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
}

void CClyRDB::Result::LoadResultFromDB(CString strAutoNO)
{
	CString strSQL;
	VARIANT var;
 
	strSQL.Format(_T("select * from 测硫仪实验数据 where 试验编号='%s'"),strAutoNO);
	try
	{
		if(pRsResult->State==adStateOpen)
			pRsResult->Close();
		pRsResult->Open(strSQL.AllocSysString(),
			theApp.pConn.GetInterfacePtr(),
			adOpenStatic,
			adLockOptimistic, -1); 
		m_szSampleNO		= VariantToStr(pRsResult->Fields->GetItem("样品编号")->Value);			//样品编号	
		var					= pRsResult->Fields->GetItem("取样日期")->Value;
		m_PickSampleDate	= pRsResult->Fields->GetItem("取样日期")->Value.date;					//取样日期
		m_dStad				=  _tcstod(VariantToStr(pRsResult->Fields->GetItem("全硫Stad")->Value),NULL);	//分析基硫
		m_dStd				=  _tcstod(VariantToStr(pRsResult->Fields->GetItem("全硫Std")->Value),NULL);//干基硫 //何学化 
		m_dSampleWeight     =  _tcstod(VariantToStr(pRsResult->Fields->GetItem("样品质量")->Value),NULL);;		//样品重量
		m_dWater			=  _tcstod(VariantToStr(pRsResult->Fields->GetItem("水分")->Value),NULL);;				//分析水
		m_fCoulomb_ByVF		=  _tcstod(VariantToStr(pRsResult->Fields->GetItem("电量")->Value),NULL);;				//电量
		m_fCoulomb_ByDA		=  _tcstod(VariantToStr(pRsResult->Fields->GetItem("电量DA")->Value),NULL);;				//电量
		m_dtAnalysisTime    =  pRsResult->Fields->GetItem("分析时间")->Value.date;		//实验时间
		m_szName			=  VariantToStr(pRsResult->Fields->GetItem("设备名称")->Value);				//设备名称
		m_bFix				= pRsResult->Fields->GetItem(_T("修正"))->Value;					//修正
		m_dtTestDate		= pRsResult->Fields->GetItem(_T("试验日期"))->Value.date;			//测试时刻
		m_szTestAutoNO		= VariantToStr(pRsResult->Fields->GetItem(_T("试验编号"))->Value) ;			//自动编号
		m_lPingXingNO		= _tcstol(VariantToStr(pRsResult->Fields->GetItem(_T("平行样号"))->Value),NULL,10) ;			//平行样编号
 
		m_sDJCoff	;		//=  _tcstol(VariantToStr(pRsResult->Fields->GetItem("电解系数")->Value),NULL,10);;	//电解系数	 //修正电容容量不对引起的频率漂移
		m_slope	;		//	=  _tcstol(VariantToStr(pRsResult->Fields->GetItem("斜率")->Value),NULL,10);;		//斜率
		m_intercept;		//	=  _tcstol(VariantToStr(pRsResult->Fields->GetItem("截距")->Value),NULL,10);;		//截距
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	} 
  
}

void CClyRDB::Result::BuildAutoNO()
{
	COleDateTime  dtEnd;									//查询结束日期
	COleDateTime  dtStart=COleDateTime::GetCurrentTime( );  //得到今天的日期
	CString strStartDate,strEndDate,AutoNo;   //开始日期,结束日期,自动编号
	CString sName = m_szName.Left(1);
	sName+=_T("S");
	//
	int RecordCount = 0;
	CString strSQL;
	_RecordsetPtr pRsTestNO;
	pRsTestNO.CreateInstance("ADODB.Recordset");	
	
	//dtStart.SetDateTime(2002, 3, 15, 1, 0, 0);  
	dtEnd   = MakeDays(1)+dtStart;
	strStartDate = dtStart.Format(_T("%Y%m%d"));
	{
		//1AE2002031504
		//查询已有 记录 		
		strSQL.Format(
			_T("select 试验编号 from 测硫仪实验数据 where 试验编号\
			like '%%%s%%' and 试验日期  between  #%s# and #%s# and \
			设备名称='%s' order by 试验编号"), 
			strStartDate,
			dtStart.Format(_T("%Y-%m-%d")),
			dtEnd.Format(_T("%Y-%m-%d")),
			m_szName
			);	 
		try
		{
			int length;
			pRsTestNO->Open(strSQL.AllocSysString(),
				theApp.pConn.GetInterfacePtr(),
				adOpenStatic,
				adLockOptimistic,
				-1);	
			if(!pRsTestNO->adoEOF)
				pRsTestNO->MoveLast();		 
			if((RecordCount = pRsTestNO->GetRecordCount())==0)
			{
				AutoNo = sName+strStartDate+(_T("001"));				
			}
			else 
			{
				AutoNo = VariantToStr(pRsTestNO->Fields->GetItem("试验编号")->Value);
				length = AutoNo.GetLength();		
				RecordCount = _tcstol(AutoNo.Right(3).GetBuffer(3),NULL,10);
				AutoNo = AutoNo.Left(length-3);;
				strStartDate= AutoNo+(_T("%03d"));
				AutoNo.Format(strStartDate,RecordCount+1);
			}
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}
	}
	if(pRsTestNO->State == adStateOpen)
		pRsTestNO->Close();
	m_szTestAutoNO = AutoNo;
}

void CClyRDB::RefreshRDBTab(CClyImageSlave *pImageSlave)	
{
	status.TempAdValue		= pImageSlave->AdValue.lTemp;									//炉子温度AD值	
	status.TempVoltage		= TempADtoVol(pImageSlave->AdValue.lTemp);							//炉子温度电压
	status.Temperature		= GetTemperature(status.TempVoltage)*attrib.m_dCoff;			//炉子温度
	//////////////////////////////////////
	status.SensorAdValue 	= pImageSlave->AdValue.lChlPol;;								//小电极电压
	status.SensorVoltage	= PolADToVol(status.SensorAdValue);								//小电极AD
	//////////////////////////////////////
	status.DJFrequency		= pImageSlave->Freq.cCurrFreq-pImageSlave->Freq.cPreFreq;		//电解电流频率
	status.DJCurrent		= (((double)status.DJFrequency)*attrib.m_sDJCoff*MAX_CURRENT)/MAX_FREQ;			//电解电流

	status.DJCoulomb		= pImageSlave->Freq.cCurrFreq*attrib.m_sDJCoff*MAX_CURRENT/MAX_FREQ;				//电流库仑
	status.SQuality			= ((double)status.DJCoulomb)/5.690f/1000;                       //S  mg
	//(((double)status.DJFrequency)*MAX_CURRENT)/MAX_FREQ;		
	//status.SPercent;		//硫含量99

	status.DJPoolAdValue	= pImageSlave->AdValue.lChlPol;		//电解池电压AD
	status.DJPoolVoltage	= DJADToVol(pImageSlave->AdValue.lChlPol);		//电解池电压
	status.DJSwitch			= pImageSlave->Freq.DJSwitch;
	status.m_bReseted		= pImageSlave->cResetState>0;
	////////////////////////////////////////
	switch(pImageSlave->MotorCtrl.cPos)
	{
	case POS_UNKNOWN :
		status.strCurrPos		= _T("未知");     //样杆当前位置
		break;
	case POS_PUT_SAMPLE:
		status.strCurrPos		= _T("放样位");   
		break;
	case POS_MIDDLE_1:
		status.strCurrPos		= _T("预分解1");   
		break;
	case POS_MIDDLE_2:
		status.strCurrPos		= _T("预分解2");   
		break;
	case POS_HIGH_TEMP:
		status.strCurrPos		= _T("高温位");   
		break; 
	} 
	status.cTestState		= pImageSlave->cTestState;
	status.cCurrPos			= pImageSlave->MotorCtrl.cPos;	
	status.bMoving			= pImageSlave->MotorCtrl.cSampleMoving;	 
	//SaveCurve();
	//int     cSampleDiskCurrPos;	
	//BOOL	bSampleDiskMoving;
	//CString strSampleDiskCurrPos;    //样盘当前位置
	status.cSampleDiskCurrPos	= pImageSlave->PosCtrl.cSamplePos;
	status.bSampleDiskMoving	= pImageSlave->PosCtrl.cSamplePosMoving;
 	status.strSampleDiskCurrPos.Format(_T("%d"),status.cSampleDiskCurrPos);
	////////////////////////////////////////////////////////////////////
}

void CClyRDB::SaveCurve()
{
	//CFile *TmpTab  = new CFile(_T("SCurve.txt"),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite );
	char strTemp[200] ; 
	CFile *fileCurve;
	try
	{
		if(status.cTestState>0)
		{
			TCHAR   cPath[MAX_PATH]; 
			GetModuleFileName(AfxGetApp()-> m_hInstance,cPath,MAX_PATH); 
			LPTSTR   p=_tcsrchr(cPath, _T('\\'));p++; 
			p[0]=0; 
			_tcscat(cPath,_T("CurveSave\\\0"));
			fileCurve = new CFile(CString(cPath)+_T("SCurve.CSV"), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite );
			
			if(fileCurve != NULL)
			{
				sprintf(strTemp,"%d,%f\n",status.SensorAdValue,status.DJCurrent);
				fileCurve->SeekToEnd ( );
				fileCurve->Write(strTemp,strlen(strTemp));
			}
			fileCurve->Close();
			delete fileCurve;
		}
	}
	catch(CFileException* ex )
	{
		ex->ReportError();
		ex->Delete();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}  
}

void CClyRDB::InitRDB(CClyImageSlave *ImageSlave)
{
	ImageSlave->DownloadWorkParam.cRightToMiddle1 = attrib.m_sPreDecomposePos1;    //放样位到预分解1 0.01s检查一次
	ImageSlave->DownloadWorkParam.cRightToMiddle2 = attrib.m_sPreDecomposePos2;    //放样位到预分解2
	ImageSlave->DownloadWorkParam.cRightToLeft	  = attrib.m_sDecomposePos;        //放样位到高温位
       //样车运行时间
	ImageSlave->DownloadWorkParam.cTimeDecompose1 = attrib.m_sPreDecomposeTim1;    //分解时间1            
	ImageSlave->DownloadWorkParam.cTimeDecompose2 = attrib.m_sPreDecomposeTim2;    //分解时间2
	ImageSlave->DownloadWorkParam.cTimeDelay	  = attrib.m_sEndDecomposDelay;         //结束延时	
	
	ImageSlave->DownloadWorkParam.lDestTempADValue= TemptoAD(attrib.m_sDestTemp/attrib.m_dCoff);
	//FLOAT			m_fStartDJVol;			//开始电解电压
	//FLOAT			m_fEndDJVol;			//结束电解电压
	ImageSlave->DownloadWorkParam.lDStartDJADValue= PolVoltoAD(attrib.m_fStartDJVol+0.1);						;
	ImageSlave->DownloadWorkParam.lDEndDJADValue  = PolVoltoAD(attrib.m_fEndDJVol+0.1); 
	ImageSlave->bAutoCly							= attrib.m_bAutoCly;
	//int max  =	VoltoAD();
	//result.m_szName  = m_szName; 
	//result.m_sDJCoff = attrib.m_sDJCoff;

	//result.m_slope		= attrib.m_slope	;
	//result.m_intercept	= attrib.m_intercept;
	//int i	=  TemptoAD(1000); 
	//int j	=  TemptoAD(1200);
	//int KC	=  (j-i)/2000;
}

long CClyRDB::TempVoltoAD(double voltage)
{
	voltage+=0.005;
	return voltage*0xFFF*GAIN_TEMP/VREF;
}

long CClyRDB::PolVoltoAD(double voltage)
{
	return voltage*0xFFF*GAIN_POL/VREF;
}

double CClyRDB::PolADToVol(long volADValue)
{
	return ((double)volADValue)*VREF/GAIN_POL/0xFFF;
}

double CClyRDB::DJADToVol(long volADValue)
{
	return ((double)volADValue)*VREF/GAIN_DJ/0xFFF;
}

long CClyRDB::TemptoAD(double Temp)
{
	int i =Temp; 
	if(Temp>1200)
		Temp=1200;
	return TempVoltoAD(TempTable[i]);
}

double CClyRDB::TempADtoVol(long ADVAlue)
{
	return	 ((double) ADVAlue *VREF)/0xFFF/GAIN_TEMP;
}

 double CClyRDB::GetTemperature(double voltage)
 {
 	int i=0;      //1768
 	for(i=0;i<sizeof(TempTable)/sizeof(double);i++)
 	{
 		if(voltage-TempTable[i]<1E-6)
 			break;
 	}
 	return i;
 }

 double CClyRDB::GetVoltage(double Temperature)
 {
 	int i = Temperature;
 	return TempTable[i];
 }
 

 BOOL CClyRDB::UpdateParam2DB()
 {  
	 BOOL bResult = TRUE;
	 HRESULT hr= S_OK; 
	 //CClyRDB *pRdb = (CClyRDB*)Rdb;
	 CClyRDB::CAttrib *pAtt = &attrib;
	 pRsDev->PutCollect("名称",_variant_t(m_szName));
	 pRsDev->PutCollect("型号",_variant_t(m_szType));
	 pRsDev->PutCollect("标识",m_bID);
	 pRsDev->PutCollect("串行口号",m_sComPort);
	 pRsDev->PutCollect("扫描周期(mS)",m_lScanPriod_mS);
	 pRsDev->PutCollect("预分解1位置(S)",pAtt->m_sPreDecomposePos1);
	 pRsDev->PutCollect("预分解1时间(S)",pAtt->m_sPreDecomposeTim1);
	 pRsDev->PutCollect("预分解2位置(S)",pAtt->m_sPreDecomposePos2);
	 pRsDev->PutCollect("预分解2时间(S)",pAtt->m_sPreDecomposeTim2);
	 pRsDev->PutCollect("分解位置(S)",pAtt->m_sDecomposePos);
	 pRsDev->PutCollect("分解结束延时(S)",pAtt->m_sEndDecomposDelay);
	 pRsDev->PutCollect("分解温度(℃)",pAtt->m_sDestTemp);
	 pRsDev->PutCollect("温度系数",pAtt->m_dCoff);
	 pRsDev->PutCollect("试样类别",_variant_t(pAtt->m_szSampleType));
	 pRsDev->PutCollect("修正",_variant_t(pAtt->m_bFix?VARIANT_TRUE:VARIANT_FALSE));

	 ////////////////////////////////////////////////////////////////////////////
	 pRsDev->PutCollect("样位1超时时间",_variant_t(pAtt->m_sPos1TimeOutTime));
	 pRsDev->PutCollect("样位N超时时间",_variant_t(pAtt->m_sPosNTimeOutTime));
	 pRsDev->PutCollect("自动测硫仪",_variant_t(pAtt->m_bAutoCly?VARIANT_TRUE:VARIANT_FALSE));
	 pRsDev->PutCollect("硫起始值(mV)",_variant_t(pAtt->m_fStartDJVol));
	 pRsDev->PutCollect("硫终止值(mV)",_variant_t(pAtt->m_fEndDJVol));
	 pRsDev->PutCollect("电解系数(%)",_variant_t(pAtt->m_sDJCoff));   
	 pRsDev->PutCollect("蜂鸣时间(mS)",_variant_t(pAtt->m_sBeepTime)); 
	 /*  
	升温速率  
	硫起始值(mV)
	硫终止值(mV)
	电解系数(%)
	蜂鸣时间(mS) 
	自动打印
	 */
	 /*
 	LRYRDB *pLryRdb = &LryRdb ;
	//((CSpinButtonCtrl*)GetDlgItem(IDC_DEVICE_SPIN))->SetRange(1,pRsDev->RecordCount);
	pRsDev->PutCollect("自动打印",_variant_t(pLryRdb->m_bAutoPrint?VARIANT_TRUE:VARIANT_FALSE));
	pRsDev->PutCollect("修正",_variant_t(pLryRdb->m_bFix?VARIANT_TRUE:VARIANT_FALSE));	
 	*/
	 return bResult; 
 }

 

BOOL CClyRDB::UpdateParamFromDB()
{
	BOOL bResult  = TRUE;
	if(pRsDev->RecordCount>0)
	{  //COleVariant 
		m_szName			= VariantToStr(pRsDev->Fields->GetItem("名称")->Value);;				//设备名称
		m_szType 			= VariantToStr(pRsDev->Fields->GetItem("型号")->Value);	//设备类型
		m_bID				= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("标识"))->Value),NULL,10  );;		//ID号
		m_sComPort			= (short)_tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("串行口号"))->Value),NULL,10);;;				//串口号	
		m_lScanPriod_mS		= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("扫描周期(mS)"))->Value),NULL,10);     //扫描周期;		//扫描周期 
		attrib.m_sPreDecomposeTim1	= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("预分解1时间(S)"))->Value),NULL,10);	//m_s预分解1时间(S); 
		attrib.m_sPreDecomposePos1	= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("预分解1位置(S)"))->Value),NULL,10);	//预分解1位置(S); 
		attrib.m_sPreDecomposeTim2	= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("预分解2时间(S)"))->Value),NULL,10); 
		attrib.m_sPreDecomposePos2	= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("预分解2位置(S)"))->Value),NULL,10);
		attrib.m_sDecomposePos		= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("分解位置(S)"))->Value),NULL,10)	;//分解位置
		attrib.m_sEndDecomposDelay	= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("分解结束延时(S)"))->Value),NULL,10);	//分解延时
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		attrib.m_sPos1TimeOutTime   = _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("样位1超时时间"))->Value),NULL,10);	//样位1超时时间
		attrib.m_sPosNTimeOutTime   = _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("样位N超时时间"))->Value),NULL,10);	//样位1超时时间
		attrib.m_bAutoCly			= V_BOOL(&pRsDev->Fields->GetItem(_T("自动测硫仪"))->Value)?TRUE:FALSE;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		attrib.m_sWarmUpSpeed		= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("升温速率"))->Value),NULL,10)		;	//升温速度
		attrib.m_sDestTemp			= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("分解温度(℃)"))->Value),NULL,10);			//目标温度
		attrib.m_dCoff				= pRsDev->Fields->GetItem(_T("温度系数"))->Value.dblVal;				//温度系数
		attrib.m_szSampleType		= VariantToStr(pRsDev->Fields->GetItem(_T("试样类别"))->Value);		    //样品类型 油/煤
		attrib.m_fStartDJVol		= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("硫起始值(mV)"))->Value),NULL,10);			//开始电解电压
		attrib.m_fEndDJVol			= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("硫终止值(mV)"))->Value),NULL,10)		;			//结束电解电压
		attrib.m_sDJCoff			= pRsDev->Fields->GetItem(_T("电解系数(%)"))->Value.dblVal;				//电解系数	
		attrib.m_sBeepTime			= _tcstol(VariantToStr(pRsDev->Fields->GetItem(_T("蜂鸣时间(mS)"))->Value),NULL,10);			//蜂鸣时间
		attrib.m_bFix				= V_BOOL(&pRsDev->Fields->GetItem(_T("修正"))->Value)?TRUE:FALSE;					//修正
		attrib.m_bAutoPrint			= V_BOOL(&pRsDev->Fields->GetItem(_T("自动打印"))->Value)?TRUE:FALSE;			//自动打印
		attrib.m_slope				= pRsDev->Fields->GetItem(_T("斜率B"))->Value.dblVal;
		attrib.m_intercept			= pRsDev->Fields->GetItem(_T("截距A"))->Value.dblVal;
		
		result.m_szName  = m_szName; 
		result.m_sDJCoff = attrib.m_sDJCoff;
		
		result.m_bFix		= attrib.m_bFix;
		result.m_slope		= attrib.m_slope	;
		result.m_intercept	= attrib.m_intercept;
	}
	else
	{
		bResult = FALSE;
		::MessageBox(theApp.m_pMainWnd->m_hWnd,_T("无法读取设备属性，初始化失败！"),_T("警告"),MB_OK+MB_ICONERROR);
	}
	return TRUE;
}


  BOOL CClyRDB::CCoff::UpdateCoff2DB()
  {
	return TRUE;
  }

  BOOL CClyRDB::CCoff::LoadCoffFromDB()
  {
	return TRUE;
  }