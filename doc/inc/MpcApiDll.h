#pragma once

#include "importexport.h"

#define	TYPE_VIPER			50
#define	TYPE_METALLICA		48

/* 
    Line Callback procedure type for receving state changes for an IO line
    \params
        context [in] - a pointer to a context that the user wants to receive back 
                        when the callback procedure gets invoked                    
        sName[out] - a pointer to string name of the IO line
        bState[out] - state of the IO line
    Note: a callback procedure of this type will get invoked from a different thread
          than your main thread. Do not do anything blocking in this callback procedure.
*/
typedef void ( __cdecl   *LineCallback_T)(void* context, const char* sName, bool bState, int32_t error, const char* pErrorMsg);
typedef void ( __stdcall *StdLineCallback_T)(void* context, const char* sName, bool bState, int32_t error, const char* pErrorMsg);
typedef void ( __cdecl   *AsyncMsgCallback_T)(void* context, const char* sName, const char* sAsyncStr,int32_t error, const char* pErrorMsg);
typedef void ( __stdcall *StdAsyncMsgCallback_T)(void* context, const char* sName, const char* sAsyncStr, int32_t error, const char* pErrorMsg);
/* 
    Device Callback procedure type for receving remove/arrival state of a device 
    \params
        context [in] - a pointer to a context that the user wants to receive back 
                        when the callback procedure gets invoked                    
        sName[out] - a pointer to string name of the device, currently not use
        state[out] - remove(0x8004)/arrival(0x8000) state of the device
    Note: a callback procedure of this type will get invoked from a different thread
          than your main thread. Do not do anything blocking in this callback procedure.
*/
typedef void ( __cdecl   *DeviceCallback_T)(void* context, const char* sName, uint32_t state, int32_t error, const char* pErrorMsg);
typedef void ( __stdcall *StdDeviceCallback_T)(void* context, const char* sName, uint32_t state, int32_t error, const char* pErrorMsg);
typedef void (__cdecl * AllDeviceCallback_T)(void* context, uint32_t serialNum, uint32_t state);
//asynchronous update firmware callback 
typedef void ( __cdecl   *AsyncFwUpdateCallback_T)(void* context, const char* sName, uint32_t state, int32_t error, const char* pErrorMsg);
typedef void ( __stdcall *StdAsyncFwUpdateCallback_T)(void* context, const char* sName, uint32_t state, int32_t error, const char* pErrorMsg);

MPC_API uint32_t MPC_DoTrans();

MPC_API uint32_t MPC_Initialize();
MPC_API uint32_t MPC_GetNumberOfDevices(int32_t* pDeviceNumber);
MPC_API uint32_t MPC_GetDeviceSerialNumList(uint32_t* pList);
MPC_API uint32_t MPC_GetDeviceSerialNumListEx(uint32_t* pList, uint32_t listLength, uint32_t* pDeviceNumber);
MPC_API uint32_t MPC_GetDeviceSerialNumListEx2(uint32_t* pSerialNumberList, uint32_t* pDeviceList, uint32_t listLength, uint32_t* pDeviceNumber);
MPC_API uint32_t MPC_GetMpcDeviceHandle(uint32_t serNum ,uint32_t* pDeviceHandle);
MPC_API uint32_t MPC_GetDeviceSerialNumber(uint32_t deviceHandle, uint32_t* pSerNum);
MPC_API uint32_t MPC_SetDeviceSerialNumber(uint32_t deviceHandle, uint32_t serNum, uint32_t timeout);
MPC_API uint32_t MPC_SetStandaloneScript(uint32_t deviceHandle, LPCTSTR scriptName, uint32_t timeout);
MPC_API uint32_t MPC_GetStandaloneScript(uint32_t deviceHandle, LPCTSTR scriptName, uint32_t timeout);
MPC_API uint32_t MPC_GetAdcCalibration(uint32_t deviceHandle, uint32_t* pAdc0Gain, uint32_t* pAdc0Offset, uint32_t* pAdc1Gain, uint32_t* pAdc1Offset, uint32_t timeout);
MPC_API uint32_t MPC_SetAdcCalibration(uint32_t deviceHandle, uint32_t adc0Gain, uint32_t adc0Offset, uint32_t adc1Gain, uint32_t adc1Offset, uint32_t timeout);
MPC_API uint32_t MPC_CloseMpcDeviceHandle(uint32_t deviceHandle);
MPC_API uint32_t MPC_SetCommHostInfo(uint32_t deviceHandle, LPCTSTR pHostName, uint32_t portNum);
MPC_API uint32_t MPC_Connect(uint32_t deviceHandle);
MPC_API uint32_t MPC_Disconnect(uint32_t deviceHandle);
MPC_API bool MPC_IsConnected(uint32_t deviceHandle);
MPC_API uint32_t MPC_RmiReadAbs(uint32_t deviceHandle, uint8_t* data, uint32_t length, uint16_t rmiBaseAddress, uint32_t timeout);
MPC_API uint32_t MPC_RmiWriteAbs(uint32_t deviceHandle, uint8_t* data, uint32_t length, uint16_t rmiBaseAddress, uint32_t timeout);
MPC_API uint32_t MPC_RmiReadAbs2(uint32_t deviceHandle, uint16_t portId, uint8_t* data, uint32_t length, uint16_t rmiBaseAddress, uint32_t timeout);
MPC_API uint32_t MPC_RmiWriteAbs2(uint32_t deviceHandle, uint16_t portId, uint8_t* data, uint32_t length, uint16_t rmiBaseAddress, uint32_t timeout);
MPC_API uint32_t MPC_FpRead(uint32_t deviceHandle, uint16_t endpoint, uint16_t command, uint8_t* pDst, uint32_t dstSize, uint32_t timeout);
MPC_API uint32_t MPC_FpWrite(uint32_t deviceHandle, uint16_t endpoint, uint16_t command, uint8_t* pSrc, uint32_t srcSize, uint32_t timeout);
MPC_API uint32_t MPC_FpRead2(uint32_t deviceHandle, uint16_t endpoint, uint16_t command, uint8_t* pDst, uint32_t dstSize, uint32_t timeout);
MPC_API uint32_t MPC_FpWrite2(uint32_t deviceHandle, uint16_t endpoint, uint16_t command, uint8_t* pSrc, uint32_t srcSize, uint32_t timeout);
MPC_API uint32_t MPC_FpGetImage(uint32_t deviceHandle, uint8_t* pDst, uint32_t dstSize, uint32_t timeout);
MPC_API uint32_t MPC_FpGetImage2(uint32_t deviceHandle, uint16_t numRows, uint16_t numCols, uint8_t* pDst, uint16_t blobSize, uint8_t* pBlob, uint32_t nUnused1, uint16_t nUnused2, uint16_t nUnused3, uint32_t timeout);
MPC_API uint32_t MPC_FpGetStatus(uint32_t deviceHandle, uint8_t* pDst, uint32_t dstSize, uint32_t timeout);
MPC_API uint32_t MPC_FpGetStatus2(uint32_t deviceHandle, uint16_t endpoint, uint8_t* pDst, uint32_t dstSize, uint32_t timeout);
MPC_API uint32_t MPC_FpGetVersion(uint32_t deviceHandle, uint8_t* pDst, uint32_t dstSize, uint32_t timeout);
MPC_API uint32_t MPC_FpLoadPatch(uint32_t deviceHandle, uint8_t* pPatch, uint32_t numBytes, uint32_t timeout);
MPC_API uint32_t MPC_FpUnloadPatch(uint32_t deviceHandle, uint32_t timeout);
MPC_API uint32_t MPC_FpOtpRomWrite(uint32_t deviceHandle, uint16_t section, uint16_t sector, uint8_t* pSrc, uint32_t srcSize, uint32_t timeout);
MPC_API uint32_t MPC_FpOtpRomRead(uint32_t deviceHandle, uint16_t section, uint16_t sector, uint8_t* pDst, uint32_t dstSize, uint32_t timeout);
MPC_API uint32_t MPC_FpModBitStream(uint16_t numBytes, uint8_t* pStream, uint16_t bitIdx, uint8_t* pModification, uint16_t numBits);
MPC_API uint32_t MPC_FpSetHvOscTrimValValid(uint16_t fpsType, uint8_t* pStream);
MPC_API uint32_t MPC_FpSetHvOscBiasValValid(uint16_t fpsType, uint8_t* pStream);
MPC_API uint32_t MPC_FpSetSiOscTcValValid(uint16_t fpsType, uint8_t* pStream);
MPC_API uint32_t MPC_FpSetSiOscFreqValValid(uint16_t fpsType, uint8_t* pStream);
MPC_API uint32_t MPC_FpSetHvOscFreqValValid(uint16_t fpsType, uint8_t* pStream);
MPC_API uint32_t MPC_FpSetHvOscFreqTrim(uint16_t fpsType, uint8_t* pStream, uint8_t trim);
MPC_API uint32_t MPC_FpSetHvOscBiasTrim(uint16_t fpsType, uint8_t* pStream, uint8_t trim);
MPC_API uint32_t MPC_FpSetSiOscTempCompTrim(uint16_t fpsType, uint8_t* pStream, uint8_t trim);
MPC_API uint32_t MPC_FpSetSiOscFreqTrim(uint16_t fpsType, uint8_t* pStream, uint16_t trim);
MPC_API uint32_t MPC_FpSetPrductionJtag(uint16_t fpsType, uint8_t* pStream, uint8_t value);
MPC_API uint32_t MPC_FpSetEngineeringJtag(uint16_t fpsType, uint8_t* pStream, uint8_t value);
MPC_API uint32_t MPC_FpSetSpiCommTrue(uint16_t fpsType, uint8_t* pStream);
MPC_API uint32_t MPC_FpSetNoWatchdogTrue(uint16_t fpsType, uint8_t* pStream);
MPC_API uint32_t MPC_FpSleepEnable(uint32_t deviceHandle, uint16_t sleepTimeout_ms, uint32_t timeout);
MPC_API uint32_t MPC_FpModuleTestPassed(uint32_t deviceHandle, uint32_t timeout);
MPC_API uint32_t MPC_WriteScript(uint32_t deviceHandle, LPCTSTR fileName, uint16_t bOverwrite, uint32_t timeout);
MPC_API uint32_t MPC_WriteScriptEx(uint32_t deviceHandle, LPCTSTR fileName, LPCTSTR pArgs, uint16_t bOverwrite, uint32_t timeout);
MPC_API uint32_t MPC_WriteFile(uint32_t deviceHandle, LPCTSTR fileName, uint16_t bOverwrite, uint32_t timeout);
MPC_API uint32_t MPC_WriteFileEx(uint32_t deviceHandle, LPCTSTR fileName, uint8_t* data, uint32_t length, uint16_t bOverwrite, uint32_t timeout);
MPC_API uint32_t MPC_DeleteFile(uint32_t deviceHandle, LPCTSTR fileName, uint32_t timeout);
MPC_API uint32_t MPC_GetFileCount(uint32_t deviceHandle, uint16_t* pFileCount, uint32_t timeout);
MPC_API uint32_t MPC_GetFileInformation(
  uint32_t deviceHandle,
  uint16_t  fileIndex,	//the file indes to query file information
  uint16_t* pFileId,	//file ID
  uint8_t*  pFileName,	//file name, 32 bytes
  uint32_t* pFileType,	//file type
  uint32_t* pNumBytes,  //number of bytes in the file
  uint32_t timeout
);
MPC_API uint32_t MPC_GetFileInformationEx(
  uint32_t	deviceHandle,
  LPCTSTR	fileName,	//File name.
  uint16_t* pFileId,	//file ID
  uint32_t* pFileType,	//file type
  uint32_t* pNumBytes,  //number of bytes in the file
  uint8_t*  pArgs,		//List of arguments (Script only)
  uint32_t* pCrc,		//CRC-32 calculated from data in the file.
  uint32_t timeout
);
MPC_API uint32_t MPC_ExeScript(uint32_t deviceHandle, LPCTSTR fileName, uint32_t timeout);
MPC_API uint32_t MPC_ExeScriptWithArgs(uint32_t deviceHandle, LPCTSTR fileName, LPCTSTR args, uint32_t timeout);
MPC_API uint32_t MPC_ExeScriptSync(uint32_t deviceHandle, uint8_t* pPmmData, uint32_t numPmmBytes, 
                                   uint8_t* pResData, uint32_t resDataMax, uint32_t* pNumResBytes, 
                                   uint32_t timeout);
MPC_API uint32_t MPC_AbortScript(uint32_t deviceHandle, LPCTSTR fileName, uint32_t timeout);
MPC_API uint32_t MPC_KillScript(uint32_t deviceHandle, LPCTSTR fileName, uint32_t timeout);
MPC_API uint32_t MPC_SetScriptSignal(uint32_t deviceHandle, LPCTSTR fileName, uint16_t signal, uint32_t timeout);
MPC_API uint32_t MPC_ScriptStatus(uint32_t deviceHandle,
                                  LPCTSTR fileName, 
                                  uint16_t* pStatus,
                                  uint16_t* pInternalError,
                                  uint32_t* pInternalStatus, 
                                  uint16_t* pNumBytes,
                                  uint32_t timeout);
MPC_API uint32_t MPC_ScriptOutput(uint32_t deviceHandle, LPCTSTR fileName, uint8_t* data, uint16_t numBytes, uint32_t timeout);
MPC_API uint32_t MPC_UpdateFirmware(uint32_t deviceHandle, LPCTSTR fileName, uint32_t timeout);
MPC_API uint32_t MPC_AsyncUpdateFirmware(uint32_t deviceHandle, LPCTSTR fileName, void* context, AsyncFwUpdateCallback_T callback, StdAsyncFwUpdateCallback_T stdCallback);
MPC_API uint32_t MPC_GetUpdateFirmwareProgress(uint32_t deviceHandle, uint32_t* pProgress);
MPC_API uint32_t MPC_AbortUpdateFirmware(uint32_t deviceHandle);
MPC_API uint32_t MPC_SetVoltages(uint32_t deviceHandle, uint16_t vdd, uint16_t vio, uint16_t vled, uint16_t vddtx, uint32_t timeout);
MPC_API uint32_t MPC_SetAttention(uint32_t deviceHandle, uint16_t enabled, uint16_t assertionState, uint32_t timeout);
MPC_API uint32_t MPC_GetAndClearAttentionFlag(uint32_t deviceHandle, uint16_t* pbAttn, uint32_t timeout);
MPC_API uint32_t MPC_ClearAttentionFlag(uint32_t deviceHandle, uint32_t timeout);
MPC_API uint32_t MPC_RegisterAttentionCallback(uint32_t deviceHandle, void* context, LineCallback_T callback);
MPC_API uint32_t MPC_RegisterAttentionStdCallback(uint32_t deviceHandle, void* context, StdLineCallback_T stdCallback);
MPC_API uint32_t MPC_DeregisterAttentionCallback(uint32_t deviceHandle, void* context, LineCallback_T callback);
MPC_API uint32_t MPC_DeregisterAttentionStdCallback(uint32_t deviceHandle, void* context, StdLineCallback_T stdCallback);
MPC_API uint32_t MPC_RegisterAsyncMsgCallback(uint32_t deviceHandle, void* context, AsyncMsgCallback_T callback);
MPC_API uint32_t MPC_RegisterAsyncMsgStdCallback(uint32_t deviceHandle, void* context, StdAsyncMsgCallback_T stdCallback);
MPC_API uint32_t MPC_DeregisterAsyncMsgCallback(uint32_t deviceHandle, void* context, AsyncMsgCallback_T callback);
MPC_API uint32_t MPC_DeregisterAsyncMsgStdCallback(uint32_t deviceHandle, void* context, StdAsyncMsgCallback_T stdCallback);
MPC_API uint32_t MPC_SetPortI2c(uint32_t deviceHandle, 
                                uint16_t portId, 
                                uint16_t address, 
                                uint16_t speed, 
                                uint16_t hidEnable, 
                                uint16_t hidMode, 
                                uint32_t timeout);

MPC_API uint32_t MPC_SetPortSpi(uint32_t deviceHandle, 
                                uint16_t portId, 
                                uint16_t chipSel, 
                                uint16_t speed, 
                                uint16_t spiMode, 
                                uint16_t interByteDelay, 
                                uint32_t timeout);

MPC_API uint32_t MPC_SetPortFpSensor(uint32_t deviceHandle, 
                                uint16_t portId, 
                                uint16_t chipSel, 
                                uint16_t speed, 
                                uint16_t spiMode, 
                                uint16_t interByteDelay, 
                                uint32_t timeout);
MPC_API uint32_t MPC_SetPortSerialProg(uint32_t deviceHandle, 
                                       uint16_t portId, 
                                       uint16_t speed_kHz, 
                                       uint16_t interbyteDelay_us, 
                                       uint32_t timeout);
MPC_API uint32_t MPC_SetVirtualPortSpi(uint32_t deviceHandle, 
                                uint16_t portId, 
                                uint16_t slaveSelPort, 
                                uint16_t slaveSelPin, 
                                uint16_t misoPort, 
                                uint16_t misoPin, 
                                uint16_t mosiPort, 
                                uint16_t mosiPin, 
                                uint16_t clkPort, 
                                uint16_t clkPin, 
                                uint16_t speed, 
                                uint16_t spiMode, 
                                uint16_t interByteDelay, 
                                uint16_t bAutoSlaveSel, 
                                uint32_t timeout);
MPC_API uint32_t MPC_SetVirtualPortSpiUsb(uint32_t deviceHandle, 
                                uint16_t portId, 
                                uint16_t slaveSelPort, 
                                uint16_t slaveSelPin, 
                                uint16_t misoPort, 
                                uint16_t misoPin, 
                                uint16_t mosiPort, 
                                uint16_t mosiPin, 
                                uint16_t clkPort, 
                                uint16_t clkPin, 
                                uint16_t speed, 
                                uint16_t spiMode, 
                                uint16_t interByteDelay, 
                                uint16_t bAutoSlaveSel, 
                                uint16_t expectedPid, 
                                uint16_t expectedMax3421eRevNum, 
                                uint16_t hidMode, 
                                uint32_t timeout);
MPC_API uint32_t MPC_SetVirtualPortSpiUsb2(uint32_t deviceHandle, 
                                uint16_t portId, 
                                uint16_t slaveSelPort, 
                                uint16_t slaveSelPin, 
                                uint16_t misoPort, 
                                uint16_t misoPin, 
                                uint16_t mosiPort, 
                                uint16_t mosiPin, 
                                uint16_t clkPort, 
                                uint16_t clkPin, 
                                uint16_t speed, 
                                uint16_t spiMode, 
                                uint16_t interByteDelay, 
                                uint16_t bAutoSlaveSel, 
                                uint16_t expectedVid, 
                                uint16_t expectedPid, 
                                uint16_t expectedMax3421eRevNum, 
                                uint16_t hidMode, 
                                uint32_t timeout);
MPC_API uint32_t MPC_SetVirtualPortSS(uint32_t deviceHandle, 
                                uint16_t portId, 
                                uint16_t bAssert, 
                                uint32_t timeout);
MPC_API _TCHAR* MPC_GetErrorDescription(int32_t error);
MPC_API uint32_t MPC_SetLeds(uint32_t deviceHandle, uint16_t green, uint16_t red, uint16_t amber, uint32_t timeout);
MPC_API uint32_t MPC_GetTestValues(uint32_t deviceHandle, uint16_t overSamples, uint32_t* pGain, uint32_t* pOffset, uint32_t arValues[32], uint32_t timeout);
MPC_API uint32_t MPC_PeriphEnable(uint32_t deviceHandle, uint32_t periphId, uint32_t timeout);
MPC_API uint32_t MPC_GpioSetPinType(uint32_t deviceHandle, uint16_t portId, uint32_t mskPins, uint16_t pinType, uint32_t timeout);
MPC_API uint32_t MPC_GpioPinConfigure(uint32_t deviceHandle, uint32_t pinConfig, uint32_t timeout);
MPC_API uint32_t MPC_GpioPadConfigure(uint32_t deviceHandle, uint16_t portId, uint32_t mskPins, uint32_t strength, uint32_t pinType, uint32_t timeout);
MPC_API uint32_t MPC_GpioDirModSet(uint32_t deviceHandle, uint16_t portId, uint32_t mskPins, uint32_t direction, uint32_t timeout);
MPC_API uint32_t MPC_GpioPinRead(uint32_t deviceHandle, uint16_t portId, uint32_t mskPins, uint32_t* pMskPinState, uint32_t timeout);
MPC_API uint32_t MPC_GpioPinWrite(uint32_t deviceHandle, uint16_t portId, uint32_t mskPins, uint32_t mskPinState, uint32_t timeout);
MPC_API uint32_t MPC_GpioGetFreq(uint32_t deviceHandle, uint16_t portId, uint32_t mskPins, uint32_t* pFreq_Hz, uint32_t timeout);
MPC_API uint32_t MPC_GpioGetFreqHw(uint32_t deviceHandle, uint16_t nPinNum, uint32_t* pFreq_Hz, uint32_t timeout);
MPC_API uint32_t MPC_GetIdentity(uint32_t deviceHandle, uint16_t* pDevType, uint32_t* pRevBootLoader, uint32_t* pRevApplication, uint32_t timeout);
MPC_API uint32_t MPC_GetUsbIds(uint32_t deviceHandle, uint16_t* pVid, uint16_t* pPid, uint32_t timeout);
MPC_API uint32_t MPC_GetDllVersion(uint32_t* pMajor, uint32_t* pMinor, uint32_t* pInternalMajor, uint32_t* pInternalMinor);
MPC_API uint32_t MPC_ReadPort(uint32_t deviceHandle, uint32_t portId, uint8_t* data, uint16_t length, uint32_t timeout);
MPC_API uint32_t MPC_WritePort(uint32_t deviceHandle, uint32_t portId, uint8_t* data, uint16_t length, uint32_t timeout);
MPC_API uint32_t MPC_ReadPort2(uint32_t deviceHandle, uint32_t portId, uint8_t* data, uint16_t length, uint16_t bAssertSS, uint16_t bDeassertSS, uint32_t timeout);
MPC_API uint32_t MPC_WritePort2(uint32_t deviceHandle, uint32_t portId, uint8_t* data, uint16_t length, uint16_t bAssertSS, uint16_t bDeassertSS, uint32_t timeout);
MPC_API uint32_t MPC_GetCurrentSenseValues(uint32_t deviceHandle, uint16_t gain, uint16_t oversample, uint32_t arValues[4], uint32_t timeout);
MPC_API uint32_t MPC_RmiRead(uint32_t deviceHandle, uint8_t* data, uint32_t length, 
                             uint16_t rmiFunctionNum, uint16_t registerGroup, uint16_t registerOffset, 
                             uint32_t timeout);
MPC_API uint32_t MPC_RmiWrite(uint32_t deviceHandle, uint8_t* data, uint32_t length, 
                              uint16_t rmiFunctionNum, uint16_t registerGroup, uint16_t registerOffset, 
                              uint32_t timeout);
MPC_API uint32_t MPC_RmiSelectPage(uint32_t deviceHandle, uint16_t functionNum, uint32_t timeout);
MPC_API uint32_t MPC_RmiRefreshDesTable(uint32_t deviceHandle, uint8_t* pDst, uint16_t dstBufSize, uint32_t timeout);
MPC_API uint32_t MPC_RmiRefreshDesTable2(uint32_t deviceHandle, uint8_t* pDst, uint16_t dstBufSize,
                    uint16_t firstPublicPage, uint16_t lastPublicPage,
                    uint16_t firstPrivatePage, uint16_t lastPrivatePage, uint32_t timeout);

MPC_API uint32_t MPC_RegisterAllDeviceNotification(void* context, AllDeviceCallback_T callback);
MPC_API uint32_t MPC_RegisterDeviceNotification(uint32_t serialNum, void* context, DeviceCallback_T callback);
MPC_API uint32_t MPC_RegisterDeviceStdNotification(uint32_t serialNum, void* context, StdDeviceCallback_T stdCallback);
MPC_API uint32_t MPC_DeregisterDeviceNotification(uint32_t serialNum, void* context, DeviceCallback_T callback);
MPC_API uint32_t MPC_DeregisterDeviceStdNotification(uint32_t serialNum, void* context, StdDeviceCallback_T stdcallback);
MPC_API uint32_t MPC_ResetUut(uint32_t deviceHandle, uint16_t pulseDuration_ms, uint16_t polarity, uint16_t outputType, uint32_t timeout);
MPC_API uint32_t MPC_WriteXBar(uint32_t deviceHandle, uint16_t lineX, uint16_t lineY, uint16_t state, uint32_t timeout);
MPC_API uint32_t MPC_ReadXBar(uint32_t deviceHandle, uint16_t lineX, uint16_t lineY, uint16_t* pState, uint32_t timeout);
MPC_API uint32_t MPC_ClearXBar(uint32_t deviceHandle, uint32_t timeout);
MPC_API uint32_t MPC_ExeProgScript(LPCTSTR fileName);
MPC_API uint32_t MPC_ReadAdc(uint32_t deviceHandle, 
                             uint16_t portIdAdc, uint16_t portIdPin, 
                             uint16_t mskPin, uint16_t speed, 
                             uint16_t resolution, uint16_t oversamples, 
                             uint32_t* pAdcValue, uint32_t timeout);
MPC_API uint32_t MPC_AsicSerialProgMode(uint32_t deviceHandle, uint32_t timeout);
MPC_API uint32_t MPC_WriteScriptBuffer(uint32_t deviceHandle, LPCTSTR filename, 
                                       uint8_t* pData, uint32_t numBytes, 
                                       LPCTSTR args, 
                                       uint16_t bOverwrite, uint16_t writeToRam, 
                                       uint32_t timeout);
MPC_API uint32_t MPC_DownloadScript(uint32_t deviceHandle, 
                                    LPCTSTR filename, 
                                    uint8_t* pBuffer,
                                    uint32_t length,
                                    LPCTSTR args,
                                    uint32_t flags,
                                    uint32_t timeout);
MPC_API uint32_t MPC_GetScriptCompileError(char* pType, char* pMsg, char* pTraceback);

MPC_API uint32_t MPC_GetScriptStatus(uint32_t deviceHandle,
                            LPCTSTR	scriptName,
                            uint16_t* pStatus,
                            uint16_t* pErrorNum,
                            uint16_t* pErrorLine,
                            uint16_t* pInternalError,
                            uint32_t* pInternalStatus,
                            uint16_t* pNumBytes,
                            uint32_t timeout);

MPC_API uint32_t MPC_GetScriptStatus2(uint32_t deviceHandle,
                            LPCTSTR	scriptName,
                            uint16_t* pStatus,
                            uint16_t* pErrorNum,
                            uint16_t* pErrorLine,
                            uint16_t* pInternalError,
                            uint32_t* pInternalStatus,
                            uint16_t* pNumBytes,
                            LPCTSTR	  strErrorFile,
                            uint32_t timeout);

MPC_API uint32_t MPC_GenScriptByteCode(LPCTSTR script, unsigned char* byteCodeBuffer, uint32_t byteCodeBufferSize, uint32_t* generatedSize);
MPC_API uint32_t MPC_GetMpcDeviceHandleWithTimeout(uint32_t serNum ,uint32_t* pDeviceHandle, uint32_t timeout);
MPC_API uint32_t MPC_GetMpcREDHandle(LPCTSTR hostName, uint32_t portNumber, uint32_t* pDeviceHandle, uint32_t timeout);
MPC_API uint32_t MPC_RedConnect(uint32_t handle, uint32_t timeout);
MPC_API uint32_t MPC_SetHwScriptAcceleration(uint32_t deviceHandle, BOOL value);



//internal function
uint32_t MPC_WriteScriptToStorage(uint32_t deviceHandle, 
                                  LPCTSTR fileName, LPCTSTR pArgs, 
                                  uint16_t bOverwrite, uint16_t writeToRam, 
                                  uint32_t timeout);
uint16_t FindRemoteFileId(uint32_t deviceHandle, LPCTSTR fileName, uint32_t timeout);

uint32_t MPC_ScriptOutputEx(uint32_t deviceHandle, uint16_t fileId, uint8_t* data, uint16_t numBytes, uint32_t timeout);
uint32_t MPC_ScriptStatusEx(uint32_t deviceHandle,
                            uint16_t fileId,
                            uint16_t* pStatus,
                            uint16_t* pInternalError,
                            uint32_t* pInternalStatus,
                            uint16_t* pNumBytes,
                            uint32_t timeout);
MPC_API uint32_t MPC_DeleteFileEx(uint32_t deviceHandle, uint16_t fileId, uint32_t timeout);

//HID APIs
MPC_API uint32_t MPC_HidGetReportLength(uint32_t handle, uint32_t id, uint32_t* pLength, uint32_t timeout);
MPC_API uint32_t MPC_HidGetReport(uint32_t handle, uint32_t id, uint8_t* data, uint32_t length, uint32_t* pActualLength, uint32_t timeout);
MPC_API uint32_t MPC_HidSendReport(uint32_t handle, uint32_t id, const uint8_t* data, uint32_t length, uint32_t timeout);
MPC_API uint32_t MPC_HidGetReportTableLength(uint32_t handle, uint32_t id, uint32_t* pLength, uint32_t timeout);
MPC_API uint32_t MPC_HidGetReportTable(uint32_t handle, uint32_t id, uint32_t* data, uint32_t length, uint32_t* pActualLength, uint32_t timeout);
MPC_API uint32_t MPC_HidGetLogicalValueRange(uint32_t handle, uint32_t id, uint32_t offset, 
  uint32_t* min, uint32_t* max, uint32_t timeout);
MPC_API uint32_t MPC_HidGetPhysicalValueRange(uint32_t handle, uint32_t id, uint32_t offset, 
  uint32_t* min, uint32_t* max, uint32_t timeout);
MPC_API uint32_t MPC_HidReset(uint32_t handle, uint32_t timeout);
MPC_API uint32_t MPC_HidSetPower(uint32_t handle, uint8_t state, uint32_t timeout);
MPC_API uint32_t MPC_HidSendCommand(uint32_t handle, uint32_t commandId, uint8_t* data, uint32_t lenght, uint32_t timeout);
MPC_API uint32_t MPC_HidGetReportList(uint32_t handle, uint8_t* reportList, uint32_t* length, uint32_t timeout);
MPC_API uint32_t MPC_HidEnumDevice(uint32_t handle, uint32_t ctrlFlag, uint32_t timeout);
MPC_API uint32_t MPC_HidSwitchMode(uint32_t handle, uint8_t mode, uint32_t timeout);
MPC_API uint32_t MPC_HidFingerModeSupport(uint32_t handle, uint32_t* support, uint32_t timeout);
MPC_API uint32_t MPC_AutoDetectI2cAddress(uint32_t handle, uint8_t* address, uint32_t timeout);

uint32_t GetFileDataPtrLocal(uint32_t deviceHandle, LPCTSTR fileName, uint8_t** ppBuffer, uint32_t timeout);
uint32_t GetFileDataSizeLocal(uint32_t deviceHandle, LPCTSTR fileName, uint32_t* pSize, uint32_t timeout);
int32_t GetScriptArgIntegerLocal(uint32_t deviceHandle, int index);
float GetScriptArgFloatLocal(uint32_t deviceHandle, int index);
const char* GetScriptArgStringLocal(uint32_t deviceHandle, int index);
void SetScriptArgsLocal(uint32_t deviceHandle, const char* args);
void PutScriptDataLocal(uint32_t deviceHandle, unsigned char* data, int numBytes);
uint32_t GetScriptDataSize(uint32_t deviceHandle);
unsigned char* GetScriptDataLocal(uint32_t deviceHandle);
uint32_t ExeScriptLocal(uint32_t deviceHandle, LPCTSTR fileName, LPCTSTR args, uint32_t timeout);
uint32_t ExeScriptLocalAsync(uint32_t deviceHandle, LPCTSTR fileName, LPCTSTR args, uint32_t timeout);
uint32_t FpRead(uint32_t deviceHandle, uint8_t* data, uint32_t length, uint16_t endpoint, uint16_t command, uint32_t timeout);
uint32_t FpWaitForCommandComplete(uint32_t deviceHandle, uint32_t timeout);
uint32_t FpGetCommandErrorCode(uint32_t deviceHandle, uint16_t numBytes, uint32_t timeout);
