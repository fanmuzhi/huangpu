#pragma once

#include <stdint.h>

#define SYN_FALSE							0
#define SYN_TRUE							!SYN_FALSE

#define MAJOR_VER							1
#define MINOR_VER							12
#define DEVOLOPMENT_VER						0

#define MAXROW								200
#define MAXCOL								200
#define MAXFRAMES							50
#define MAX_PRINTFILE_SIZE					20000
#define MAX_OPENS_SHORTS_RES				5000
#define MAX_AFE_TEST_RES					5000
#define MAX_PIXEL_PATCH_RES_BYTES			3000
#define MAX_WOVAR_RES_BYTES					3000
#define MAX_LED_TEST_MSG_LEN				500

#define NUM_ADC_BASE_READINGS				4
#define NUM_CURRENT_DRAW_READINGS			5

//CALIBRATION DEFINITIONS	
#define STAGE_1_CALIBRATION_RANGE			11
#define STAGE_1_CALIBRATION_START_OFFSET	123

//following are defined for SNR Test	
#define HISTOGRAM							258
#define REGIONS								7
#define HEADER								8

typedef unsigned char UINT8; // UINT8 is created to handle MPC04 data

#define	DUT_SER_NUM_SIZE	6
#define	VERSION_SIZE		36
#define	BS0_SIZE			64
#define	BS1_SIZE			64
#define	MS0_SIZE			2 * 1024 
#define	MS1_SIZE			2 * 1024
#define	MAIN_SEC			0
#define	BOOT_SEC			1

#define	TAG_CAL					0X0E
#define	EXT_TAG_PRODUCT_ID		0x00000002L
#define	EXT_TAG_LNA				0x80000003L
#define	EXT_TAG_SNR				0x80000005L
#define	EXT_TAG_PGA_OOPR		0x80000007L		//PGA one offset per row.
#define	EXT_TAG_FlexId			0x80000008L
#define	EXT_TAG_WOF_BOT			0x80000009L
#define	EXT_TAG_DutTempAdc		0x8000000AL
#define	EXT_TAG_WOF_TOP			0x8000000BL
#define	EXT_TAG_PGA_OOPP		0x8000000CL		//PGA one offset per pixel.
#define	EXT_TAG_SCM_WOF_BOT		0x8000000DL
#define	EXT_TAG_SCM_WOF_TOP		0x8000000EL
#define EXT_TAG_PART_NUMBERS	0x8000000FL
#define EXT_TAG_LNA_PGA_GAINS	0x80000012L
#define	NUM_EXT_TAGS			13

#define	PGA_OOPP_OTP_ROW1					4
#define	PGA_OOPP_OTP_ROW2					18
#define	PGA_OOPP_OTP_ROW3					28
#define	PGA_OOPP_OTP_ROW4					38
#define	PGA_OOPP_OTP_ROW5					51
#define	NUM_PGA_OOPP_OTP_ROWS				5

#define	ADC1				0
#define	ADC2				1
#define	ADC4				2
#define	ADC5				3
#define	NUM_CURRENT_VALUES	4

#define	KNUMGAINS			4

#define NUM_REGISTERS_VIPER_TRIM_OSC	  7
#define NUM_REGISTERS_METAL_TRIM_OSC	  8
#define NUM_REGISTERS_METAL_TRIM_SLOW_OSC 6
#define SLOW_OSC_CALIBRATION_VALUE		  502
#define SLOW_OSC_FINE_TUNING_RANGE		  32
#define SLOW_OSC_LOW_LIMIT				  475
#define SLOW_OSC_HIGH_LIMIT				  501

#define MPC_SELF_TEST_BUFFER 32

#define MAX_PART_NUMBER_LENGTH 8


typedef struct
{
	int		_bExecuted;

	uint8_t _BootSector0Array[BS0_SIZE];
	uint8_t _BootSector1Array[BS1_SIZE];
	
	uint8_t _MainSector0Array[MS0_SIZE];//MS0_SIZE
	uint8_t _MainSector1Array[MS1_SIZE];

	uint8_t _UserSpecifiedBS0[BS0_SIZE];
	uint8_t _UserSpecifiedBS1[BS1_SIZE];

	int		_bCheckTAG_LNA;
	int		_bCheckTAG_PGA_OOPR;
	int		_bCheckTAG_SNR;
	int		_bCheckTAG_FlexId;
	int		_bCheckTAG_WOF_BOT;
	int		_bCheckTAG_DutTempAdc;
	int		_bCheckTAG_WOF_TOP;
	int		_bCheckTAG_SCM_WOF_BOT;
	int		_bCheckTAG_SCM_WOF_TOP;
	int		_bCheckTAG_PGA_OOPP;
	int		_bCheckTAG_PRODUCT_ID;
	int		_bCheckTAG_PART_NUMBERS;

}OTPCheckInfo;

typedef struct
{
	uint8_t		_PRD_JTAG;
	uint8_t		_ENG_JTAG;
	uint16_t	_Reserved;
	uint8_t		_FWSecure;	//bit
	uint8_t		_AES_Export;

	int			_bPass;
}OTPCheckResults;

typedef struct
{
	int	_bExecuted;
	int _highLimit;
	int _lowLimit;
	int _numSamples;
}HuaweiIQTestInfo;

typedef struct
{
	float	snr;
	int		singalValue;
	float	nosieValue;
	int		_bPass;

	double  m_elapsedtime;
}HuaweiIQTestResults;

typedef struct
{
	uint8_t  _GerVerArray[VERSION_SIZE];

	uint32_t   buildtime;        /* Unix-style build time, in seconds   *//*  from 1/1/1970 12:00 AM GMT         */
	uint32_t   buildnum;         /* build number                        */
	uint8_t    vmajor;           /* major version                       */
	uint8_t    vminor;           /* minor version                       */
	uint8_t    target;           /* target, e.g. VCSFW_TARGET_ROM       */
	uint8_t    product;          /* product, e.g.  VCSFW_PRODUCT_FALCON */
	uint8_t    siliconrev;       /* silicon revision                    */
	uint8_t    formalrel;        /* boolean: non-zero -> formal release */
	uint8_t    platform;         /* Platform (PCB) revision             */
	uint8_t    patch;            /* patch level                         */
	uint8_t    serial_number[6]; /* 48-bit Serial Number                */
	uint8_t    security[2];      /* bytes 0 and 1 of OTP                */
	uint32_t   patchsig;         /* opaque patch signature              */
	uint8_t    iface;            /* interface type, see below           */
	uint8_t    otpsig[3];        /* OTP Patch Signature                 */
	uint16_t   otpspare1;        /* spare space                         */
	uint8_t    reserved;         /* reserved byte                       */
	uint8_t    device_type;      /* device type                         */

	char sSerialNumber[12];
}GetVerInfo;

///////////////////////////// ////////////////////////
//////////////////////////// ////////////////////////
typedef struct
{
	unsigned long serial_number;
}LogWriter;
///////////////////////////// ////////////////////////
//////////////////////////// ////////////////////////


typedef struct
{
	uint8_t arr[MAXROW+1][MAXCOL];

}FPSFrame;

typedef struct
{
	FPSFrame arr[MAXFRAMES]; //FPSFrame* arr;
}FPSSample;

typedef struct
{
	float arr[MAXROW][MAXCOL];
}FPSMatrix;

///////////////////////////// ////////////////////////
//////////////////////////// ////////////////////////
///////////////////////////	////////////////////////

/////////////////////////////CALIBRATION STRUCTS////////////////////////
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
typedef struct 
{
	uint8_t      mt_partnum[MAX_PART_NUMBER_LENGTH];  /* MT SW part number */
    uint8_t      mt_config[MAX_PART_NUMBER_LENGTH];   /* MT test config */
    uint8_t      mt_month;       /* 0-11 */
    uint8_t      mt_day;         /* 0-30 */
    uint16_t     mt_year;        /* 0-65535 (little-endian format) */
}MtAndConfigPnInfo;
//structrues for SNR Test
// It's possible to provide capability to get args from SysConfig.xml
// It needs Mike's confirm before we use these parameters in SYN_SNRExecute()
typedef struct
{
	int m_bExecuted;
	int numFrames;
	int numcols;
	int numrows;
	int numMinRows;
	int numMaxRows;
	int numMinCols;
	int numMaxCols;
	int percent;

	int limit_overall_snr_high;
	int limit_overall_snr_low;
	int limit_overall_signal_high;
	int limit_overall_signal_low;
	int limit_overall_noise_high;
	int limit_overall_noise_low;
	int limit_zone_snr_high;
	int limit_zone_snr_low;
	int limit_zone_signal_high;
	int limit_zone_signal_low;
	int limit_zone_noise_high;
	int limit_zone_noise_low;
}SNRInfo;

typedef struct
{
	int bPass;
	int SIGNAL[REGIONS];
	float NOISE[REGIONS];
	float SNR[REGIONS];
	FPSMatrix STD[REGIONS];
	FPSMatrix NORM_AVGS[REGIONS];
	FPSMatrix PIXEL_AVGS[REGIONS];
	//for logfile
	float AVG_NOFINGER[MAXROW][MAXCOL];
	float SPAN_NOFINGER[MAXROW][MAXCOL];
	float STD_NOFINGER[MAXROW][MAXCOL];
	float AVG_FINGER[MAXROW][MAXCOL];
	float SPAN_FINGER[MAXROW][MAXCOL];
	float STD_FINGER[MAXROW][MAXCOL];

	float OTPVal_SNR;
	float OTPVal_Noise;
	int   OTPVal_Signal;

	double      m_elapsedtime;
}SNRResults;
//end structrues for SNR Test


///////////////////////////// ////////////////////////
//LEDTest Structures
typedef struct
{
	int		  m_nGPIO1;
	int		  m_nGPIO2;
	int		  m_nGPIO3;
	char	  m_sUserMsg[MAX_LED_TEST_MSG_LEN];
	int		  m_nMpcGpioPin;
	uint32_t  m_nRegisterAddress;
	int		  m_nPokeValue;
	int		  m_nTimeout;
	int		  m_bExecuted;
}LEDTestInfo;
typedef struct
{
	int	m_bPass;
}LEDTestResults;

//SdkBaselineTest Structures
typedef struct
{
	int		  m_nTrimRight;
	int		  m_nTrimLeft;
	int		  m_nTrimTop;
	int		  m_nTrimBottom;
	int		  m_nRightShiftBits;
	int		  m_nStrideWidth;
	int		  m_nMaxSumAllowed;
	int		  m_nPercentFail;
	int		  m_bExecuted;
}SdkBaselineTestInfo;
typedef struct
{
	int m_bPass;
	int m_nNumBadRows;
	int m_nNumGoodRows;
}SdkBaselineTestResults;

///////////////////////////// ////////////////////////
//structrues for Initialization Step
typedef struct
{
	int			m_bExecuted;
	int			m_bBga;
	int			m_nFlexId;
	int			m_nTrimTopWithoutStim;	
	int			m_nTrimBotWithoutStim;	
	int			m_nTrimLeftWithoutStim;	
	int			m_nTrimRightWithoutStim;	
	int			m_nBuildNum;
	int			m_nTrimTopWithStim;	
	int			m_nTrimBotWithStim;	
	int			m_nTrimLeftWithStim;	
	int			m_nTrimRightWithStim;
	int			m_nMaxFileSize_MB;
	int			m_bVerboseMode;
	uint32_t	m_nStartTick;
	uint32_t	m_nProductId;
	int			m_nVdd;
	int			m_nVio;
	int			m_nVddtx;
	int			m_nVled;
}InitializationInfo;

typedef struct
{
	int			m_bPass;
	uint8_t		m_arDutSerNum[DUT_SER_NUM_SIZE];
	int			bMpcVoltagesPass;
	float		m_nMeasuredVdd;
	float		m_nMeasuredVio;
	float		m_nMeasuredVddtx;
	float		m_nMeasuredVled;
	uint8_t*	pModuleTestPatchPtr;
	int			nModuleTestPatchSize;
	int			bProductionMode;
}InitializationResults;

///////////////////////////// ////////////////////////
typedef struct
{
	int			m_bExecuted;
	int			m_nNumImagesToDiscard;
	int			m_nLnaIdx;
	int			m_nPgaIdx;
	int			m_bForceCal;
	int			m_nDiscardImages;
	int			m_nLnaOffsetLow;
	int			m_nLnaOffsetHigh;
	int			m_nPgaLimitLow;
	int			m_nPgaLimitHigh;
	int			m_nCalType;
	float		m_nPgaOffsetRatio;
	int			m_nNumPgaSamples;
	int			m_nPgaVarianceLimit;
	int			m_nHpfOffset;
	int			m_bPgaFineTuning;
}CalibrationInfo;

typedef struct
{
	int			m_bPass;
	int			m_nPrintPatchSize;
	uint8_t		m_pPrintPatch[MAX_PRINTFILE_SIZE + 4];
	int			m_nLNA_count, m_nPGA_OOPR_count, m_nPGA_OOPP_count, m_nSNR_count;
	int			m_nFlexId_count, m_nWofBot_count, m_nWofTop_count, m_nDutTempAdc_count;
	int			m_nScmWofTop_count, m_nScmWofBot_count, m_nProductId_count, m_nPartNumberId_count;
	uint8_t		m_arPgaOffsets[NUM_PGA_OOPP_OTP_ROWS * MAXCOL];
	uint8_t     m_pPGAOtpArray[NUM_PGA_OOPP_OTP_ROWS * MAXCOL];
	int			m_nStage2VarianceScore;

	FPSFrame	arr_ImageFPSFrame;

	double      m_elapsedtime;
}CalibrationResults;

///////////////////////////// ////////////////////////
typedef struct
{
	int			m_bExecuted;
	int			m_nNumResBytes;
	int			m_nDelay_ms;
}AFETestInfo;

typedef struct
{
	int			m_bPass;	
	uint8_t		m_pResponse[MAX_AFE_TEST_RES];

	double      m_elapsedtime;
}AFETestResults;

///////////////////////////	////////////////////////
#define MAX_WOF_DATA		5 * 1024
#define MAX_WOF_LINES		2
#define MAX_THRESHOLD_GAINS	6

typedef struct
{
	int			m_bExecutedWithoutStimulus;
	int			m_bExecutedWithStimulus;
	int			m_bWithStimulus;
	int			m_nMinTriggerThreshold;
	int			m_nMaxTriggerThreshold;
	int			m_nDelta_100;
	int			m_nNumResponseBytes;
	int			m_nNumNoiseSamples;
	int			m_nThresholdOffsetHigh;
	int			m_nThresholdOffsetLow;
	float		m_nStdDevThreshold;
	int			m_nDelta_200;
	int			m_nDelta_200_3p7;
	float		m_nVCC;
	uint8_t		m_arWofData[MAX_WOF_DATA];
	uint8_t		m_arThreshGains[MAX_THRESHOLD_GAINS];
}WofTestInfo;

typedef struct
{
	int			m_bPass;
	int			m_bValid;
	int			m_nGain;
	int			m_nTriggerWithoutStim;
	int			m_nTriggerWithStim;
	uint8_t		m_arDataWithoutStim[MAX_WOF_DATA];
	uint8_t		m_arDataWithStim[MAX_WOF_DATA];

	int			m_nThreshStart;
	int			m_nThreshInc;
	int			m_nThreshStop;
	int			m_nNumThresholds;
	int			m_nGainStart;
	int			m_nGainInc;
	int			m_nGainStop;
	int			m_nNumGains;
	int			m_nResponseSize;

	double      m_elapsedtime;
}WofTestResults;

///////////////////////////	////////////////////////
#define MAX_WOF_DATA		5 * 1024
#define MAX_WOF_LINES		2
#define MAX_THRESHOLD_GAINS	6

typedef struct
{
	int			m_bExecutedWithoutStimulus;
	int			m_bExecutedWithStimulus;
	int			m_bWithStimulus;
	int			m_nMinTriggerThreshold;
	int			m_nMaxTriggerThreshold;
	int			m_nDelta_100;
	int			m_nNumResponseBytes;
	int			m_nNumNoiseSamples;
	int			m_nThresholdOffsetHigh;
	int			m_nThresholdOffsetLow;
	float		m_nStdDevThreshold;
	int			m_nDelta_200;
	int			m_nDelta_200_3p7;		
	float		m_nVCC;
}SCM_WofTestInfo;

typedef struct
{
	int			m_bPass;
	int			m_bValid;
	int			m_nGain;
	int			m_nTriggerWithoutStim;
	int			m_nTriggerWithStim;
	uint8_t		m_arDataWithoutStim[MAX_WOF_DATA];
	uint8_t		m_arDataWithStim[MAX_WOF_DATA];

	int			m_nThreshStart;
	int			m_nThreshInc;
	int			m_nThreshStop;
	int			m_nNumThresholds;
	int			m_nGainStart;
	int			m_nGainInc;
	int			m_nGainStop;
	int			m_nNumGains;
	int			m_nResponseSize;

	double      m_elapsedtime;
}SCM_WofTestResults;

//////////////////////////// ////////////////////////
typedef struct
{
	int			m_nNumResBytes;
	int			m_nDelay_ms;
	int			m_bExecuted;
}WoVarInfo;

typedef struct
{
	int m_bPass;
	uint8_t		m_pResponse[MAX_WOVAR_RES_BYTES];

	double      m_elapsedtime;
}WoVarResults;

///////////////////////////// ////////////////////////
typedef struct
{
	int	m_bPass;
	int pegged_ROW;
	int pegged_COL;
	int avg[MAXROW][MAXCOL];
	//for logfile
	int pegged_pixel_rows[MAXCOL];
	int pegged_pixel_cols[MAXCOL];

	double      m_elapsedtime;
}PeggedPixelsResults;

typedef struct
{
	int	m_bExecuted;
	int numFrames;
	int fail_row;
	int fail_col;
	int fail_limits;
}PeggedPixelsInfo;

//structrues for Pixel Patch Test
//structrues for Pixel Test
typedef struct
{
	int	m_bExecuted;
	int m_nNumResBytes;
	int m_nDelay_ms;
}PixelPatchInfo;

typedef struct
{
	int			m_bPass;
	uint8_t		m_pResponse[MAX_PIXEL_PATCH_RES_BYTES];

	double      m_elapsedtime;
}PixelPatchResults;

//structrues for Pixel Test
typedef struct
{
	int	m_bExecuted;
	int numFrames;		
	int nTrim;
	int nThresholdRange;		
	int fail_row;
	int fail_col;
	int nCountFailedPixels;
}PixelInfo;

typedef struct
{
	int bPass;
	int DA[MAXROW][MAXCOL-HEADER];
	int A[MAXROW][MAXCOL];
	float ROW_STD[MAXROW-1];
	float COL_STD[MAXCOL-1];

	int nMaxPixelValue;
	int nMinPixelValue;
	int nCountAboveThreshold;

	double      m_elapsedtime;
}PixelResults;

///////////////////////////// ////////////////////////
//floored pixels test
typedef struct
{
	int	m_bExecuted;
	int numFrames;
	int fail_row;
	int fail_col;
	int fail_limits;
}FlooredPixelsInfo;

typedef struct
{
	int	m_bPass;
	int floored_ROW;
	int floored_COL;
	int avg[MAXROW][MAXCOL];
	//for logfile
	int floored_pixel_rows[MAXROW];
	int floored_pixel_cols[MAXCOL];

	double      m_elapsedtime;
}FlooredPixelsResults;

///////////////////////////// ////////////////////////
//Consecutive pixels test
typedef struct
{
	int	m_bExecuted;
	int m_peggedThreshold;
	int m_flooredThreshold;
	int m_maxAdjacentPixelsAllowed;
	int m_cutEdge;
	int numFrames;
}ConsecutivePixelsInfo;

typedef struct
{
	int	m_bPass;
	int avg[MAXROW][MAXCOL];
	int consecutive_pegged_rows[MAXROW-2];
	int consecutive_floored_rows[MAXROW-2];
	int consecutive_pegged_cols[MAXCOL-HEADER-3];
	int consecutive_floored_cols[MAXCOL-HEADER-3];

	int floored_ROW;
	int pegged_ROW;
	int floored_COL;
	int pegged_COL;

	double      m_elapsedtime;
}ConsecutivePixelsResults;

///////////////////////////// ////////////////////////
typedef struct
{
	int	m_bExecuted;
	int m_peggedThreshold;
	int m_flooredThreshold;
	int m_maxAdjacentPixelsAllowed;

	int numFrames;
}ImperfectionsTestInfo;

typedef struct
{
	int	m_bPass;
	int avg[MAXROW][MAXCOL];
	int consecutive_pegged_rows[MAXROW-2];
	int consecutive_floored_rows[MAXROW-2];
	int consecutive_pegged_cols[MAXCOL-HEADER-3];
	int consecutive_floored_cols[MAXCOL-HEADER-3];

	int floored_ROW;
	int pegged_ROW;
	int floored_COL;
	int pegged_COL;

	double      m_elapsedtime;
}ImperfectionsTestResults;


///////////////////////////// ////////////////////////
//Current test
typedef struct
{
	int			m_bExecuted;
	uint32_t	m_arAdcBaseLines[NUM_CURRENT_VALUES][KNUMGAINS];
	uint32_t	m_nVdd;
	uint32_t	m_nVio;
	uint32_t	m_nVled;
	uint32_t	m_nVddh;
}AdcBaseLineInfo;

typedef struct
{
	int	m_bExecuted;
	int m_nOversample;	//0, 2, 4, 8, 16, 32, 64
	//0 = No oversampling

	int m_nLowGain;		//Gain used for ADC1 and ADC4:
	int m_nHighGain;	//Gain used for ADC1 and ADC4:
	//0 = gain of 1
	//1 = gain of 10
	//2 = gain of 100
	//3 = gain of 1000

	AdcBaseLineInfo m_ablInfo;
	int	m_nImageAcqDigMax_uA;
	int	m_nImageAcqAnaMax_uA;
	int	m_nImageAcqDigMin_uA;
	int	m_nImageAcqAnaMin_uA;
	int	m_nActiveDigLimit_uA;
	int	m_nActiveAnaLimit_uA;
	int	m_nSleepDigLimit_uA;
	int	m_nSleepAnaLimit_uA;

	int	m_arImageAcqCurrentVals[NUM_CURRENT_VALUES];
	int	m_arActiveCurrentVals[NUM_CURRENT_VALUES];
	int	m_arSleepCurrentVals[NUM_CURRENT_VALUES];
}CurrentInfo;

typedef struct
{
	int bPass;
	int	m_nImageAcqDigCurrent_uA;
	int	m_nImageAcqAnaCurrent_uA;
	int	m_nSleepDigCurrent_uA;
	int	m_nSleepAnaCurrent_uA;

	double      m_elapsedtime;
}CurrentResults;

//////////////////////////// ////////////////////////
///////////////////////////	////////////////////////
//structures for Pegged Pixels Test

typedef struct
{
	UINT8 arr[MAXROW][MAXCOL];
}PeggedPixelsFrame;
typedef struct
{
	PeggedPixelsFrame arr[MAXFRAMES];
}PeggedPixelsSample;
typedef struct
{
	UINT8 arr[MAXROW][MAXCOL];
}PeggedPixelsMatrix;

//end of structures for Pegged Pixels Test


///////////////////////////// ////////////////////////
//////////////////////////// ////////////////////////
///////////////////////////	////////////////////////
//structures for DeltaPixelPixels Test
typedef struct
{
	int	m_bExecuted;
	int numFrames;//7
	int fail_row;//0
	int fail_col;//0
	int fail_limits;//10
}DeltaPixelInfo;

typedef struct
{
	int	m_bPass;
	int pegged_ROW;
	int pegged_COL;
	int finger[MAXROW][MAXCOL];
	int nofinger[MAXROW][MAXCOL];
	int deltaMatrix[MAXROW][MAXCOL];
	//for logfile
	int delta_pixel_rows[MAXCOL];
	int delta_pixel_cols[MAXCOL];
}DeltaPixelResults;
//end of structures for DeltaPixel Test

///////////////////////////// ////////////////////////
//////////////////////////// ////////////////////////
///////////////////////////	////////////////////////
//structures for OpensShorts Test
typedef struct
{
	int			m_bExecuted;
	int			m_nNumResBytes;
	int			m_nDelay_ms;
}OpensShortsInfo;

typedef struct
{
	int			m_bPass;
	uint8_t		m_pResponse[MAX_OPENS_SHORTS_RES];

	double      m_elapsedtime;
}OpensShortsResults;

//CDM16 Structs
////////////////////////////////////////
typedef struct
{
	int arr[MAXROW][MAXCOL];
}DecodedImageFrame;
typedef struct
{
	DecodedImageFrame arr[MAXFRAMES];

	int arr_ImageDecode[MAXROW*MAXCOL];
	uint8_t arr_ImageEncode[MAXROW*MAXCOL];
}DecodedImageSample;
////////////////////////////////////////


//structrues for AcquireFPS Step
typedef struct
{
	int			m_bExecuted;
	int			m_nNumImagesToDiscard;
	int			m_nNumImagesWithoutStimulus;
}AcqImgNoFingerInfo;

typedef struct
{
	FPSFrame	m_arImagesWithoutStimulus[MAXFRAMES];//NONCDM
	FPSFrame	arr_nofinger[MAXFRAMES];//CDM

	FPSFrame	arr_ImageFPSFrame;
	int iRealRowNumber;
	int iRealColumnNumber;
}AcqImgNoFingerResult;

typedef struct
{
	int			m_bExecuted;
	int			m_nNumImagesToDiscard;
	int			m_nNumImagesWithStimulus;
}AcqImgFingerInfo;

typedef struct
{
	FPSFrame	m_arImagesWithStimulus[MAXFRAMES];//NONCDM
	FPSFrame	arr_finger[MAXFRAMES];//CDM

	FPSFrame	arr_ImageFPSFrame;
	int iRealRowNumber;
	int iRealColumnNumber;
}AcqImgFingerResult;

///////////////////////////// ////////////////////////
//////////////////////////// ////////////////////////
///////////////////////////	////////////////////////
enum	{NUM_DRDY_CHECKS = 10};

//structures for DRdy Test
typedef struct
{
	int				m_bExecuted;
	int				m_portId;
	unsigned int	m_pinMsk;
}DRdyInfo;

typedef struct
{
	int			m_bPass;
	uint8_t		m_arHiStates[NUM_DRDY_CHECKS];
	uint8_t		m_arLoStates[NUM_DRDY_CHECKS];

	double      m_elapsedtime;
}DRdyResults;

///////////////////////////// ////////////////////////
//////////////////////////// ////////////////////////
///////////////////////////	////////////////////////
typedef struct
{
	int				m_bExecuted;
	int				m_nExpectedVid;
	int				m_nExpectedPid;
	int				m_nExpectedMaximRev;
	int				m_nSlaveSelPin;
	int				m_nMisoPin;
	int				m_nMosiPin;
	int				m_nSclkPin;
}UsbCommInfo;

typedef struct
{
	int				m_bPass;
	int				m_nAcquiredVid;
	int				m_nAcquiredPid;
}UsbCommResults;

///////////////////////////// ////////////////////////
//////////////////////////// ////////////////////////
///////////////////////////	////////////////////////
enum	{MAX_SPI_FLASH_RES = 500};

typedef struct
{
	int				m_bExecuted;
	int				m_nNumResBytes;
}SpiFlashInfo;

typedef struct
{
	int				m_bPass;
	uint8_t			m_pResponse[MAX_SPI_FLASH_RES];
}SpiFlashResults;

//////////////////////////////////////////////////
enum	{MAX_SPI_OWNERSHIP_RES = 2048};

typedef struct
{
	int				m_bExecuted;
	int				m_nNumResBytes;
}SpiOwnershipInfo;

typedef struct
{
	int				m_bPass;
	uint8_t			m_pResponse[MAX_SPI_OWNERSHIP_RES];
}SpiOwnershipResults;

//////////////////////////// ////////////////////////
enum	{NUM_BTN_CHECKS = 10};

//structures for Button Test
typedef struct
{
	int				m_bWithStimulus;
	int				m_bExecuted;
	int				m_portId;
	unsigned int	m_pinMsk;
	int             m_expectedState;
}BtnTestInfo;

typedef struct
{
	int			m_bPass;
	uint8_t     m_btnCurrState;
	uint8_t		m_arCurrStates[NUM_BTN_CHECKS];

	double      m_elapsedtime;
}BtnTestResults;

//////////////////////////// ////////////////////////
typedef struct
{	
	int     m_bExecuted;
	int		m_nNumResBytes;
	int		m_nDelay_ms;
	uint8_t	m_pResponse[3000];
}RAMTestInfo;

typedef struct
{
	int bPass;
	int bPassCacheDataRam;
	uint8_t	m_pResponseCacheDataRam[MAXROW];
	int bPassCacheTagRam;
	uint8_t	m_pResponseCacheTagRam[MAXROW];
	int bPassCachInstDataRam;
	uint8_t	m_pResponseCachInstDataRam[MAXROW];
	int bPassCachInstTagRam;
	uint8_t	m_pResponseCachInstTagRam[MAXROW];
	int bPassScmAndMainRam;
	uint8_t	m_pResponseScmAndMainRam[MAXROW];

	double      m_elapsedtime;
}RAMTestResults;

//Structs for ProductIDTest
typedef struct
{	
	int m_bPass;
	int m_nOtpProductId;
}ProductIDTestResults;

typedef struct
{
	int m_bExecuted;
}ProductIDTestInfo;
//structs for imaging
typedef struct
{	
	float SHARPNESS[4];//SHARPNESS[0]->Z1, SHARPNESS[1]->Z2, SHARPNESS[2]->Z3, SHARPNESS[3]->OVERALL
	int	  bPass;
	int	  Gx[MAXROW][MAXCOL];
	int	  Gy[MAXROW][MAXCOL];
	int	  h[MAXCOL];
	int	  g[MAXROW][MAXCOL];
	int	  pImg_t[MAXROW][MAXCOL];
	float	percent;

	double      m_elapsedtime;
}SharpnessResults;

typedef struct
{
	int limit;
	int m_bExecuted;
}SharpnessInfo;

typedef struct
{	
	int   m_bExecuted;
	int	  limit;
	int   numFrames;//set to 30 from xml.
}RxStandardDevInfo;

typedef struct
{
	int m_bPass;
	float Rx_STDs[MAXROW];
	float percent[MAXROW];
	float max_percent;
	int	  limit;

	double      m_elapsedtime;
}RxStandardDevResults;

typedef struct
{
	int				m_bExecuted;
}ReadDutAdcInfo;

typedef struct
{
	int			m_bPass;
	int			m_nDutAdc;
}ReadDutAdcResults;

//osc trim
typedef struct
{
	int		  m_bExecuted;
	uint32_t  nLowerLimit_Hz;
	uint32_t  nUpperLimit_Hz;
	uint32_t  nInitialTrim;
	uint32_t  m_OscTrimDefault;
	int		  m_bDefaultValueUsed;
}OscTrimInfo;

typedef struct
{
	int			m_bPass;
	uint32_t	m_nOscTrim;
}OscTrimResults;

//Slow Osc
typedef struct
{
	int	           m_bExecuted;
	uint32_t	   nUpperLimit_Hz;
	uint32_t	   nLowerLimit_Hz;
	uint32_t	   m_nDefaultTrim;
	uint32_t	   m_nDefaultBias;
}SlowOscInfo;

typedef struct
{
	int        m_bPass;
	uint32_t   m_nTrim;
	uint32_t   m_nBias;
	int		   m_bDefaultValueUsed;
}SlowOscResults;

//RetainMode:
typedef struct
{
	int			m_bExecuted;
	int			m_nGain;
	float		m_nMaxCurrent;
	float		m_nMinCurrent;
	int			m_nDelay;
}RetainModeInfo;

typedef struct
{
	int			m_bPass;
	float		m_nRetainModeCurrent;

	double      m_elapsedtime;
}RetainModeResults;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	int			m_bExecuted;
	int			m_nGain;
	int			m_nMaxCurrent_uA;
	int			m_nMinCurrent_uA;
	int			m_nDelay_ms;
}WofLowPowerInfo;

typedef struct
{
	int			m_bPass;
	float		m_nCurrent_uA;

	double      m_elapsedtime;
}WofLowPowerResults;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	int			m_bProduction; 
	int			m_bEngineering;	
	int			m_bExecuted;		
	int			m_bUsbMode;
	int			m_bSpiOwnership;
	int			m_nNumSpiOwnershipRuns;
}SecurityStepInfo;

typedef struct
{
	int			m_bPass;
}SecurityStepResults;

typedef struct
{
	FPSFrame _ImagepFrame;
	int iRealRowNumber;
	int iRealColumnNumber;
}Syn_WaitStimulusResults;// Syn_WaitStimulusInfo;