#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const int datalen = 10240;
const int max_stages = 12;

/* interface array, put your signal data here and
	 pure signal is returned here after filtering*/
float dataio[datalen];
float* data[max_stages];// main data arrays
float* cost[max_stages];
char* sig[max_stages];
float* har;
float* gar;

// scaling filter coefficients
static const float db1[] = { 0.7071067f,0.7071067f };
static const float db2[] = { 0.4829629f,0.8365163f,0.2241439f,
	-0.1294095f };
static const float db3[] = { 0.3326706f,0.8068915f,0.4598775f,-0.1350110f,
	-0.0854413f,0.0352263f };
static const float db4[] = { 0.2303778f,0.7148466f,0.6308808f,-0.0279838f,
		-0.1870348f,0.0308414f,0.0328830f,-0.0105974f };
static const float db5[] = { 0.1601024f,0.6038293f,0.7243085f,
	0.1384281f,-0.2422949f,-0.0322449f,0.0775715f,-0.0062415f,
	-0.0125808f,0.0033357f };
static const float db6[] = { 0.1115407f,0.4946239f,0.7511339f,0.3152504f,
		-0.2262647f,-0.1297669f,0.0975016f,0.0275229f,
		-0.031582f,0.0005538f,0.0047773f,-0.0010773f };
static const float db7[] = { 0.0778521f,0.3965393f,0.7291321f,0.4697823f,
		-0.1439060f,-0.2240362f,0.0713092f,0.0806126f,-0.0380299f,
		-0.0165745f,0.012551f,0.0004296f,-0.0018016f,0.0003537f };
static const float db8[] = { 0.0544158f,0.3128716f,0.6756307f,0.5853549f,
		-0.0158291f,-0.2840155f,0.0004725f,0.1287474f,-0.0173693f,
		-0.0440883f,0.0139810f,0.0087461f,-0.0048704f,-0.0003917f,
		0.0006754f,-0.0001175f };
static const float db9[] = { 0.0380779f,0.2438347f,0.6048231f,0.6572881f,
		0.1331974f,-0.2932738f,-0.0968408f,0.1485407f,0.0307257f,
		-0.0676328f,0.0002509f,0.0223617f,-0.0047232f,-0.0042815f,
		0.0018476f,0.0002304f,-0.000252f,0.0000393f };
static const float db10[] = { 0.0266701f,0.1881768f,0.5272012f,0.6884590f,0.2811723f,
		-0.2498464f,-0.1959463f,0.1273693f,0.0930574f,-0.0713941f,
		-0.0294575f,0.0332127f,0.0036066f,-0.0107332f,0.0013954f,
		0.0019924f,-0.0006859f,-0.0001165f,0.0000936f,-0.0000133f };
static const float* pdb[10] = { db1,db2,db3,db4,db5,db6,db7,db8,db9,db10 };

int dwpt_filter(int dbN, int stages, int numf, float thrshold);
int hperiod(int dbN, int len);
int gperiod(int dbN, int len);
int decompose(int dbN, int stages);
int codeh(int nlevel, int dbN, int start, int len);
int	codeg(int nlevel, int dbN, int start, int len);
int calcost(int stages, int numf, float thrshold);
int select(int stages, int numf, float thrshold);
int detailoff(int nlevel, int nblock);
int filter(int nlevel, int nblock);
int recompose(int dbN, int stages);
int recodeh(int nlevel, int dbN, int start, int len);
int recodeg(int nlevel, int dbN, int start, int len);


////////////////////////////////////////////////////////////////////////////////////
/* This program uses wavelet packets to filter signals in order to de-noise.      *
*    Many other usages such as signal analysis, detecting, compressing            *
*  	 can also be done with only a little more functions added to this program     */
////////////////////////////////////////////////////////////////////////////////////
//int main()
//{
//	int dbN, stages, numf, j;
//	float thrshold = 0.5;
//	FILE* fp, * fp1;
//
//	do
//	{
//		printf("Please select a wavelet filter(dbN: 1 - 10)...\n");
//		scanf_s("%d", &dbN);
//		if (dbN > 10)
//			printf("\ndbN should be from 1 to 10, try it again!\n");
//	} while (dbN > 10);
//	do
//	{
//		printf("\nPlease input stages of decompose(note: stages <= max_stages)...\n");
//		scanf_s("%d", &stages);
//		if (stages > max_stages)
//			printf("\nstages should not be more than max_stages!\n");
//	} while (stages > max_stages);
//	printf("\nPlease select a cost function:\n");
//	printf("1 - Shannon entropy cost function\n");
//	printf("2 - Threshold cost function\n");
//	scanf_s("%d", &numf);
//	if (numf != 1)
//	{
//		printf("Please input the threshold(0 - infinity) ...\n");
//		scanf_s("%f", &thrshold);
//	}
//
//	// open the input file
//	if ((fp = fopen("input.txt", "r")) == NULL)
//	{
//		printf("cann't open input file\n");
//		exit(-3);
//	}
//	for (j = 0; j < datalen; j++)
//		fscanf_s(fp, "%f", &dataio[j]);
//
//	// wavelet transformation
//	dwpt_filter(dbN, stages, numf, thrshold);
//
//	// out put result
//	if ((fp1 = fopen("output.txt", "w+")) == NULL)
//	{
//		printf("cann't open output file\n");
//		exit(-4);
//	}
//	for (j = 0; j < datalen; j++)
//		fprintf(fp1, "%10.4f\n", dataio[j]);
//
//	fclose(fp);
//	fclose(fp1);
//	return 1;
//}


int dwpt_filter(int dbN, int stages, int numf, float thrshold)
{
	int i, j;

	// allocate memory for data[stages][datalen] array
	for (i = 0; i < stages; i++)
	{
		if ((data[i] = new float[datalen]) == NULL)
		{
			printf("Memory allocation for data[][] error\n");
			exit(0);
		}
	}
	for (i = 0; i < stages; i++)
	{
		if ((cost[i] = new float[1 << i]) == NULL)
		{
			printf("Memory allocation for cost[][] error\n");
			exit(0);
		}
	}
	for (i = 0; i < stages; i++)
	{
		if ((sig[i] = new char[1 << i]) == NULL)
		{
			printf("Memory allocation for sig[][] error\n");
			exit(0);
		}
	}
	for (j = 0; j < datalen; j++)
		data[0][j] = dataio[j];// initialize data[0][]

	decompose(dbN, stages);
	select(stages, numf, thrshold);
	filter(0, 0);// recurse to get rid of detail part of components
	recompose(dbN, stages);

	for (j = 0; j < datalen; j++)
		dataio[j] = data[0][j];// computation result returned in dataio[]

	// free memory of data[stages][datalen], cos[][] and sig[][] array
	for (i = 0; i < stages; i++)
	{
		delete data[i];
		delete cost[i];
		delete sig[i];
	}
	return 1;
}


int decompose(int dbN, int stages)
{
	int nlevel, len, nblock, start, j;

	// main loop for the nlevel-th decomosition
	for (nlevel = 1; nlevel < stages; nlevel++)
	{
		for (j = 0; j < datalen; j++)
			data[nlevel][j] = data[nlevel - 1][j]; //initialize the data of nlevel-th stage

		len = datalen >> (nlevel - 1);
		hperiod(dbN, len);
		gperiod(dbN, len);
		start = 0;
		for (nblock = 0; nblock < (1 << (nlevel - 1)); nblock++)
		{
			codeh(nlevel, dbN, start, len);
			codeg(nlevel, dbN, start, len);
			start += len;
		}
		delete har;
		delete gar;
	}
	return 1;
}


int hperiod(int dbN, int len)
{
	int sign, i;
	float temp;
	// allocate memory for har[]
	if ((har = new float[len]) == NULL)
	{
		printf("not enough memory");
		exit(-1);
	}

	for (sign = 0; sign < len; sign++)
	{
		temp = 0.0;
		for (i = 0; i < (2 * dbN); i++)
		{
			if (((sign - i + dbN - 1) % len) == 0)
				temp += pdb[dbN - 1][i];
		}
		har[sign] = temp;
	}
	return 1;
}


int gperiod(int dbN, int len)
{
	int sign, i, flag;
	float temp;
	// allocate memory for gar[]
	if ((gar = new float[len]) == NULL)
	{
		printf("not enough memory");
		exit(-2);
	}

	for (sign = 0; sign < len; sign++)
	{
		temp = 0.0;
		for (i = 0; i < (2 * dbN); i++)
		{
			if (((sign + i - dbN - 2) % len) == 0)
			{
				if ((dbN - i + 1) % 2)
					flag = -1;
				else
					flag = 1;
				temp += pdb[dbN - 1][i] * flag;
			}
		}
		gar[sign] = temp;
	}
	return 1;
}


int codeh(int nlevel, int dbN, int start, int len)
{
	int i, i2, k, len1;
	float temp;
	len1 = len >> 1;
	for (i = 0; i < len1; i++)
	{
		i2 = 2 * i;
		data[nlevel][i + start] = 0.0;
		if (len > (2 * dbN))
		{
			for (k = i2 - len; k < i2 - len + dbN + 1; k++)
			{
				if ((k >= 0) && (k < len) && ((temp = har[(k - i2 + len) % len]) != 0))
					data[nlevel][i + start] += temp * data[nlevel - 1][k + start];
			}
			for (k = i2 - dbN + 1; k < i2 + dbN + 1; k++)
			{
				if ((k >= 0) && (k < len) && ((temp = har[(k - i2 + len) % len]) != 0))
					data[nlevel][i + start] += temp * data[nlevel - 1][k + start];
			}
			for (k = i2 + len - dbN + 1; k < i2 + len; k++)
			{
				if ((k >= 0) && (k < len) && ((temp = har[(k - i2 + len) % len]) != 0))
					data[nlevel][i + start] += temp * data[nlevel - 1][k + start];
			}
		}
		else
		{
			for (k = 0; k < len; k++)
			{
				if ((temp = har[(k - i2 + len) % len]) != 0)
					data[nlevel][i + start] += temp * data[nlevel - 1][k + start];
			}
		}
	}
	return 1;
}


int codeg(int nlevel, int dbN, int start, int len)
{
	int i, i2, k, len1;
	float temp;
	len1 = len >> 1;
	for (i = 0; i < len1; i++)
	{
		i2 = 2 * i;
		data[nlevel][i + start + len1] = 0.0;
		if (len > (2 * dbN))
		{
			for (k = i2 - len; k < i2 - len + dbN + 3; k++)
			{
				if ((k >= 0) && (k < len) && ((temp = gar[(k - i2 + len) % len]) != 0))
					data[nlevel][i + start + len1] += temp * data[nlevel - 1][k + start];
			}
			for (k = i2 - dbN + 3; k < i2 + dbN + 3; k++)
			{
				if ((k >= 0) && (k < len) && ((temp = gar[(k - i2 + len) % len]) != 0))
					data[nlevel][i + start + len1] += temp * data[nlevel - 1][k + start];
			}
			for (k = i2 + len - dbN + 3; k < i2 + len; k++)
			{
				if ((k >= 0) && (k < len) && ((temp = gar[(k - i2 + len) % len]) != 0))
					data[nlevel][i + start + len1] += temp * data[nlevel - 1][k + start];
			}
		}
		else
		{
			for (k = 0; k < len; k++)
			{
				if ((temp = gar[(k - i2 + len) % len]) != 0)
					data[nlevel][i + start + len1] += temp * data[nlevel - 1][k + start];
			}
		}
	}
	return 1;
}


float costfun(int numf, float thrshold, int nlevel, int nblock, int len)
{
	int start, i;
	float temp, max;

	start = nblock * len;
	max = 0.0;
	for (i = start; i < (start + len); i++)
	{
		if (numf == 1)
		{
			if (data[nlevel][i] != 0)
			{
				temp = data[nlevel][i] * data[nlevel][i];
				max -= temp * (float)log(temp);
			}
		}
		else
		{
			if (fabs(data[nlevel][i]) >= fabs(thrshold))
				max += 1.0;
			else
				data[nlevel][i] = 0.0;
		}
	}
	return max;
}


// compute the cost of each wavelet packet
int calcost(int stages, int numf, float thrshold)
{
	int nlevel, len, nblock;

	for (nlevel = stages - 1; nlevel >= 0; nlevel--)
	{
		len = datalen >> nlevel;
		for (nblock = 0; nblock < (1 << nlevel); nblock++)
			cost[nlevel][nblock] = costfun(numf, thrshold, nlevel, nblock, len);
	}
	return 1;
}


int select(int stages, int numf, float thrshold)// select best sets of wavelet packets
{
	int nlevel, nblock, blocks;

	calcost(stages, numf, thrshold);
	for (nblock = 0; nblock < (1 << (stages - 1)); nblock++)
		sig[stages - 1][nblock] = 1;
	for (nlevel = stages - 2; nlevel >= 0; nlevel--)
	{
		blocks = 1 << nlevel;
		for (nblock = 0; nblock < blocks; nblock++)
		{
			if (cost[nlevel][nblock] >= (cost[nlevel + 1][2 * nblock] + cost[nlevel + 1][2 * nblock + 1]))
			{
				sig[nlevel][nblock] = 0;
				cost[nlevel][nblock] = cost[nlevel + 1][2 * nblock] + cost[nlevel + 1][2 * nblock + 1];
			}
			else
				sig[nlevel][nblock] = 1;
		}
	}
	return 1;
}


int detailoff(int nlevel, int nblock)
{
	int len, start, j;
	if (nblock % 2 != 0)
	{
		len = datalen >> nlevel;
		start = nblock * len;
		for (j = start; j < start + len; j++)
			data[nlevel][j] = 0.0;
	}
	return 1;
}


int filter(int nlevel, int nblock)
{
	if (sig[nlevel][nblock] == 1)
		detailoff(nlevel, nblock);
	else
	{
		filter(nlevel + 1, 2 * nblock);
		filter(nlevel + 1, 2 * nblock + 1);
	}
	return 1;
}


int recompose(int dbN, int stages)
{
	int nlevel, len, nblock, start;

	for (nlevel = stages - 1; nlevel > 0; nlevel--)
	{
		len = datalen >> (nlevel - 1);
		hperiod(dbN, len);
		gperiod(dbN, len);
		for (nblock = 0; nblock < (1 << nlevel); nblock += 2)
		{
			start = nblock * (len >> 1);
			if (sig[nlevel][nblock] == 1)
			{
				recodeh(nlevel, dbN, start, len);
				sig[nlevel - 1][nblock / 2] = 1;
			}
			if (sig[nlevel][nblock + 1] == 1)
			{
				recodeg(nlevel, dbN, start, len);
				sig[nlevel - 1][nblock / 2] = 1;
			}
		}
		delete har;
		delete gar;
	}
	return 1;
}


int recodeh(int nlevel, int dbN, int start, int len)
{
	int i, k, k1, len1;
	float temp;
	len1 = len >> 1;
	for (i = 0; i < len; i++)
	{
		data[nlevel - 1][i + start] = 0.0;
		if (len > (2 * dbN))
		{
			for (k1 = (i - dbN); k1 < (i + dbN); k1++)
			{
				k = k1 / 2;
				if ((k >= 0) && (k < len1) && ((k1 % 2) == 0) && ((temp = har[(i - k1 + len) % len]) != 0))
					data[nlevel - 1][i + start] += temp * data[nlevel][k + start];
			}
			for (k1 = (i + len - dbN); k1 < (i + len + 1); k1++)
			{
				k = k1 / 2;
				if ((k >= 0) && (k < len1) && ((k1 % 2) == 0) && ((temp = har[(i - k1 + len) % len]) != 0))
					data[nlevel - 1][i + start] += temp * data[nlevel][k + start];
			}
			for (k1 = (i - len + 1); k1 < (i - len + dbN); k1++)
			{
				k = k1 / 2;
				if ((k >= 0) && (k < len1) && ((k1 % 2) == 0) && ((temp = har[(i - k1 + len) % len]) != 0))
					data[nlevel - 1][i + start] += temp * data[nlevel][k + start];
			}
		}
		else
		{
			for (k = 0; k < len1; k++)
			{
				if ((temp = har[(i - 2 * k + len) % len]) != 0)
					data[nlevel - 1][i + start] += temp * data[nlevel][k + start];
			}
		}
	}
	return 1;
}


int recodeg(int nlevel, int dbN, int start, int len)
{
	int i, k, k1, len1;
	float temp;
	len1 = len >> 1;
	for (i = 0; i < len; i++)
	{
		if (len > (2 * dbN))
		{
			for (k1 = (i + len - dbN - 2); k1 < (i + len); k1++)
			{
				k = k1 / 2;
				if ((k >= 0) && (k < len1) && ((k1 % 2) == 0) && ((temp = gar[(i - k1 + len) % len]) != 0))
					data[nlevel - 1][i + start] += temp * data[nlevel][k + start + len1];
			}
			for (k1 = (i - dbN - 2); k1 < (i + dbN - 2); k1++)
			{
				k = k1 / 2;
				if ((k >= 0) && (k < len1) && ((k1 % 2) == 0) && ((temp = gar[(i - k1 + len) % len]) != 0))
					data[nlevel - 1][i + start] += temp * data[nlevel][k + start + len1];
			}
			for (k1 = (i - len); k1 < (i - len + dbN - 2); k1++)
			{
				k = k1 / 2;
				if ((k >= 0) && (k < len1) && ((k1 % 2) == 0) && ((temp = gar[(i - k1 + len) % len]) != 0))
					data[nlevel - 1][i + start] += temp * data[nlevel][k + start + len1];
			}
		}
		else
		{
			for (k = 0; k < len1; k++)
			{
				if ((temp = gar[(i - 2 * k + len) % len]) != 0)
					data[nlevel - 1][i + start] += temp * data[nlevel][k + start + len1];
			}
		}
	}
	return 1;
}

