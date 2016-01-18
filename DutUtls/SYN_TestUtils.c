#include "SYN_TestUtils.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define CAL_LIMIT_VIPER_MIN 96.0
#define CAL_LIMIT_VIPER_MAX 160.0

static float m_matrix[144][144];

//sharpness test kernels
static int m_kernelSharpnessGx[3][3] = {{1, 2, 1},{0,0,0},{-1,-2,-1}};
static int m_kernelSharpnessGy[3][3] = {{1, 0, -1},{2,0,-2},{1,0,-1}};

void SYN_GetVersion(int* pMajorVer, int* pMinorVer, int* pDevelopmentVer)
{
	*pMajorVer			= MAJOR_VER;
	*pMinorVer			= MINOR_VER;
	*pDevelopmentVer	= DEVOLOPMENT_VER;
}

void SYN_AFETestExecute(AFETestInfo* pInfo, AFETestResults* pResults)
{
	pResults->m_bPass = 0;

	if (pResults->m_pResponse[4] & 0x01)
		pResults->m_bPass = 1;
	else
		pResults->m_bPass = 0;
}

void SYN_SDKBaselineTest(SdkBaselineTestInfo* pInfo, SdkBaselineTestResults* pResults, AcquireFPSResults* nofingerdata, int numRows, int numCols, int numFrames)
{
	int i,j,k;
	int nTempSum;
	int nLineVariance;

	uint8_t arrLine[MAXCOL];
	uint8_t *pLine = &arrLine[0];

	float nPercentError;

	//Trim values
	int nTrimTop   = pInfo->m_nTrimTop;
	int nTrimBot   = pInfo->m_nTrimBottom;
	int nTrimLeft  = pInfo->m_nTrimLeft;
	int nTrimRight = pInfo->m_nTrimRight;	
	int nHorizMargin = nTrimLeft + nTrimRight;	
	
	pResults->m_bPass = 1;
	pResults->m_nNumBadRows = 0;
	pResults->m_nNumGoodRows = 0;

	//Get the average no finger image with header bytes included
	for(i=nTrimTop; i<numRows - nTrimBot; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				nTempSum+=nofingerdata->m_arImagesWithoutStimulus[k].arr[i][j];				
			}
			arrLine[j] = (uint8_t) (nTempSum/numFrames);			
			nTempSum=0;
		}
		pLine = &arrLine[0];
		nLineVariance = getWovarLineVariance((pLine + HEADER + nHorizMargin), (numCols - HEADER - nHorizMargin),pInfo->m_nStrideWidth,pInfo->m_nRightShiftBits);

		if (nLineVariance > pInfo->m_nMaxSumAllowed)
			pResults->m_nNumBadRows++;
		else
			pResults->m_nNumGoodRows++;
	}

	nPercentError = (float) 100*(pResults->m_nNumBadRows)/(numRows-(nTrimTop+nTrimBot)); //number of failing rows over the total number of rows
	if (nPercentError > pInfo->m_nPercentFail)
		pResults->m_bPass = 0;
}

int getWovarLineVariance(uint8_t *pData, int nLength, int nStride, int nBitsIgnored)
{
    int     nVariance = 0;
    int     nDelta;
    int     nIndex;

    for (nIndex = nStride; nIndex < nLength; nIndex++)
    {
        /* get the absolute delta of the most significant 5-bits of the 2 
           nearby pixels */
        nDelta = abs((pData[nIndex] >> nBitsIgnored) - (pData[nIndex - nStride] >> nBitsIgnored));

        nVariance += nDelta;
    }
    return nVariance;
}

void SYN_MPC_VoltageTestExecute(InitializationInfo* pInfo, InitializationResults* pResults, uint32_t arValues[MPC_SELF_TEST_BUFFER])
{
	float c1 = (float)0.000732;
	float nMeasuredVdd   = 0;
	float nMeasuredVio   = 0;
	float nMeasuredVddtx = 0;
	float nMeasuredVled  = 0;
 	float nActualdVdd    = (float) pInfo->m_nVdd / 1000;
	float nActualVio     = (float) pInfo->m_nVio / 1000;
	float nActualVddtx   = (float) pInfo->m_nVddtx / 1000;
	float nActualVled    = (float) pInfo->m_nVled / 1000;

	float nPercentLimit = (float)0.05; //hardcoded value of 5%
	float nPercentError = 0;

	//Set result to 1 
	pResults->bMpcVoltagesPass = 1;

	//Calculate voltages
	nMeasuredVdd   = arValues[0] * c1 * 2;
	nMeasuredVio   = arValues[1] * c1 * 2;
	nMeasuredVddtx = arValues[2] * c1 * 2;
	nMeasuredVled  = arValues[3] * c1 * 2;
	
	//Assign calculated results to the Results struct
	pResults->m_nMeasuredVdd   = nMeasuredVdd;
	pResults->m_nMeasuredVio   = nMeasuredVio;
	pResults->m_nMeasuredVddtx = nMeasuredVddtx;
	pResults->m_nMeasuredVled  = nMeasuredVled;

	//Find percentage error 
	nPercentError = (nActualdVdd == (float)0.0) ? (float)0.05 : (float)fabs((nMeasuredVdd - nActualdVdd)/nActualdVdd);
	if (nPercentError > nPercentLimit)
		pResults->bMpcVoltagesPass = 0;
	
	nPercentError = (nActualVio == (float)0) ? (float)0.05 : (float)fabs((nMeasuredVio - nActualVio)/nActualVio);
	if (nPercentError > nPercentLimit)
		pResults->bMpcVoltagesPass = 0;
	
	nPercentError = (nActualVddtx == (float)0) ? (float)0.05 : (float)fabs((nMeasuredVddtx - nActualVddtx)/nActualVddtx);
	if (nPercentError > nPercentLimit)
		pResults->bMpcVoltagesPass = 0;
	
	nPercentError = (nActualVled ==(float) 0) ? (float)0.05 : (float)fabs((nMeasuredVled - nActualVled)/nActualVled);
	if (nPercentError > nPercentLimit)
		pResults->bMpcVoltagesPass = 0;	
}

float find_mean(float* arr,int S_COL)
{
	int i;
	float tempsum=0.0;
	for(i=0; i<S_COL; i++)
	{
		tempsum+=arr[i];
	}

	return tempsum/S_COL;
}


float find_noise(SNRResults * pRes,int S_ROW, int S_COL, int regions)
{
	int row, col;
	float temp=0.0;

	for(row=0; row<S_ROW; row++)
	{
		for(col=0; col<S_COL; col++)
		{
			temp+=pRes->STD[regions].arr[row][col];
		}
	}

	return temp/(S_ROW*S_COL);
}

void find_bins(SNRResults *pRes,int S_ROW, int S_COL, int bins[HISTOGRAM],int regions)
{
	int row, col, minlim, maxlim,i;
	int count=0;
	float temp;

	for(i=0; i<HISTOGRAM;i++)
	{
		if (i==0)
		{
			minlim=-1000;
			maxlim=0;
		}
		else if(i==HISTOGRAM-1)
		{
			minlim=i;
			maxlim=1000;
		}
		else
		{
			minlim=i;
			maxlim=i+1;
		}

		for(row=0; row<S_ROW; row++)
		{
			for(col=0; col<S_COL; col++)
			{			
				temp=pRes->NORM_AVGS[regions].arr[row][col];
	
				if(temp>= minlim && temp<maxlim)
					count+=1;
			}
		}
		
		bins[i]=count;
		count=0;
	}
}

int sum(int arr[HISTOGRAM],int firstindex,int lastindex)
{
	int i;
	int tempsum=0;

	for(i=firstindex; i<lastindex; i++)
	{
		tempsum+=arr[i];
	}
	
	return tempsum;
}

int get_signal_value(FPSFrame* fingerdata, FPSFrame* nofingerdata, SNRInfo* info, int minrow, int maxrow, int mincol, int maxcol,int regions, SNRResults* results)//,int s_ROW, int s_COL)
{
		int p_percent,row, col, layer,i,S_ROW,S_COL,SIGNAL=0, signalmin=0, signalmax=0, signalsum_min=0, signalsum_max=0;
		int temp_fing=0, temp_nofing=0;
		float mean=0.0, tempsum_finger=0.0, tempsum_nofinger=0.0;
		float percent;
		
		
		int bins[HISTOGRAM]={0};
		// float temp_mean[MAXC-MINC];
		float* pTemp_mean = (float*) malloc((info->numMaxCols-info->numMinCols)*sizeof(float));

		p_percent=info->percent;
		percent=(float)p_percent/100;

		S_ROW=maxrow-minrow;
		S_COL=maxcol-mincol;

		//temp_mean =(float*) malloc(S_COL * sizeof(float *));
		
		for(row=minrow; row<maxrow; row++)	
			{
			for(col=mincol; col<maxcol; col++)
			{
				for(layer=0; layer<info->numFrames;layer++)
				{			
					temp_fing+=fingerdata[layer].arr[row][col];
				}
				tempsum_finger=(float) temp_fing/info->numFrames;
				//Truncate
				if (tempsum_finger > 255 || tempsum_finger < 0)
					tempsum_finger = tempsum_finger > 255 ? (float) 255 : (float) 0;

				for(layer=0; layer<info->numFrames;layer++)
				{	
					temp_nofing+=nofingerdata[layer].arr[row][col];
				}
				tempsum_nofinger=(float) temp_nofing/info->numFrames;
				//Truncate
				if (tempsum_nofinger > 255 || tempsum_nofinger < 0)
					tempsum_nofinger = tempsum_nofinger > 255 ? (float) 255 : (float) 0;
				/*---BEGIN PIXEL_AVGS---*/			
				results->PIXEL_AVGS[regions].arr[row-minrow][col-mincol]=tempsum_finger-tempsum_nofinger+128;
				/*---ENDPIXEL_AVGS---*/
				pTemp_mean[col-mincol]=tempsum_finger-tempsum_nofinger+128;//results->PIXEL_AVGS[regions].arr[row-minrow][col-mincol];//for calculating the norm_avgs
				//Truncate
				if (pTemp_mean[col-mincol] > 255 || pTemp_mean[col-mincol] < 0)
					pTemp_mean[col-mincol] = pTemp_mean[col-mincol] > 255 ? (float) 255 : (float) 0;

				tempsum_finger=0.0;
				tempsum_nofinger=0.0;
				temp_fing=0;
				temp_nofing=0;
			}

			for(i=0; i<S_COL; i++)
			{
				results->NORM_AVGS[regions].arr[row-minrow][i]=results->PIXEL_AVGS[regions].arr[row-minrow][i]-find_mean(pTemp_mean,S_COL)+(float)128.00;
				//Truncate
				if (results->NORM_AVGS[regions].arr[row-minrow][i] > 255 || results->NORM_AVGS[regions].arr[row-minrow][i] < 0)
					results->NORM_AVGS[regions].arr[row-minrow][i] = results->NORM_AVGS[regions].arr[row-minrow][i] > 255 ? (float) 255 : (float) 0;
			}
		}		

	find_bins(results,S_ROW,S_COL,bins,regions);
	

	signalmin=0;	
	signalsum_min=0;
	for(i=1; i<HISTOGRAM; i++)
	{
		if(signalsum_min==0)
		{
			if(sum(bins,0,i)>=0.5*percent*sum(bins,0,HISTOGRAM))
				signalmin=i;
			else 
				signalmin=0;
		}
		else
			signalmin=0;
				
		signalsum_min+=signalmin;
	}
	
	signalmax=0;	
	signalsum_max=0;
	for(i=1; i<HISTOGRAM; i++)
	{
		if(signalsum_max==0)
		{
			if(sum(bins,0,i)>=(1-0.5*percent)*sum(bins,0,HISTOGRAM))
				signalmax=i;
			else 
				signalmax=0;
		}
		else
			signalmax=0;

		signalsum_max+=signalmax;
			
	}
	SIGNAL=signalsum_max-signalsum_min;
	
	free(pTemp_mean);
	pTemp_mean = NULL;

	return SIGNAL;

}

float get_noise_value(FPSFrame* fingerdata, FPSFrame* nofingerdata, SNRInfo* info, int minrow, int maxrow, int mincol, int maxcol,int regions,SNRResults* results)//,int s_ROW, int s_COL)
{
	int row, col, layer,i,S_ROW,S_COL;
	float mean=0.0, sum_deviation=0.0, tempsum_nofinger=0.0;
	

	short temp[MAXFRAMES]={0};	

	S_ROW=maxrow-minrow;
	S_COL=maxcol-mincol;	

	for(row=minrow; row<maxrow; row++)
	{
		for(col=mincol; col<maxcol; col++)
		{
			for(layer=0; layer<info->numFrames; layer++)
			{
				*(temp+layer)=(short)nofingerdata[layer].arr[row][col];
				mean+=*(temp+layer);
			}
	
			tempsum_nofinger=mean/info->numFrames;
			mean=mean/info->numFrames;
			for(i=0; i<info->numFrames;++i)
				sum_deviation+=(*(temp+i)-mean)*(*(temp+i)-mean);
			//NUMFRAMES from info structure.
			results->STD[regions].arr[row-minrow][col-mincol]=(float)sqrt(sum_deviation/(info->numFrames-1));
		
			sum_deviation=0.0;
			mean=0.0;

		}
	}
	
		return find_noise(results, S_ROW, S_COL,regions);		

}

void SYN_SNRExecute(AcquireFPSResults* fingerdata, AcquireFPSResults* nofingerdata, SNRInfo* info,SNRResults* results,int numRows, int numCols, int bIsBga)
{
	float NOISE=0.0, SNR=0.0;
	
	int minrow, mincol, maxcol, maxrow, SIGNAL=0;//;, s_ROW,s_COL;
	int i,j,k;//for 9 regions
	int M, N; //no of elements in row and columns of the segment window.
	int C0,C1,C2,R0,R1;
	int C[4], R[3];

	int all_zones_passed=1;
	int overall_passed=1;
	
	//debug
	for (i=0; i<10; i++)
	{
		results->SNR[i] = 0;
		results->SIGNAL[i] = 0;
		results->NOISE[i] = 0;
	}

	// Get min/max cols and min/max rows from SNRInfo struct
	N=info->numMaxCols - (info->numMinCols + HEADER);
	M=info->numMaxRows - info->numMinRows;
	 
	//ZONE DIVISION STARTS HERE
	C0 = N/3;
	C1 = C0+C0;
	C2 = N;
	C[0]=0;
	C[1]=C0;
	C[2]=C1;
	C[3]=C2;
	//
	R0=M/2;
	R1=R0+R0;	
	R[0]=0;
	R[1]=R0;
	R[2]=R1;
	
	SNR_Fill_Log(fingerdata->arr_finger,nofingerdata->arr_nofinger,results,numRows,numCols,info->numFrames);

	k=0;
	for(i=0; i<2; i++)//row
	{
		for(j=0; j<3; j++)//col
		{			
			if(k==0)
			{
				mincol=info->numMinCols + C[0] + HEADER;
				maxcol=C[1] + HEADER;
				minrow=R[0];
				maxrow=R[1];
			}
			if (k==1)
			{
				mincol=C[1] + HEADER;
				maxcol=C[2] + HEADER;
				minrow=R[0];
				maxrow=R[1];				
			}
			if (k==2)
			{
				mincol=C[2] + HEADER;
				maxcol=C[3] + HEADER;
				minrow=R[0];
				maxrow=R[1];
			}
			if (k==3)
			{
				mincol=info->numMinCols+HEADER+C[0];
				maxcol=C[1] + HEADER;
				minrow=R[1];
				maxrow=R[2];
			}
			if (k==4)
			{
				mincol=C[1] + HEADER;
				maxcol=C[2] + HEADER;
				minrow=R[1];
				maxrow=R[2];
			}
			if (k==5)
			{
				mincol=C[2] + HEADER;
				maxcol=C[3] + HEADER;
				minrow=R[1];
				maxrow=R[2];
			}			

			SIGNAL=get_signal_value(fingerdata->arr_finger,nofingerdata->arr_nofinger,info, minrow, maxrow, mincol, maxcol,k,results);
			NOISE=get_noise_value(fingerdata->arr_finger,nofingerdata->arr_nofinger,info, minrow, maxrow, mincol, maxcol,k,results);//,s_ROW,s_COL);
			SNR=SIGNAL/NOISE;

			results->SIGNAL[k]=SIGNAL;
			results->NOISE[k]=NOISE;
			results->SNR[k]=SNR;
			
			SIGNAL=0;
			NOISE=0.0;
			SNR=0.0;
			k+=1;

			if(k==6)//if the overall window is looked at.
			{
				minrow=info->numMinRows;
				maxrow=info->numMaxRows;
				mincol=info->numMinCols+HEADER;
				maxcol=info->numMaxCols;
			
				SIGNAL=get_signal_value(fingerdata->arr_finger,nofingerdata->arr_nofinger,info, minrow, maxrow, mincol, maxcol,k,results);
				NOISE=get_noise_value(fingerdata->arr_finger,nofingerdata->arr_nofinger,info, minrow, maxrow, mincol, maxcol,k,results);//,s_ROW,s_COL);
				SNR=SIGNAL/NOISE;
	
				results->SIGNAL[k]=SIGNAL;
				results->NOISE[k]=NOISE;
				results->SNR[k]=SNR;
			}
		}
		
	}
	
	//region 6 is the full picture.
	for(k=0; k<7; k++)
	{
		if (!bIsBga)
		{
			if( k!= 6)//zones
			{
				if( !( (results->SIGNAL[k] >= info->limit_zone_signal_low && results->SIGNAL[k] <= info->limit_zone_signal_high) && 
					   (results->NOISE[k]  >= info->limit_zone_noise_low  && results->NOISE[k]  <= info->limit_zone_noise_high) && 
					   (results->SNR[k]    >= info->limit_zone_snr_low    && results->SNR[k]    <= info->limit_zone_snr_high))
				  )
					all_zones_passed=0;			
			}
			else //overall
			{
				if( !( (results->SIGNAL[k] >= info->limit_overall_signal_low && results->SIGNAL[k] <= info->limit_overall_signal_high) && 
					   (results->NOISE[k]  >= info->limit_overall_noise_low  && results->NOISE[k]  <= info->limit_overall_noise_high) && 
					   (results->SNR[k]    >= info->limit_overall_snr_low    && results->SNR[k]    <= info->limit_overall_snr_high))
				  )
					overall_passed=0;
			}
		}
		else
		{
			if( k!= 6)//zones
			{
				if ( !((results->NOISE[k]  >= info->limit_zone_noise_low  && results->NOISE[k]  <= info->limit_zone_noise_high)))
					all_zones_passed=0;						
			}
			else //overall			
			{
				if( !((results->NOISE[k]  >= info->limit_overall_noise_low  && results->NOISE[k]  <= info->limit_overall_noise_high)))
					overall_passed=0;
			}			
		}
	}
	if( all_zones_passed && overall_passed )
		results->bPass=1;
	else
		results->bPass=0;	
}

uint8_t* get_frame_ptr(int nFrameIdx, uint8_t* pFrames, int nFrameSize)
{
	return (&pFrames[nFrameIdx * nFrameSize]);
}
//Cal_FPSFrame* get_frame_ptr(Cal_FPSSample *smp,int frameNo)
//{
//	return (&smp->arr[frameNo]);
//}

//fills the result structure's all members for SNR TEST that "will be logged"
void SNR_Fill_Log(FPSFrame* fingerdata, FPSFrame* nofingerdata, SNRResults* pResults, int numRows, int numCols, int numFrames)
{
	int i,j,k,l;
	int min, max;
	float mean=0.0, sum_deviation=0.0;
	int temp=0;
	int span[MAXFRAMES]={0};

	//nofinger avg
	for (i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				temp+=nofingerdata[k].arr[i][j];
			}

			pResults->AVG_NOFINGER[i][j]= (float) temp/numFrames;
			//Truncate
			if (pResults->AVG_NOFINGER[i][j] > 255 || pResults->AVG_NOFINGER[i][j] < 0)
				pResults->AVG_NOFINGER[i][j] = pResults->AVG_NOFINGER[i][j] > 255 ? (float) 255 : (float) 0;
			temp=0;
		}
	}

	//finger avg
	temp=0;
	for (i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				temp+=fingerdata[k].arr[i][j];
			}

			pResults->AVG_FINGER[i][j]= (float) temp/numFrames;
			//Truncate
			if (pResults->AVG_FINGER[i][j] > 255 || pResults->AVG_FINGER[i][j] < 0)
				pResults->AVG_FINGER[i][j] = pResults->AVG_FINGER[i][j] > 255 ? (float) 255 : (float) 0;
			temp=0;
		}
	}

	//nofinger span
	for (i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				span[k]=nofingerdata[k].arr[i][j];				
			}
			get_span(span,&min,&max, numFrames);
			pResults->SPAN_NOFINGER[i][j]=(float)max-min;			
			//Truncate
			if (pResults->SPAN_NOFINGER[i][j] > 255 || pResults->SPAN_NOFINGER[i][j] < 0)
				pResults->SPAN_NOFINGER[i][j] = pResults->SPAN_NOFINGER[i][j] > 255 ? (float) 255 : (float) 0;
			max=0;
			min=0;			
		}
	}
	//finger span	
	for (i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				span[k]=fingerdata[k].arr[i][j];				
			}
			get_span(span,&min,&max, numFrames);
			pResults->SPAN_FINGER[i][j]=(float)max-min;			
			//Truncate
			if (pResults->SPAN_FINGER[i][j] > 255 || pResults->SPAN_FINGER[i][j] < 0)
				pResults->SPAN_FINGER[i][j] = pResults->SPAN_FINGER[i][j] > 255 ? (float) 255 : (float) 0;
			max=0;
			min=0;
		}
	}

	
	//nofinger std
	for(i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				*(span+k)=nofingerdata[k].arr[i][j];
				mean+=*(span+k);
			}			
			mean=mean/numFrames;
			for(l=0; l<numFrames;++l)
				sum_deviation+=(*(span+l)-mean)*(*(span+l)-mean);
			//printf("%d\tstd_row\n",col-mincol);			
			pResults->STD_NOFINGER[i][j]=(float)sqrt(sum_deviation/(numFrames-1));				
			//Truncate
			if (pResults->STD_NOFINGER[i][j] > 255 || pResults->STD_NOFINGER[i][j] < 0)
				pResults->STD_NOFINGER[i][j] = pResults->STD_NOFINGER[i][j] > 255 ? (float) 255 : (float) 0;
			sum_deviation=0.0;
			mean=0.0;
		}
	}

	//finger std
	sum_deviation=0.0;
	mean=0.0;
	for(i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				*(span+k)=fingerdata[k].arr[i][j];
				mean+=*(span+k);
			}			
			mean=mean/numFrames;
			for(l=0; l<numFrames;++l)
				sum_deviation+=(*(span+l)-mean)*(*(span+l)-mean);
			//printf("%d\tstd_row\n",col-mincol);			
			pResults->STD_FINGER[i][j]=(float)sqrt(sum_deviation/(numFrames-1));				
			//Truncate
			if (pResults->STD_FINGER[i][j] > 255 || pResults->STD_FINGER[i][j] < 0)
				pResults->STD_FINGER[i][j] = pResults->STD_FINGER[i][j] > 255 ? (float) 255 : (float) 0;
			sum_deviation=0.0;
			mean=0.0;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void SYN_RetainModeExecute(RetainModeInfo* pInfo, RetainModeResults* pResults)
{
	pResults->m_bPass = 1;

	if ( (pResults->m_nRetainModeCurrent > pInfo->m_nMaxCurrent) || (pResults->m_nRetainModeCurrent < pInfo->m_nMinCurrent) )
		pResults->m_bPass = 0;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void SYN_WofLowPowerExecute(WofLowPowerInfo* pInfo, WofLowPowerResults* pResults)
{
	pResults->m_bPass = 1;

	if ( (pResults->m_nCurrent_uA > pInfo->m_nMaxCurrent_uA) || (pResults->m_nCurrent_uA < pInfo->m_nMinCurrent_uA) )
		pResults->m_bPass = 0;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void SYN_SCMWofTestExecute(SCM_WofTestInfo* pInfo, SCM_WofTestResults* pResults)
{
	int			i;
	int			nTgrIdx;
	uint8_t		m_arGains[2] = {0};
	int			nRecSize = (pInfo->m_nNumResponseBytes-6) / 2;
	int			bTgrFound = 0;
	int			nDelta;
	uint8_t*	pBuf;

	pResults->m_bPass = 1;	//Assume true.

	if (pInfo->m_bWithStimulus == 0)  // If NO Stimulus
	{
		//Find the trigger index from gain of 100.
		nTgrIdx = 0;
		for (i=0; (i<nRecSize) && (bTgrFound==0); i++)
		{
			if (pInfo->m_arWofData[i+6] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		pResults->m_nRecIdxWithoutStim = 0;
		pResults->m_nTriggerIdxWithoutStim_100 = nTgrIdx - 1;

		//Get the trigger index for the second gain.
		nTgrIdx = 0;
		bTgrFound = 0;
		for (i=0; (i<nRecSize) && (bTgrFound==0); i++)
		{
			if (pInfo->m_arWofData[i + 6 + nRecSize] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		pResults->m_nTriggerIdxWithoutStim_200 = nTgrIdx-1;

		//If invalid threshold from gain of 100.
		if ((nTgrIdx < pInfo->m_nMinTriggerThreshold) || (nTgrIdx >= pInfo->m_nMaxTriggerThreshold))
			pResults->m_nRecIdxWithoutStim = 1;
	}
	else if (pResults->m_nWithStimCount == 0)	//With stimulus at 3.3V.
	{
		pResults->m_bPassAtGain100 = 1;
		pResults->m_bPassAtGain200 = 1;
		pResults->m_bPassAtGain200_3p7V = 1;

		//Get trg index at gain 100.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &pInfo->m_arWofData[6];
		for (i=0; (i<nRecSize) && (bTgrFound==0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		pResults->m_nTriggerIdxWithStim_100 = nTgrIdx-1;

		//Get trg index at gain 200.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &pInfo->m_arWofData[6 + nRecSize];
		for (i=0; (i<nRecSize) && (bTgrFound==0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		pResults->m_nTriggerIdxWithStim_200 = nTgrIdx-1;

		nDelta = pResults->m_nTriggerIdxWithoutStim_100 - pResults->m_nTriggerIdxWithStim_100;
		if (nDelta <= pInfo->m_nDelta_100)
		{
			pResults->m_bPassAtGain100 = 0;
			nDelta = pResults->m_nTriggerIdxWithoutStim_200 - pResults->m_nTriggerIdxWithStim_200;
			if (nDelta <= pInfo->m_nDelta_200)
			{
				pResults->m_bPassAtGain200 = 0;
				pResults->m_bPass = 0;
			}
		}
		pResults->m_nWithStimCount++;
	}
	else //With stimulus at 3.7V.
	{
		//Get trg index at gain 200.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &pInfo->m_arWofData[6 + nRecSize];
		for (i=0; (i<nRecSize) && (bTgrFound==0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}

		// To save log for 3.7v pass
        pResults->m_nTriggerIdxWithStim_200_3p7 = nTgrIdx-1;

		nDelta = pResults->m_nTriggerIdxWithoutStim_200 - (nTgrIdx-1);
		if (nDelta <= pInfo->m_nDelta_200_3p7)
		{
			pResults->m_bPassAtGain200_3p7V = 0;
			pResults->m_bPass = 0;
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void SYN_WofTestExecute(WofTestInfo* pInfo, WofTestResults* pResults)
{
	int			i;
	int			nTgrIdx;
	uint8_t		m_arGains[2] = {0};
	int			nRecSize = (pInfo->m_nNumResponseBytes-6) / 2;
	int			bTgrFound = 0;
	int			nDelta;
	uint8_t*	pBuf;
		
	pResults->m_bPass = 1;	//Assume true.

	if (pInfo->m_bWithStimulus == 0)  // If NO Stimulus
	{
		//Find the trigger index from gain of 100.
		nTgrIdx = 0;
		for (i=0; (i<nRecSize) && (bTgrFound==0); i++)
		{
			if (pInfo->m_arWofData[i+6] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		pResults->m_nRecIdxWithoutStim = 0;
		pResults->m_nTriggerIdxWithoutStim_100 = nTgrIdx - 1;

		//Get the trigger index for the second gain.
		bTgrFound = 0;
		nTgrIdx = 0;
		for (i=0; (i<nRecSize) && (bTgrFound==0); i++)
		{
			if (pInfo->m_arWofData[i + 6 + nRecSize] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		pResults->m_nTriggerIdxWithoutStim_200 = nTgrIdx-1;

		//If invalid threshold from gain of 100.
		if ((nTgrIdx < pInfo->m_nMinTriggerThreshold) || (nTgrIdx >= pInfo->m_nMaxTriggerThreshold))
			pResults->m_nRecIdxWithoutStim = 1;
	}
	else if (pResults->m_nWithStimCount == 0)	//With stimulus at 3.3V.
	{
		pResults->m_bPassAtGain100 = 1;
		pResults->m_bPassAtGain200 = 1;
		pResults->m_bPassAtGain200_3p7V = 1;

		//Get trg index at gain 100.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &pInfo->m_arWofData[6];
		for (i=0; (i<nRecSize) && (bTgrFound==0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		pResults->m_nTriggerIdxWithStim_100 = nTgrIdx-1;

		//Get trg index at gain 200.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &pInfo->m_arWofData[6 + nRecSize];
		for (i=0; (i<nRecSize) && (bTgrFound==0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		pResults->m_nTriggerIdxWithStim_200 = nTgrIdx-1;

		nDelta = pResults->m_nTriggerIdxWithoutStim_100 - pResults->m_nTriggerIdxWithStim_100;
		if (nDelta <= pInfo->m_nDelta_100)
		{
			pResults->m_bPassAtGain100 = 0;
			nDelta = pResults->m_nTriggerIdxWithoutStim_200 - pResults->m_nTriggerIdxWithStim_200;
			if (nDelta <= pInfo->m_nDelta_200)
			{
				pResults->m_bPassAtGain200 = 0;
				pResults->m_bPass = 0;
			}
		}
		pResults->m_nWithStimCount++;
	}
	else //With stimulus at 3.7V.
	{
		//Get trg index at gain 200.
		nTgrIdx = 0;
		bTgrFound = 0;
		pBuf = &pInfo->m_arWofData[6 + nRecSize];
		for (i=0; (i<nRecSize) && (bTgrFound==0); i++)
		{
			if (pBuf[i] == 0)
				nTgrIdx++;
			else
				bTgrFound = 1;
		}
		
		// To save log for 3.7v pass
        pResults->m_nTriggerIdxWithStim_200_3p7 = nTgrIdx-1;

		nDelta = pResults->m_nTriggerIdxWithoutStim_200 - (nTgrIdx-1);
		if (nDelta <= pInfo->m_nDelta_200_3p7)
		{
			pResults->m_bPassAtGain200_3p7V = 0;
			pResults->m_bPass = 0;
		}
	}	
}

void SYN_WoVarTestExecute(WoVarInfo* pInfo, WoVarResults* pResults)
{
	if(pResults->m_pResponse[4] & 0x01)
		pResults->m_bPass = 1;
	else
		pResults->m_bPass = 0;
}

void SYN_PixelPatchTestExecute(PixelPatchInfo* pInfo, PixelPatchResults *pResults)
{
	pResults->m_bPass = 0;
	if (pResults->m_pResponse[4] & 0x01)
		pResults->m_bPass = 1;	
	else
		pResults->m_bPass = 0;

}

void SYN_SpiFlashTestExecute(SpiFlashInfo* pInfo, SpiFlashResults *pResults)
{
	pResults->m_bPass = 0;
	if (pResults->m_pResponse[4] & 0x01)
		pResults->m_bPass = 1;	
	else
		pResults->m_bPass = 0;

}

void SYN_PixelTestExecute(AcquireFPSResults* nofingerdata,PixelInfo* info, PixelResults* results,
							int nTrimLeft, int nTrimRight, int nTrimTop, int nTrimBottom, int numRows, int numCols)
{
	//:
	int   i, j, k;
	int   temp, numFrames;
	int   row_limit, col_limit;
	float span_row[MAXROW]={0};
	float span_col[MAXCOL]={0};
	float mean=0.0, sum_deviation=0.0;
	float mean_rows=0.0;
	float mean_cols=0.0;
	int   sizeRows=(numRows-nTrimBottom-1-4)-(nTrimTop+1);
	
	int nMaxPixel = 0;
	int nMinPixel = -1;

	int	nCountBadPixel=0;
	row_limit=info->fail_row;
	col_limit=info->fail_col;

	temp=0,	k=0;
	//set result to pass before anything is done...
	results->bPass=1;
	numFrames=info->numFrames;

	for(i=nTrimTop; i<numRows-nTrimBottom; i++)
	{
		for(j=nTrimLeft+HEADER; j<numCols-nTrimRight; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				temp+=nofingerdata->arr_nofinger[k].arr[i][j];		
			}
			results->A[i-nTrimTop][j-nTrimLeft-HEADER]=(int)temp/numFrames;
			temp=0;
		}
	}

	for( i = 0; i < numRows-(nTrimBottom+nTrimTop) - 1; i++ )
	{
		for( j = 0; j < numCols-(HEADER+nTrimLeft+nTrimRight) - 1; j++ )
		{
			results->DA[i][j]=( results->A[i+1][j+1] - results->A[i+1][j] ) - ( results->A[i][j+1] - results->A[i][j] ); //DA => difference image
		}
	}

	temp=0;
	//process DA(difference image) according to Mike's requested hardcoded values of 8 from each edge
	//find max
	for( i = 8; i < numRows-(nTrimTop+nTrimBottom) - 8; i++ )
	{
		for( j = 8; j < numCols-(HEADER+nTrimLeft+nTrimRight) - 8; j++ )
		{
			temp=results->DA[i][j];
			//count the bad pixels
			if( (temp >= info->nThresholdRange) || (temp <= -(info->nThresholdRange)) )
				nCountBadPixel+=1;

			//find min and max in the 2d diff image
			if(temp <= nMinPixel)
				nMinPixel = temp;
			if(temp > nMaxPixel)
				nMaxPixel = temp;
		}
	}

	results->nMaxPixelValue			= nMaxPixel;
	results->nMinPixelValue			= nMinPixel;
	results->nCountAboveThreshold	= nCountBadPixel;
	
	if(nCountBadPixel > info->nCountFailedPixels)
		results->bPass=0;	
}

void SYN_ConsecutivePixelsExecute(AcquireFPSResults* nofingerdata, ConsecutivePixelsInfo* info, ConsecutivePixelsResults* results,
									int nTrimLeft, int nTrimRight, int nTrimTop, int nTrimBottom, int numRows, int numCols)
{
	int numFrames,i,j,k,temp=0;
	int failRowPegged,failRowFloored,countRow,countCol,failLimit; //these are filled out by structure from .cpp file.
	int temp_sum=0;
	int temp_val=0;
	int temp_consecutive[MAXCOL];
	int temp_col[MAXCOL];
	int temp_row[MAXROW];
	numFrames=info->numFrames;
	
	//initialize temp_consecutive
	for(i=0; i<numCols; i++)
		temp_consecutive[i]=-1;
	//init. temp_row and temp_col
	for(i=0; i<numCols; i++)
		temp_col[i]=0;
	for(i=0; i<numRows; i++)
		temp_row[i]=0;

	countRow=0;
	countCol=0;
	results->m_bPass=1;//set the device to pass to begin with.
	
	//eliminate=info->m_cutEdge;
	
	failRowPegged=info->m_peggedThreshold;
	failRowFloored=info->m_flooredThreshold;
	failLimit=info->m_maxAdjacentPixelsAllowed;//4

	//initialize the structs.
	for(i=0; i<numCols-HEADER-(nTrimLeft+nTrimRight); i++)
	{
		results->consecutive_floored_cols[i]=0;
		results->consecutive_pegged_cols[i]=0;
	}
	for(i=0; i<numRows-(nTrimTop+nTrimBottom); i++)
	{
		results->consecutive_floored_rows[i]=0;
		results->consecutive_pegged_rows[i]=0;
	}

	//set up the nofinger avg matrix that is obtained by taking the arithmetic mean of 30 frames of nofinger data.
	for(i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				temp_sum+=nofingerdata->m_arImagesWithoutStimulus[k].arr[i][j];				
			}
			
			temp_val=temp_sum/numFrames;
			results->avg[i][j]=temp_val;
			
			temp_sum=0;
			temp_val=0;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	temp=0;
	//scan through each row.
	for(i=nTrimTop; i<numRows-nTrimBottom; i++)
	{
		for(j=HEADER+nTrimLeft; j<numCols-nTrimRight; j++)
		{
			if( (results->avg[i][j] >= failRowPegged) )
			{
				temp++;			
				temp_consecutive[j-HEADER-nTrimLeft]=temp;
			}
			else
			{
				temp=0;
				temp_consecutive[j-HEADER-nTrimLeft]=temp;
			}

			if(temp >= failLimit)//fail the test if the count is greater than our expected limits.
				results->m_bPass=0;
		}				
		temp_row[i-nTrimTop]=max_array(&temp_consecutive[0],numCols-HEADER-(nTrimLeft+nTrimRight));
		
		temp=0;
	} 

	
	//fill struct for logging purposes
	for(i=0; i<numRows-(nTrimTop+nTrimBottom); i++)
		results->consecutive_pegged_rows[i]=temp_row[i];
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//initialize temp_consecutive
	for(i=0; i<numCols; i++)
		temp_consecutive[i]=-1;
	//init. temp_row and temp_col
	for(i=0; i<numCols-HEADER; i++)
		temp_col[i]=0;
	for(i=0; i<numRows; i++)
		temp_row[i]=0;
	temp=0;
	//scan through each row.
	for(i=nTrimTop; i<numRows-nTrimBottom; i++)
	{
		for(j=HEADER+nTrimLeft; j<numCols-nTrimRight; j++)
		{
			if( (results->avg[i][j] <= failRowFloored) )
			{
				temp++;
				temp_consecutive[j-HEADER-nTrimLeft]=temp;
			}
			else
			{
				temp=0;
				temp_consecutive[j-HEADER-nTrimLeft]=temp;
			}

			if(temp >= failLimit)//fail the test if the count is greater than our expected limits.
				results->m_bPass=0;
		}				
		temp_row[i-nTrimTop]=max_array(&temp_consecutive[0],numCols-HEADER-(nTrimLeft+nTrimRight));		
		temp=0;
	} 
	
	//fill struct for logging purposes
	for(i=0; i<numRows-(nTrimTop+nTrimBottom); i++)
		results->consecutive_floored_rows[i]=temp_row[i];
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//initialize temp_consecutive
	for(i=0; i<numCols; i++)
		temp_consecutive[i]=-1;
	//init. temp_row and temp_col
	for(i=0; i<numCols-HEADER; i++)
		temp_col[i]=0;
	for(i=0; i<numRows; i++)
		temp_row[i]=0;

	temp=0;
	//scan through each col. floored cols
	for(j=HEADER+nTrimLeft; j<numCols-nTrimRight; j++)
	{
		for(i=nTrimTop; i<numRows-nTrimBottom; i++)
		{
			if( (results->avg[i][j] <= failRowFloored) )
			{
				temp++;
				temp_consecutive[i-nTrimTop]=temp;
			}
			else
			{
				temp=0;
				temp_consecutive[i-nTrimTop]=temp;
			}

			if(temp >= failLimit)//fail the test if the count is greater than our expected limits.
				results->m_bPass=0;
		}	
		temp_col[j-HEADER-nTrimLeft]=max_array(&temp_consecutive[0],numRows-(nTrimTop+nTrimBottom));
		temp=0;
	} 
	
	//fill struct for logging purposes
	for(i=0; i<numCols-HEADER-(nTrimLeft+nTrimRight); i++)
		results->consecutive_floored_cols[i]=temp_col[i];
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//initialize temp_consecutive
	for(i=0; i<numCols; i++)
		temp_consecutive[i]=-1;
	//init. temp_row and temp_col
	for(i=0; i<numCols-HEADER; i++)
		temp_col[i]=0;
	for(i=0; i<numRows; i++)
		temp_row[i]=0;
	temp=0;
	//scan through each col.
	for(j=HEADER+nTrimLeft; j<numCols-nTrimRight; j++)
	{
		for(i=nTrimTop; i<numRows-nTrimBottom; i++)
		{
			if( (results->avg[i][j] >= failRowPegged) )
			{
				temp++;
				temp_consecutive[i-nTrimTop]=temp;
			}
			else
			{
				temp=0;
				temp_consecutive[i-nTrimTop]=temp;
			}

			if(temp >= failLimit)//fail the test if the count is greater than our expected limits.
				results->m_bPass=0;
		}		

		temp_col[j-HEADER-nTrimLeft]=max_array(&temp_consecutive[0],numRows-(nTrimTop+nTrimBottom));
		temp=0;
	}
	
	//fill struct for logging purposes
	for(i=0; i<numCols-HEADER-(nTrimLeft+nTrimRight); i++)
		results->consecutive_pegged_cols[i]=temp_col[i];

	results->pegged_ROW=max_array(results->consecutive_pegged_rows,numRows-(nTrimTop+nTrimBottom));
	results->floored_ROW=max_array(results->consecutive_floored_rows,numRows-(nTrimTop+nTrimBottom));
	results->floored_COL=max_array(results->consecutive_floored_cols,numCols-HEADER-(nTrimLeft+nTrimRight));
	results->pegged_COL=max_array(results->consecutive_pegged_cols,numCols-HEADER-(nTrimLeft+nTrimRight));
}

void get_span(int span[MAXFRAMES], int *min, int *max, int numFrames)
{
	int i;
	*max=-3200,*min=3200;
	for (i=0; i<numFrames; i++)
	{
		if (span[i]> *max)
		{
		    *max = span[i];
		}
		else if (span[i] < *min)
		{
		    *min = span[i];
		}
	}
}

void SYN_PeggedPixelsExecute(AcquireFPSResults* nofingerdata, PeggedPixelsInfo* info,PeggedPixelsResults* results,
							int nTrimLeft, int nTrimRight, int nTrimTop, int nTrimBottom,int numRows, int numCols)
{
	int i,j,k,temp=0;
	int numFrames,failRow,failCol,countRow,countCol,failLimit; //these are filled out by structure from .cpp file.
	int temp_sum=0;
	int temp_val=0;
	
	int temp_pegged[MAXCOL]={0};

	countCol=0;
	countRow=0;

	results->m_bPass=1;//set the device to pass to begin with.
	
	numFrames=info->numFrames;
	failCol=info->fail_col;
	failRow=info->fail_row;
	failLimit=info->fail_limits;//225
	//set up the nofinger avg matrix that is obtained by taking the arithmetic mean of 30 frames of nofinger data.
	for(i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<info->numFrames; k++)
			{
				temp_sum+=nofingerdata->m_arImagesWithoutStimulus[k].arr[i][j];			
			}
			
			temp_val=temp_sum/numFrames;
			results->avg[i][j]=temp_val;
			
			temp_sum=0;
			temp_val=0;
		}
	}
	//scan through each row.
	for(i=nTrimTop; i<numRows-nTrimBottom; i++)
	{
		for(j=HEADER+nTrimLeft; j<numCols-nTrimRight; j++)
		{
			if( results->avg[i][j] >= failLimit )
			{
				countRow+=1;
				temp+=1;
			}
			if(temp > failRow)//fail the test if the count is greater than our expected limits.
				results->m_bPass=0;
		}

		temp_pegged[i-nTrimTop]=countRow;
		countRow=0;
		results->pegged_pixel_rows[i-nTrimTop]=temp;
		temp=0;
	}

	//assign number of pegged 
	results->pegged_ROW=max_array(temp_pegged,numCols);

	//initialize temp_pegged;
	for(i=0; i<numCols; i++)
		temp_pegged[i]=0;
	//scan through each col.
	for(j=HEADER+nTrimLeft; j<numCols-nTrimRight; j++)
	{
		for(i=nTrimTop; i<numRows-nTrimBottom; i++)
		{
			if( results->avg[i][j] >= failLimit )
			{
				countCol+=1;
				temp+=1;
			}
			if(temp > failCol)//fail the test if the count is greater than our expected limits.
				results->m_bPass=0;
		}

		temp_pegged[j-nTrimLeft-HEADER]=countCol;
		countCol=0;
		results->pegged_pixel_cols[j-nTrimLeft-HEADER]=temp;
		temp=0;
	}
	//assign number of pegged
	results->pegged_COL=max_array(temp_pegged,numCols);
}

void SYN_FlooredPixelsExecute(AcquireFPSResults* nofingerdata, FlooredPixelsInfo* info,FlooredPixelsResults* results,
							  int nTrimLeft, int nTrimRight, int nTrimTop, int nTrimBottom,int numRows, int numCols)
{
	int i,j,k,temp=0;
	int numFrames,failRow,failCol,countRow,countCol,failLimit,nFailCount; //these are filled out by structure from .cpp file.
	int temp_sum=0;
	int temp_val=0;


	countCol = 0;
	countRow = 0;

	results->m_bPass = 1;//set the device to pass to begin with.
	
	numFrames=info->numFrames;
	failCol = info->fail_col;
	failRow = info->fail_row;
	failLimit = info->fail_limits;
	//set up the nofinger avg matrix that is obtained by taking the arithmetic mean of 30 frames of nofinger data.
	for(i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
				temp_sum += (nofingerdata->m_arImagesWithoutStimulus[k]).arr[i][j];
			
			temp_val=temp_sum/numFrames;
			results->avg[i][j]=temp_val;
			
			temp_sum=0;
			temp_val=0;
		}
	}

	//scan through each row.
	results->floored_ROW = 0;
	for(i=nTrimTop; i<numRows - nTrimBottom; i++)
	{
		nFailCount = 0;
		for(j=HEADER+nTrimLeft; j<numCols-(HEADER+nTrimLeft+nTrimRight); j++)
		{
			if( results->avg[i][j] < failLimit )
				nFailCount+=1;

			if(nFailCount > failRow)
				results->m_bPass=0;
		}		
		results->floored_pixel_rows[i-nTrimTop] = nFailCount;
		if (nFailCount > results->floored_ROW)
			results->floored_ROW = nFailCount;
	} 

	//scan through each col.
	results->floored_COL = 0;
	for(j=HEADER+nTrimLeft; j<numCols-nTrimRight; j++)
	{
		nFailCount = 0;
		for(i=nTrimTop; i<numRows-(nTrimTop+nTrimBottom); i++)
		{
			if( results->avg[i][j] < failLimit )
				nFailCount++;

			if(temp > failCol)//fail the test if the count is greater than our expected limits.
				results->m_bPass=0;
		}
		results->floored_pixel_cols[j-(HEADER+nTrimLeft)] = nFailCount;
		if (nFailCount > results->floored_COL)
			results->floored_COL = nFailCount;
	}
}

void SYN_DeltaPixelExecute(FPSMatrix* pNormalizedAvgs, DeltaPixelInfo* info, DeltaPixelResults* results,int nTrimLeft, int nTrimRight, int nTrimTop, int nTrimBot,int numRows, int numCols)
{
	int i, j;
	int nFailCount;

	int numFrames	= info->numFrames;
	int colFail		= info->fail_col;
	int rowFail		= info->fail_row;
	int failLimit	= info->fail_limits;
	results->m_bPass = 1;


	results->pegged_ROW = 0;
	for(i=0; i<numRows - (nTrimTop + nTrimBot); i++)
	{
		nFailCount = 0;
		for(j=0; j<numCols - (nTrimRight + nTrimLeft + HEADER); j++)
		{
			if( ((pNormalizedAvgs->arr[i][j] - 128) < rowFail) && ((pNormalizedAvgs->arr[i][j] - 128) > -rowFail) )
				nFailCount++;

			if(nFailCount >= info->fail_limits)
				results->m_bPass = 0;
		}
		results->delta_pixel_rows[i] = nFailCount;
		if (nFailCount > results->pegged_ROW)
			results->pegged_ROW = nFailCount;
	}

	results->pegged_COL = 0;
	for(j=0; j<numCols - (nTrimRight + nTrimLeft + HEADER); j++)
	{
		nFailCount = 0;
		for(i=0; i<numRows - (nTrimTop + nTrimBot); i++)
		{
			if( ((pNormalizedAvgs->arr[i][j] - 128) < colFail) && ((pNormalizedAvgs->arr[i][j] - 128) > -colFail))
				nFailCount++;

			if(nFailCount >= info->fail_limits)
				results->m_bPass = 0;
		}
		results->delta_pixel_cols[j] = nFailCount;
		if (nFailCount > results->pegged_COL)
			results->pegged_COL = nFailCount;
	}
}

int max_array(int a[], int num_elements)
{
   int i, max=-32000;
   for (i=0; i<num_elements; i++)
   {
	 if (a[i]>max)
	 {
	    max=a[i];
	 }
   }
   return(max);
}

void SYN_CurrentExecute(CurrentInfo* info, CurrentResults* results)
{
	int arGains[4] = {1,10,100,1000};
	unsigned long int	current_uA = 0;
	AdcBaseLineInfo		ablInfo = info->m_ablInfo;

	//actCurrent_uA = ((3/4096) * ADCCounts * (1000000/gain) (uint32_t)
	current_uA = (info->m_arImageAcqCurrentVals[ADC4] > (int)ablInfo.m_arAdcBaseLines[ADC4][info->m_nLowGain]) ?
		(info->m_arImageAcqCurrentVals[ADC4] - ablInfo.m_arAdcBaseLines[ADC4][info->m_nLowGain]): 0;
	results->m_nImageAcqAnaCurrent_uA = (3 * current_uA * (1000000/arGains[info->m_nLowGain])) / 4096;

	current_uA = (info->m_arImageAcqCurrentVals[ADC1] > (int)ablInfo.m_arAdcBaseLines[ADC1][info->m_nHighGain]) ?
		(info->m_arImageAcqCurrentVals[ADC1] - ablInfo.m_arAdcBaseLines[ADC1][info->m_nHighGain]): 0;
	results->m_nImageAcqDigCurrent_uA = (3 * current_uA * (1000000/arGains[info->m_nHighGain])) / 4096;

	results->m_nSleepAnaCurrent_uA = 0;
	results->m_nSleepDigCurrent_uA = 0;

	results->bPass = 1;
	if ((results->m_nImageAcqAnaCurrent_uA > (info->m_nImageAcqAnaMax_uA)) ||
		(results->m_nImageAcqAnaCurrent_uA < (info->m_nImageAcqAnaMin_uA)))
	{
		results->bPass = 0;
	}

	if ((results->m_nImageAcqDigCurrent_uA > (info->m_nImageAcqDigMax_uA)) ||
		(results->m_nImageAcqDigCurrent_uA < (info->m_nImageAcqDigMin_uA)))
	{
		results->bPass = 0;
	}

}

void SYN_ReadDutAdcStepExecute(ReadDutAdcInfo* pInfo, ReadDutAdcResults* pResults)
{

}

void SYN_OpensShortsExecute(OpensShortsInfo* pInfo, OpensShortsResults* pResults)
{
	pResults->m_bPass = 0;

	if (pResults->m_pResponse[4] & 0x00000001)
		pResults->m_bPass = 1;

	if (pResults->m_pResponse[4] & 0x00000002)
		pResults->m_bPass = 0;
}

void SYN_ProcessDRdyData(DRdyInfo* pInfo, DRdyResults* pResults)
{
	int i;

	pResults->m_bPass = 1;

	//If the DRdy pin was low when it was supposed to be high, it's a fail.
	for (i=0; i<NUM_DRDY_CHECKS; i++)
	{
		if (pResults->m_arHiStates[i] == 0)
			pResults->m_bPass = 0;
	}

	//If the DRdy pin was high when it was supposed to be low, it's a fail.
	for (i=0; i<NUM_DRDY_CHECKS; i++)
	{
		if (pResults->m_arLoStates[i] == 1)
			pResults->m_bPass = 0;
	}
}

void  SYN_ProcessBtnTestData(BtnTestInfo* pInfo, BtnTestResults* pResults)
{
    int i;
    uint8_t failState = 0;
    
    //If the Button pin is not match with ActiveState, it's a fail.
    for (i=0; i<NUM_BTN_CHECKS; i++)
    {
        if (pResults->m_arCurrStates[i] != pInfo->m_expectedState)
        {      
            pResults->m_bPass = 0;
            failState = pResults->m_arCurrStates[i];
            break;
        }
    }

    // Save current status
    pResults->m_btnCurrState = (pResults->m_bPass) ? pResults->m_arCurrStates[0] : failState;
}

void get_sensor_matrices(AcquireFPSResults* pFrames, SNRInfo *pInfo, SNRResults* pResult, float nofinger[MAXROW][MAXCOL], float finger[MAXROW][MAXCOL],int numRows, int numCols)
{
	int i,j,k;
	int temp, numFrames;
	numFrames = pInfo->numFrames;
	
	temp=0;
	//note that nofinger is printed in raw form.
	for (i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				temp+=pFrames->arr_nofinger[k].arr[i][j];
			}
			//nofinger avg:
			nofinger[i][j]= (float) temp/numFrames;
			temp=0;
		}
	}
	//finger avg:
	temp = 0;
	for(i=pInfo->numMinRows; i<pInfo->numMaxRows; i++)
	{
		for(j=pInfo->numMinCols; j<pInfo->numMaxCols - HEADER; j++)
		{
			finger[i - pInfo->numMinRows][j - pInfo->numMinCols] = pResult->NORM_AVGS[6].arr[i - pInfo->numMinRows][j - pInfo->numMinCols];
		}
	}


	/*
	int i,j;

	int START_ROW, END_ROW, START_COL, END_COL;
	
	START_ROW	= pInfo->numMinRows;
	END_ROW		= numRows - pInfo->numMaxRows;
	START_COL	= pInfo->numMinCols;
	END_COL		= numCols - pInfo->numMaxCols;

	for(i=START_ROW; i<numRows-END_ROW; i++)
	{
		for(j=HEADER+START_COL; j<numCols-END_COL; j++)
		{
			nofinger[i-START_ROW][j-HEADER-START_COL]=pResult->AVG_NOFINGER[i][j];
		}		
	}

	for(i=0; i<numRows - (START_ROW + END_ROW); i++)
	{
		for(j=0; j<numCols - (START_COL + END_COL); j++)
		{
			finger[i][j]=pResult->NORM_AVGS[REGIONS-1].arr[i][j];
		}		
	}
	*/
}

void GetSectorInfo(char* BS0, char* BS1, char* MS0, char* MS1)
{
	BS0[0]=0x01;
	BS0[1]=0X00;

	BS1[0]=0x01;
	BS1[1]=0x01;
	
	MS0[0]=0x00;
	MS0[1]=0x00;

	MS1[0]=0x00;
	MS1[1]=0x01;
}

int check_if_written_STAGE_1(char* arData, int numBytes)
{

	int ret,i;
	char c0=0xFF,
		 c1=0xFF,
		 c2=0xFF,
		 c3=0xFF;

	char cmp[4];

	ret=0;

	for (i=0; i<numBytes - 3; i++)
	{
		cmp[0]=arData[i];
		cmp[1]=arData[i+1];
		cmp[2]=arData[i+2];
		cmp[3]=arData[i+3];

		//loooking for the command LITTLE ENDIAN 0X 80 00 00 03 in the main sector 0 to see if LNA has been written before. 
		if( (cmp[3] == -128)  && (cmp[2] == 0) && (cmp[1] == 0) && (cmp[0] == 3) ) 
		{
			ret=1;
			break;
		}

	}


	return ret;

}
//for SNR and other TESTS 
int check_if_written_STAGE_2(char* arData, int numBytes)
{

	int ret,i;
	char c0=0xFF,
		c1=0xFF,
		c2=0xFF,
		c3=0xFF;
	char cmp[4];

	ret=0;

	for (i=0; i<numBytes - 3; i++)
	{
		cmp[0]=arData[i];
		cmp[1]=arData[i+1];
		cmp[2]=arData[i+2];
		cmp[3]=arData[i+3];

		//loooking for the command LITTLE ENDIAN 0X 80 00 00 07 in the main sector 0 to see if PGA has been written before. 
		if( (cmp[3] == -128)  && (cmp[2] == 0) && (cmp[1] == 0) && (cmp[0] == 7) ) 
		{
			ret=1;
			break;
		}

	}

	return ret;
}

void Create_SN(uint8_t* SN, uint32_t nDutdSerNum, int nSiteNum, int nTestLocationId)
{
	int year, day, hour, min, sec, YearSec;
	unsigned long long int TimeInSecs;
	unsigned long long serial;
	time_t t = time(NULL);
	struct tm tm;

	//Get elapsed time (secs) since 1-1-2014.
	localtime_s(&tm,&t);
	year	= (tm.tm_year + 1900) - 2014;
	day		= tm.tm_yday + 1;
	hour	= tm.tm_hour;
	min		= tm.tm_min;
	sec		= tm.tm_sec;
	YearSec	= year*365*24*3600;
	day		= day*24*3600;
	hour	= hour*3600;
	min		= min*60;
	TimeInSecs=YearSec+day+hour+min+sec;

	serial = 0x01;							//Denotes MT set the SN.
	serial = (serial <<  1) | 0x00;			//Denotes ATE set the SN.
	serial = (serial <<  4) | (nSiteNum & 0x0F);
	serial = (serial << 28) | (TimeInSecs & 0xFFFFFFF);
	serial = (serial <<  4) | (nTestLocationId & 0x0F);
	serial = (serial << 10) | (nDutdSerNum & 0x3FF);
					 
	SN[0] = (serial >> 40)	& 0x00000000000000FF;
	SN[1] = (serial >> 32)	& 0x00000000000000FF;
	SN[2] = (serial >> 24)	& 0x00000000000000FF;
	SN[3] = (serial >> 16)	& 0x00000000000000FF;
	SN[4] = (serial >> 8)	& 0x00000000000000FF;
	SN[5] = serial			& 0x00000000000000FF;	
}

void get_otp_write_arguments(char* outString, int* numBytes, char* pVal2Write, char* m_printfile, int isStage1, int ROW)
{
	int i;
	int LNA_START_INDEX = 1028;//print file specific
	int PGA_START_INDEX = 1136;//print file specific

	char flagLevel_1[4];
	char flagLevel_2[4];
	char offset[4];
	char extended_tag[4];
	char nbytes[2];
	char tag[1];
	char maxntags[1];
	char firstidx[1];

	char blob_array[56];//for calibration input to write
	


	flagLevel_1[0] = 0x00;
	flagLevel_1[1] = 0x00;
	flagLevel_1[2] = 0x00;
	flagLevel_1[3] = 0x00;
	
	flagLevel_2[0] = 0x11;
	flagLevel_2[1] = 0x00;
	flagLevel_2[2] = 0x00;
	flagLevel_2[3] = 0x00;	

	offset[0] = 0x00;
	offset[1] = 0x00;
	offset[2] = 0x00;
	offset[3] = 0x00;

	tag[0] = 0x0E;
	maxntags[0] = 0x00;
	firstidx[0] = 0x00;

	if(isStage1 == 2)//2 is to specify SNR
	{
		extended_tag[0]= 0x05;
		extended_tag[1]= 0x00;
		extended_tag[2]= 0x00;
		extended_tag[3]= 0x80;

		nbytes[0] = 0x04;
		nbytes[1] = 0x00;	
	}
		
	if(isStage1 == 1)
	{
		extended_tag[0]= 0x03;
		extended_tag[1]= 0x00;
		extended_tag[2]= 0x00;
		extended_tag[3]= 0x80;

		
		nbytes[0] = 0x3C;
		nbytes[1] = 0x00;	

	}
	if(isStage1 == 0)//PGA
	{
		extended_tag[0]= 0x07;
		extended_tag[1]= 0x00;
		extended_tag[2]= 0x00;
		extended_tag[3]= 0x80;

		
		nbytes[0] = 0x3C;
		nbytes[1] = 0x00;	
	}

	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	if(isStage1 == 2)//for SNR
	{
		outString[0] = flagLevel_1[0];
		outString[1] = flagLevel_1[1];
		outString[2] = flagLevel_1[2];
		outString[3] = flagLevel_1[3];

		outString[4] = flagLevel_2[0];
		outString[5] = flagLevel_2[1];
		outString[6] = flagLevel_2[2];
		outString[7] = flagLevel_2[3];

		outString[8] = offset[0];
		outString[9] = offset[1];
		outString[10] = offset[2];
		outString[11] = offset[3];

		outString[12] = extended_tag[0];
		outString[13] = extended_tag[1];
		outString[14] = extended_tag[2];
		outString[15] = extended_tag[3];

		outString[16] = nbytes[0];
		outString[17] = nbytes[1];

		outString[18] = tag[0];

		outString[19] = maxntags[0];

		outString[20] = firstidx[0];


		//DUMMY BYTES
		outString[21] = 0;
		outString[22] = 0;
		outString[23] = 0;
		//DUMMY BYTES

		for (i=0; i<4; i++)
		{
			blob_array[i]=pVal2Write[i];
		}

		for(i=24; i<28; i++)//for(i=21; i<25; i++)//
		{
			outString[i]=blob_array[i-24];//outString[i]=blob_array[i-21];//
		}

		*numBytes=24 + 4;//*numBytes=21 + 4;//
	}


	if(isStage1 == 1)//lna
	{	//21 is the size of Arguments from Jorge
		outString[0] = flagLevel_1[0];
		outString[1] = flagLevel_1[1];
		outString[2] = flagLevel_1[2];
		outString[3] = flagLevel_1[3];

		outString[4] = flagLevel_2[0];
		outString[5] = flagLevel_2[1];
		outString[6] = flagLevel_2[2];
		outString[7] = flagLevel_2[3];

		outString[8] = offset[0];
		outString[9] = offset[1];
		outString[10] = offset[2];
		outString[11] = offset[3];

		outString[12] = extended_tag[0];
		outString[13] = extended_tag[1];
		outString[14] = extended_tag[2];
		outString[15] = extended_tag[3];

		outString[16] = nbytes[0];
		outString[17] = nbytes[1];

		outString[18] = tag[0];

		outString[19] = maxntags[0];

		outString[20] = firstidx[0];

		//DUMMY BYTES
		outString[21] = 0;
		outString[22] = 0;
		outString[23] = 0;
		//DUMMY BYTES

		//
		outString[24] = 0x7;
		outString[25] = 0x0;
		outString[26] = 0x0;
		outString[27] = 0x0;

		
		for (i=0; i<ROW; i++)
		{
			blob_array[i]=pVal2Write[i];//updated print file fed in
		}

		for(i=28; i<84; i++)//84=28+56,56=row
		{
			outString[i]=blob_array[i-28];
		}


		*numBytes= 28 + ROW;
	}
	if(isStage1 == 0)//pga
	{
		outString[0] = flagLevel_1[0];
		outString[1] = flagLevel_1[1];
		outString[2] = flagLevel_1[2];
		outString[3] = flagLevel_1[3];

		outString[4] = flagLevel_2[0];
		outString[5] = flagLevel_2[1];
		outString[6] = flagLevel_2[2];
		outString[7] = flagLevel_2[3];

		outString[8] = offset[0];
		outString[9] = offset[1];
		outString[10] = offset[2];
		outString[11] = offset[3];

		outString[12] = extended_tag[0];
		outString[13] = extended_tag[1];
		outString[14] = extended_tag[2];
		outString[15] = extended_tag[3];

		outString[16] = nbytes[0];
		outString[17] = nbytes[1];

		outString[18] = tag[0];



		outString[19] = maxntags[0];

		outString[20] = firstidx[0];

		//DUMMY BYTES
		outString[21] = 0;
		outString[22] = 0;
		outString[23] = 0;
		//DUMMY BYTES

		//
		outString[24] = 0x7;
		outString[25] = 0x0;
		outString[26] = 0x0;
		outString[27] = 0x0;

		for (i=0; i<ROW; i++)
		{
			blob_array[i]=pVal2Write[i];//updated print file fed in
		}

		for(i=28; i<84; i++)//84=28+56,56=row
		{
			outString[i]=blob_array[i-28];
		}


		*numBytes= 28 + 56;	
	}
}

void get_invalidate_arguments(char* outString, int* numBytes, int isStage1 )
{
	char extended_tag[4];
	extended_tag[1]= 0x00;
	extended_tag[2]= 0x00;
	extended_tag[3]= 0x80;

	if (isStage1 == 10)	//10 is Part Numbers - Config and MT
	{
		extended_tag[0] = 0x0F;
	}
	else if (isStage1 == 9)	// 9 is product ID
	{
		extended_tag[0]= 0x02;
		extended_tag[1]= 0x00;
		extended_tag[2]= 0x00;
		extended_tag[3]= 0x00;
	}
	else if (isStage1 == 8)	// 8 is scm wof
	{
		extended_tag[0] = 0x0D;
	}
	else if(isStage1 == 7)			//7 is PGA one offset per pixel.
	{
		extended_tag[0]= 0x0C;
	}
	else if(isStage1 == 5)		//5 is to specify DUT temperature
	{
		extended_tag[0]= 0x0A;
	}
	else if(isStage1 == 6)		//4 is to specify WOF Top values.
	{
		extended_tag[0]= 0x0B;
	}
	else if(isStage1 == 4)		//4 is to specify WOF Bot values.
	{
		extended_tag[0]= 0x09;
	}
	else if(isStage1 == 3)		//3 is to specify Flex ID
	{
		extended_tag[0]= 0x08;
	}
	else if(isStage1 == 2)		//2 is to specify SNR
	{
		extended_tag[0]= 0x05;
	}
	else if(isStage1 == 1)		//1 is to specify LNA
	{
		extended_tag[0]= 0x03;
	}
	else if(isStage1 == 0)	//0 is PGA one offset per row.
	{
		extended_tag[0]= 0x07;
	}
	
	outString[0] = 0;
	outString[1] = 0;
	outString[2] = 0;
	outString[3] = 0;

	outString[4] = 0x21;
	outString[5] = 0;
	outString[6] = 0;
	outString[7] = 0;
	
	outString[8] = 0;
	outString[9] = 0;
	outString[10] =0;
	outString[11] =0;
	
	outString[12] = extended_tag[0];
	outString[13] = extended_tag[1];
	outString[14] = extended_tag[2];
	outString[15] = extended_tag[3];
	
	outString[16] = 0;
	outString[17] = 0;

	outString[18] = 0x0E;
	
	outString[19] = 0x00;
	
	outString[20] = 0x00;
	
	//DUMMY BYTES
	outString[21] = 0;
	outString[22] = 0;
	outString[23] = 0;
	//DUMMY BYTES
	
	*numBytes=24;
}

int check_if_written_BS1(unsigned char* arData, int numBytes)
{
	if ( 
		( (arData[32] == 0x00) && (arData[33] == 0x00) && (arData[34] == 0x00) && (arData[35] == 0x00) ) &&
		( (arData[40] == 0x00) && (arData[41] == 0x00) && (arData[42] == 0x00) && (arData[43] == 0x00) ) &&
		( ( (arData[48] == 0x00) && (arData[49] == 0x00) ) && ( (arData[56] == 0x00) && (arData[57] == 0x00) ) )
	   )
		return 0;//no its not written yet
	else
		return 1;//yes, it's been written already	
}
//works with FPWrite
void BS1_arguments_to_write(char* Argument_to_write, int* numBytes,char* serial_number, char* bs1)
{
	char bootSector[64];
	int i;

	for(i=0; i<64; i+=4)
	{
		if( ( i == 32 ) || ( i == 40 ) || ( i == 48 ) || ( i == 56 ) )
		{
			if( i == 32 )
			{
				bootSector[i]=serial_number[0];	
				bootSector[i+1]=serial_number[1];
				bootSector[i+2]=serial_number[2];
				bootSector[i+3]=serial_number[3];
			}
			else if ( i == 40 )
			{
				bootSector[i]=serial_number[0];	
				bootSector[i+1]=serial_number[1];
				bootSector[i+2]=serial_number[2];
				bootSector[i+3]=serial_number[3];
			}
			else if ( i == 48 )
			{
				bootSector[i]=serial_number[4];	
				bootSector[i+1]=serial_number[5];
				bootSector[i+2]=0;
				bootSector[i+3]=0;
			}
			else 
			{
				bootSector[i]=serial_number[4];	
				bootSector[i+1]=serial_number[5];
				bootSector[i+2]=0;
				bootSector[i+3]=0;
			}
		}
		else
		{
			bootSector[i]=bs1[i];
			bootSector[i+1]=bs1[i+1];
			bootSector[i+2]=bs1[i+2];
			bootSector[i+3]=bs1[i+3];
		}
	}

	Argument_to_write[0]=1;
	Argument_to_write[1]=1;
	Argument_to_write[2]=10;
	Argument_to_write[3]=0;

	for(i=4; i<68; i++)
	{
		Argument_to_write[i]=bootSector[i-4];
	}
	
	*numBytes = 68;
	
}

int check_if_written_BS0(unsigned char* arData, int numBytes)
{	
	int bHasBeenWritten = 1;

	if ( (arData[2] & 0xFE) != 0xFE  )
		bHasBeenWritten = 0;
	if ( (arData[3] & 0xFF) != 0xFF  )
		bHasBeenWritten = 0;
	if ( (arData[49] & 0x50) != 0x50  )
		bHasBeenWritten = 0;
	if ( (arData[51] & 0x40) != 0x40  )
		bHasBeenWritten = 0;
		
	return bHasBeenWritten;
}
void BS0_SecurityArgumentsToWriteProd_Metallica(uint8_t* Argument_to_write, uint8_t* bs0)
{
	int i;

	Argument_to_write[0]=0;// Boot Sector 0
	Argument_to_write[1]=1;
	Argument_to_write[2]=10;
	Argument_to_write[3]=0;
	
	bs0[0]  |= 0x20;	
	bs0[8]  |= 0x20;	

	bs0[2]  |= 0xFE;
	bs0[10] |= 0xFE;

	bs0[3]  |= 0xFF;	
	bs0[11] |= 0xFF;



	bs0[49] |= 0x50;
	bs0[51] |= 0x40;
	bs0[57] |= 0x50;
	bs0[59] |= 0x40;
	
	for(i = 0; i < BS0_SIZE; i++)
		Argument_to_write[i + 4] = bs0[i];
}

void BS0_SecurityArgumentsToWriteEngr_Metallica(uint8_t* arData, uint8_t* bs0)
{

}

void BS0_arguments_to_write(char* Argument_to_write, char* bs0)
{
	int i;

	Argument_to_write[0]=0;
	Argument_to_write[1]=1;
	Argument_to_write[2]=10;
	Argument_to_write[3]=0;
	
	bs0[2]  |= 0xFE;
	bs0[3]  |= 0xFF;
	bs0[10] |= 0xFE;
	bs0[11] |= 0xFF;
	bs0[49] |= 0x50;
	bs0[51] |= 0x40;
	bs0[57] |= 0x50;
	bs0[59] |= 0x40;
	
	for(i = 0; i < BS0_SIZE; i++)
		Argument_to_write[i + 4] = bs0[i];
}

int get_blob_array(	char* pStitcher, char* pPrintfile,int *pNumPrintBytes,
					char* pPatchfile, int *pNumPatchBytes,
					char* pPrintpatch, int* pNumPrintpatchBytes,
					char* pFirmwarePatch,int*pNumFirmwarePatchBytes, 
					unsigned char* SysConfigPrintFile,int SysConfigPrintFileSize,
					unsigned char* SysConfigPatchFile,int SysConfigPatchFileSize,
					unsigned char* SysConfigGetPrintPatchFile,int SysConfigGetPrintPatchFileSize,
					unsigned char* SysConfigOpensShortsFile,int SysConfigOpensShortsSize )
{
	int i;
	int bSuccess = 1;

	*pNumPrintBytes=SysConfigPrintFileSize;
	*pNumPatchBytes=SysConfigPatchFileSize;
	*pNumPrintpatchBytes = SysConfigGetPrintPatchFileSize;
	*pNumFirmwarePatchBytes = SysConfigOpensShortsSize;

	for (i=0; i<SysConfigPrintFileSize; i++)
	{
		pStitcher[i]=SysConfigPrintFile[i];
		pPrintfile[i]=SysConfigPrintFile[i];
	}
	for (i=0; i<SysConfigPatchFileSize; i++)
	{
		pPatchfile[i]=SysConfigPatchFile[i];
	}
	for (i=0; i<SysConfigGetPrintPatchFileSize; i++)
	{
		pPrintpatch[i]=SysConfigGetPrintPatchFile[i];
	}
	for (i=0; i<SysConfigOpensShortsSize; i++)
	{
		pFirmwarePatch[i]=SysConfigOpensShortsFile[i];
	}

	return bSuccess;
}

void SYN_SharpnessTest(int height, int width, float pImg[MAXROW][MAXCOL], float* sharpnessMeasure, SharpnessInfo* pInfo, SharpnessResults* pResults)
{
	int i, j;
	int M = height;
	int N = width;
	int C0=N/3;
	int C1=C0+C0;
	int C2=N;
	int min,max;
	float measure;

	float zones[3] = {0};
	float tempImg[MAXROW][MAXCOL];

	pResults->bPass = 1;

	//overall
	get_sharpness(M,N,pImg,&measure,pResults);
	pResults->SHARPNESS[3]=measure;
	//zone 1
	for(i=0; i<M; i++)
	{
		for(j=0; j<C0; j++)
		{
			tempImg[i][j]=pImg[i][j];
		}
	}
	get_sharpness(M,(C0+1),tempImg,&measure,pResults);
	pResults->SHARPNESS[0]=measure;	
	//zone 2
	for(i=0; i<M; i++)
	{
		for(j=C0; j<C1; j++)
		{
			tempImg[i][j-C0]=pImg[i][j];
		}
	}
	get_sharpness(M,(C1-C0+1),tempImg,&measure,pResults);
	pResults->SHARPNESS[1]=measure;
	//zone 3
	for(i=0; i<M; i++)
	{
		for(j=C1; j<C2; j++)
		{
			tempImg[i][j-C1]=pImg[i][j];
		}
	}
	get_sharpness(M,(C2-C1+1),tempImg,&measure,pResults);
	pResults->SHARPNESS[2]=measure;
	
	zones[0]=pResults->SHARPNESS[0];
	zones[1]=pResults->SHARPNESS[1];
	zones[2]=pResults->SHARPNESS[2];
	min=find_min(zones,3);
	max=find_max(zones,3);
	pResults->percent = (float)abs( (int) (( (zones[max] - zones[min]) / pResults->SHARPNESS[3]) * 100));

	if( pResults->percent > pInfo->limit)
		pResults->bPass = 0;	
}
void get_sharpness(int height, int width, float pImg[MAXROW][MAXCOL], float* sharpnessMeasure, SharpnessResults* pSharpness)
{
	int i,j;
	int m, n;
	double x,y;
	int tempSum=0;
	int sum;
	int sharpImage[MAXROW][MAXCOL];

	int tempImg[3][3];
	int tempGx[3][3];
	int tempGy[3][3];
	
	int product[MAXROW][MAXCOL];
	
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			//==================
			for(m=0; m<3; m++)
			{
				for(n=0; n<3; n++)
				{
					tempImg[m][n] = (int) pImg[i+m][j+n];
				}
			}
			//==================
			for(m=0; m<3; m++)
			{
				for(n=0; n<3; n++)
				{
					tempGx[m][n]=tempImg[m][n] * m_kernelSharpnessGx[m][n];
				}
			}
			for(m=0; m<3; m++)
			{
				for(n=0; n<3; n++)
				{
					tempSum += tempGx[m][n];
				}
			}
			x=tempSum;
			tempSum=0;
			for(m=0; m<3; m++)
			{
				for(n=0; n<3; n++)
				{
					tempGy[m][n]=tempImg[m][n] * m_kernelSharpnessGy[m][n];
				}
			}
			for(m=0; m<3; m++)
			{
				for(n=0; n<3; n++)
				{
					tempSum+=tempGy[m][n];
				}
			}
			y=tempSum;
			tempSum=0;
			sum = (int) ((x*x) + (y*y));
			sum = (int) sqrt((long double)sum);
			sharpImage[i][j] = sum;
			sum=0;
		}
	}
	
	gradient(height,width,sharpImage,pSharpness->Gx,pSharpness->Gy,pSharpness);
	for(i=0; i<height-2; i++)
	{
		for(j=0; j<width-2; j++)
		{
			product[i][j] = (pSharpness->Gx[i][j]*pSharpness->Gx[i][j]) + (pSharpness->Gy[i][j]*pSharpness->Gy[i][j]);			
			product[i][j] = (int) sqrt( (long double) product[i][j]);
		}
	}
	sum=0;
	for(i=0; i<height-3; i++)
	{
		for(j=0; j<width-4; j++)
		{
			sum+=product[i][j];
		}
	}
	//overall zone
	*sharpnessMeasure = (float) (sum/((height-2)*(width-4)));	
}
//differential along x and y - > Gx and Gy
//gradient() will populate Gx and Gy matrices.
void gradient(int numRowsImg, int numColsImg, int pImg[MAXROW][MAXCOL], int Gx[MAXROW][MAXCOL],int Gy[MAXROW][MAXCOL], SharpnessResults* pSharpness)
{
	int n,p;
	int k,ndim,i;
	int row, col;
	ndim=2;

	for(k=0; k<ndim; k++)
	{
		//56x144
		if(k == 0)
		{
			n=numRowsImg;
			p=numColsImg;
			for(i=0; i<numRowsImg; i++)
				pSharpness->h[i]=i;
			//Take forward differences on left and right edges
			for(i=0; i<numColsImg; i++)
				pSharpness->g[0][i] = pImg[1][i] - pImg[0][i];
			for(i=0; i<numColsImg; i++)
				pSharpness->g[n][i] = pImg[n][i] - pImg[n-1][i];

			for(row=2; row<n; row++)
			{
				for(col=0; col<p; col++)
				{
					pSharpness->g[row-1][col]=(pImg[row][col] - pImg[row-2][col])/2;
				}
			}
			//Obtain Gy
			for(row=0; row<numRowsImg-2; row++)
			{
				for(col=0; col<numColsImg; col++)
				{
					Gy[row][col]=pSharpness->g[row][col];
				}
			}					

			//init. g[][] to zero
			for(row=0; row<MAXROW; row++)
			{
				for(col=0; col<MAXCOL; col++)
				{
					pSharpness->g[row][col]=0;
				}
			}
		}		
		//144x56
		else
		{
			n=numColsImg;
			p=numRowsImg;
			//transpose image 
			for(row=0; row<numColsImg; row++)
			{
				for(col=0; col<numRowsImg; col++)
				{
					pSharpness->pImg_t[row][col] = pImg[col][row];
				}
			}			
			for(i=0; i<numColsImg; i++)
				pSharpness->h[i]=i;
			//Take forward differences on left and right edges
			for(i=0; i<numColsImg; i++)
				pSharpness->g[0][i] = pSharpness->pImg_t[1][i] - pSharpness->pImg_t[0][i];
			for(i=0; i<numColsImg; i++)
				pSharpness->g[n][i] = pSharpness->pImg_t[n][i] - pSharpness->pImg_t[n-1][i];

			for(row=2; row<n; row++)
			{
				for(col=0; col<p; col++)
				{
					pSharpness->g[row-1][col]=(pSharpness->pImg_t[row][col] - pSharpness->pImg_t[row-2][col])/2;
				}
			}
			//transpose image and Obtain Gx			
			for(row=0; row<p; row++)
			{
				for(col=0; col<n; col++)
				{
					Gx[row][col] = pSharpness->g[col][row];
				}
			}						
		}
	}
	
}

//if order = 1 => transpose
//	else	   => no transpose	
void permute(int numRowsImg, int numColsImg, int pImg[MAXROW][MAXCOL], int* numRowsArranged, int* numColsArranged, int pArranged[MAXROW][MAXCOL], int numOrder)
{
	int row,col;	
	if(numOrder == 1)
	{
		//transpose
		for(row=0; row<numColsImg; row++)
		{
			for(col=0; col<numRowsImg; col++)
			{
				pArranged[row][col]=pImg[col][row];
			}
		}
		*numRowsArranged = numColsImg;
		*numColsArranged = numRowsImg;
	}
	else
	{
		//do not transpose
		for(row=0; row<numRowsImg; numRowsImg++)
		{
			for(col=0; col<numColsImg; numColsImg++)
			{
				pArranged[row][col]=pImg[row][col];
			}
		}	
		*numRowsArranged = numRowsImg;
		*numColsArranged = numColsImg;
	}
}

float std_dev(float pData[], int size)
{
	float mean=0.0, sum_deviation=0.0;
	int i;
	for(i=0; i<size;++i)
	{
		mean+=pData[i];
	}
	mean=mean/size;
	for(i=0; i<size;++i)
		sum_deviation+=(pData[i]-mean)*(pData[i]-mean);


	return (float)sqrt(sum_deviation/size);           
}

int find_min(float* pArray, int size)
{
	int min, i, loc = 0;
	min = (int)pArray[0];
	for(i=1; i<size; i++)
	{
		if(pArray[i] < min)
		{
			min = (int)pArray[i];
			loc = i;
		}
	}

	return loc;
}
//returns location of max
int find_max(float* pArray, int size)
{
	int max, i, loc = 0;
	max = (int)pArray[0];
	for(i=1; i<size; i++)
	{
		if(pArray[i] > max)
		{
			max = (int)pArray[i];
			loc = i;
		}
	}

	return loc;
}

void SYN_ImperfectionsTestExecute(SNRResults* fingerdata, ImperfectionsTestInfo* info, ImperfectionsTestResults* results,int nTrimLeft, int nTrimRight, int nTrimTop, int nTrimBottom,int numRows, int numCols)
{	
	int numFrames,i,j,temp=0;
	int failRowPegged,failRowFloored,countRow,countCol,failLimit; //these are filled out by structure from .cpp file.
	int temp_sum=0;
	int temp_val=0;
	int temp_consecutive[MAXCOL];
	int temp_col[MAXCOL];
	int temp_row[MAXROW];
	numFrames=info->numFrames;
	
	//initialize temp_consecutive
	for(i=0; i<numCols; i++)
		temp_consecutive[i]=-1;
	//init. temp_row and temp_col
	for(i=0; i<numCols; i++)
		temp_col[i]=0;
	for(i=0; i<numRows; i++)
		temp_row[i]=0;

	countRow=0;
	countCol=0;
	results->m_bPass=1;//set the device to pass to begin with.
	
	//eliminate=info->m_cutEdge;
	
	failRowPegged=info->m_peggedThreshold;
	failRowFloored=info->m_flooredThreshold;
	failLimit=info->m_maxAdjacentPixelsAllowed;

	//initialize the structs.
	for(i=0; i<numCols-HEADER-(nTrimLeft+nTrimRight); i++)
	{
		results->consecutive_floored_cols[i]=0;
		results->consecutive_pegged_cols[i]=0;
	}
	for(i=0; i<numRows-(nTrimTop+nTrimBottom); i++)
	{
		results->consecutive_floored_rows[i]=0;
		results->consecutive_pegged_rows[i]=0;
	}
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	temp=0;
	//scan through each row.
	for(i=0; i<numRows-nTrimBottom-nTrimTop; i++)
	{
		for(j=0; j<numCols-nTrimRight-nTrimLeft-HEADER; j++)
		{
			if( (fingerdata->NORM_AVGS[6].arr[i][j] >= failRowFloored) && (fingerdata->NORM_AVGS[6].arr[i][j] <= failRowPegged) )
			{
				temp++;			
				temp_consecutive[j]=temp;
			}
			else
			{
				temp=0;
				temp_consecutive[j]=temp;
			}

			if(temp > failLimit)//fail the test if the count is greater than our expected limits.
				results->m_bPass=0;
		}				
		temp_row[i]=max_array(&temp_consecutive[0],numCols-HEADER-(nTrimLeft+nTrimRight));
		
		temp=0;
	} 

	
	//fill struct for logging purposes
	for(i=0; i<numRows-(nTrimTop+nTrimBottom); i++)
		results->consecutive_pegged_rows[i]=temp_row[i];
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//initialize temp_consecutive
	for(i=0; i<numCols; i++)
		temp_consecutive[i]=-1;
	//init. temp_row and temp_col
	for(i=0; i<numCols-HEADER; i++)
		temp_col[i]=0;
	for(i=0; i<numRows; i++)
		temp_row[i]=0;
	temp=0;
	//scan through each col.
	for(j=0; j<numCols-nTrimRight-nTrimLeft-HEADER; j++)
	{
		for(i=0; i<numRows-nTrimBottom-nTrimTop; i++)
		{
			if( (fingerdata->NORM_AVGS[6].arr[i][j] >= failRowFloored) && (fingerdata->NORM_AVGS[6].arr[i][j] <= failRowPegged) )
			{
				temp++;
				temp_consecutive[i]=temp;
			}
			else
			{
				temp=0;
				temp_consecutive[i]=temp;
			}

			if(temp > failLimit)//fail the test if the count is greater than our expected limits.
				results->m_bPass=0;
		}		

		temp_col[j]=max_array(&temp_consecutive[0],numRows-(nTrimTop+nTrimBottom));
		temp=0;
	}
	
	//fill struct for logging purposes
	for(i=0; i<numCols-HEADER-(nTrimLeft+nTrimRight); i++)
		results->consecutive_pegged_cols[i]=temp_col[i];

	results->pegged_ROW=max_array(results->consecutive_pegged_rows,numRows-(nTrimTop+nTrimBottom));	
	results->pegged_COL=max_array(results->consecutive_pegged_cols,numCols-HEADER-(nTrimLeft+nTrimRight));
}


void SYN_RAMTestExecute(RAMTestInfo* pInfo, RAMTestResults* pResults)
{
	pResults->bPass = 1;
	if ( (pResults->m_pResponseCacheDataRam[2] == 0) && (pResults->m_pResponseCacheDataRam[3] == 0) &&
		 (pResults->m_pResponseCacheDataRam[4] == 0) && (pResults->m_pResponseCacheDataRam[5] == 0) )
		pResults->bPassCacheDataRam = 1;
	else
		pResults->bPassCacheDataRam = 0;

	if ( (pResults->m_pResponseCacheTagRam[2] == 0) && (pResults->m_pResponseCacheTagRam[3] == 0) &&
		 (pResults->m_pResponseCacheTagRam[4] == 0) && (pResults->m_pResponseCacheTagRam[5] == 0) )
		pResults->bPassCacheTagRam = 1;
	else
		pResults->bPassCacheTagRam = 0;

	if ( (pResults->m_pResponseCachInstDataRam[2] == 0) && (pResults->m_pResponseCachInstDataRam[3] == 0) &&
		 (pResults->m_pResponseCachInstDataRam[4] == 0) && (pResults->m_pResponseCachInstDataRam[5] == 0) )
		pResults->bPassCachInstDataRam = 1;
	else
		pResults->bPassCachInstDataRam = 0;

	if ( (pResults->m_pResponseCachInstTagRam[2] == 0) && (pResults->m_pResponseCachInstTagRam[3] == 0) &&
		 (pResults->m_pResponseCachInstTagRam[4] == 0) && (pResults->m_pResponseCachInstTagRam[5] == 0) )
		pResults->bPassCachInstTagRam = 1;
	else
		pResults->bPassCachInstTagRam = 0;

	if ( (pResults->m_pResponseScmAndMainRam[2] == 0) && (pResults->m_pResponseScmAndMainRam[3] == 0) &&
		 (pResults->m_pResponseScmAndMainRam[4] == 0) && (pResults->m_pResponseScmAndMainRam[5] == 0) )
		pResults->bPassScmAndMainRam = 1;
	else
		pResults->bPassScmAndMainRam = 0;

	if ( !(pResults->bPassCacheDataRam) || !(pResults->bPassCacheTagRam) || !(pResults->bPassCachInstDataRam) ||
		 !(pResults->bPassCachInstTagRam) || !(pResults->bPassScmAndMainRam) )
		pResults->bPass = 0;
}

void SYN_RXStandardDevExecute(AcquireFPSResults* fingerStack, RxStandardDevInfo* pInfo, RxStandardDevResults* pResults, int nTrimLeft, int nTrimRight, int nTrimTop, int nTrimBottom,int numRows, int numCols)
{
	int row, temp_sum, i, j, k;	
	float mean, temp_val;
	int numFrames;
	float fingerdata[MAXROW][MAXCOL] = {0};
	
	numFrames = pInfo->numFrames;
	pResults->m_bPass = 1;
	pResults->max_percent = 0;


	//set up the finger avg matrix that is obtained by taking the arithmetic mean of 30 frames of finger data.
	for(i=0; i<numRows; i++)
	{
		for(j=0; j<numCols; j++)
		{
			for(k=0; k<numFrames; k++)
			{
				temp_sum+=fingerStack->arr_finger[k].arr[i][j];				
			}
			
			temp_val = (float)(temp_sum/numFrames);
			fingerdata[i][j]=temp_val;
			
			temp_sum=0;
			temp_val=0;
		}
	}

	for(row=nTrimTop; row<numRows-nTrimBottom; row++)
	{
		pResults->Rx_STDs[row] = std_dev(&fingerdata[row][nTrimLeft+HEADER],numCols-HEADER-nTrimRight-nTrimLeft);		
	}

	mean=find_mean(&pResults->Rx_STDs[nTrimTop],numRows-(nTrimTop+nTrimBottom));

	for(row=nTrimTop; row<numRows-nTrimBottom; row++)
	{
		pResults->percent[row-nTrimTop] = (float)abs((int) ( ((pResults->Rx_STDs[row] - mean)/(mean)) * 100) );
		if(pResults->percent[row-nTrimTop] > pInfo->limit) 
			pResults->m_bPass=0;

		// SS ask to present max percent data in the log
		pResults->max_percent = (pResults->percent[row-nTrimTop] > pResults->max_percent)? pResults->percent[row-nTrimTop]:pResults->max_percent; 
	}

/* old code. It will be removed after code reveiw.
	mean=find_mean(&pResults->Rx_STDs[0],numRows-(nTrimTop+nTrimBottom));

	for(row=0; row<numRows-(nTrimTop+nTrimBottom); row++)
	{
		pResults->percent[row] = abs(((pResults->Rx_STDs[row] - mean)/(mean)) * 100);
		if(pResults->percent[row] > pInfo->limit) 
			pResults->m_bPass=0;

		// SS ask to present max percent data in the log
		pResults->max_percent = (pResults->percent[row] > pResults->max_percent)? pResults->percent[row]:pResults->max_percent; 
	}
*/
}

//CDM kernel
static float m_matrix[144][144] = {{0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,-0.062500,-0.062500,0.062500,-0.062500},
{0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,0.062500,0.062500,0.062500,-0.062500,-0.062500,-0.062500,-0.062500,0.062500}};
