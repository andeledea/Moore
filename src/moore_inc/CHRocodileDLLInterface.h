// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in dieser DLL werden mit dem CHROCODILEDEMOLIB_EXPORTS-Symbol
// kompiliert, das in der Befehlszeile definiert wurde. Das Symbol darf nicht f�r ein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// CHROCODILEDEMOLIB_API-Funktionen als aus einer DLL importiert, w�hrend die DLL
// mit diesem Makro definierte Symbole als exportiert ansieht.
#ifndef DLLINTERFACEH
#define DLLINTERFACEH

#ifdef __cplusplus
extern "C" {
#endif

#if (__cplusplus >= 201103L)
#include <cstdint>
#else
#   if defined(_MSC_VER) && (_MSC_VER >= 1900)
#       include <stdint.h>
#   elif defined(_MSC_VER) /* versions of MSVC older than VS2015 need this */
	typedef unsigned __int64 uint64_t;
	typedef signed __int64 int64_t;
	typedef unsigned int uint32_t;
	typedef signed int int32_t;
	typedef unsigned short int uint16_t;
	typedef signed short int int16_t;
#   else
#       include <stdint.h>
#   endif
#endif

#ifdef _WIN32
#	ifdef CHROCODILEDEMOLIB_EXPORTS
#		define CHROCODILEDEMOLIB_API  __declspec(dllexport)
#	else
#		define CHROCODILEDEMOLIB_API __declspec(dllimport)
#	endif
#else
#  define CHROCODILEDEMOLIB_API
#endif

static const int32_t CHR_FirstGen_Device = 0;
static const int32_t CHR_2Gen_Device = 1;
static const int32_t CHR_CLSMPS_Device = 2;
static const int32_t CHR_Compact_Device = 3;

static const int32_t Raw_Spectrum = 0;
static const int32_t Confocal_Spectrum = 1;
static const int32_t FT_Spectrum = 2;

static const int32_t DeviceMode_FreeRun = 0;
static const int32_t DeviceMode_WaitTrigger = 1;
static const int32_t DeviceMode_TriggerEach = 2;


static const int32_t Encoder_Source_PreloadTrigger_A0 = 0;
static const int32_t Encoder_Source_PreloadTrigger_B0 = 1;
static const int32_t Encoder_Source_PreloadTrigger_A1 = 2;
static const int32_t Encoder_Source_PreloadTrigger_B1 = 3;
static const int32_t Encoder_Source_PreloadTrigger_A2 = 4;
static const int32_t Encoder_Source_PreloadTrigger_B2 = 5;
static const int32_t Encoder_Source_PreloadTrigger_A3 = 6;
static const int32_t Encoder_Source_PreloadTrigger_B3 = 7;
static const int32_t Encoder_Source_PreloadTrigger_A4 = 8;
static const int32_t Encoder_Source_PreloadTrigger_B4 = 9;
static const int32_t Encoder_Source_PreloadTrigger_SyncIn = 10;
static const int32_t Encoder_Source_Quadrature = 15;
static const int32_t Encoder_PreloadTrigger_Immediate = 15;

static const int32_t Encoder_Preload_Once = 0;
static const int32_t Encoder_Preload_Eachtime = 1;
static const int32_t Encoder_Preload_Trigger_RisingEdge = 0;
static const int32_t Encoder_Preload_Trigger_FallingEdge = 2;
static const int32_t Encoder_Preload_Trigger_OnEdge = 0;
static const int32_t Encoder_Preload_Trigger_OnLevel = 4;
static const int32_t Encoder_Preload_Active = 8;

static const int32_t DLL_Output_DataFormat_Double = 0;
static const int32_t DLL_Output_DataFormat_Raw = 1;


//error message of last error
CHROCODILEDEMOLIB_API const char * LastError(void);
CHROCODILEDEMOLIB_API const char * LastDeviceError(uint32_t _hConnection);

CHROCODILEDEMOLIB_API const char * LastErrorWithErrorCode(int32_t * _pErrorCode);
CHROCODILEDEMOLIB_API const char * LastDeviceErrorWithErrorCode(uint32_t _hConnection, int32_t * _pErrorCode);

//connect to CHRocodile Device. Device type: 0, first generation CHRocodile device; 1, CHR� device; 2, CLS/MPS device
CHROCODILEDEMOLIB_API int32_t OpenConnection(const char * _strConnectionInfo, int32_t _nDeviceType, int32_t * _pHandle);
//disconnect
CHROCODILEDEMOLIB_API int32_t CloseConnection(uint32_t _hConnection);
CHROCODILEDEMOLIB_API int32_t GetConnectionInfo(uint32_t _hConnection,  const char ** _strConnectionInfo);
CHROCODILEDEMOLIB_API int32_t GetDeviceType(uint32_t _hConnection);

//read the next sample from device
CHROCODILEDEMOLIB_API int32_t GetNextSample(uint32_t _hConnection, double ** _ppData, int32_t * _pnNumberOfSignals);
CHROCODILEDEMOLIB_API int32_t GetNextSampleMultiChannel(uint32_t _hConnection, double ** _ppData, int32_t * _pnNumberOfSignals, int32_t * _pnNumberOfGlobalSigs, int32_t * _pnNumberOfChannels);
//read the newest sample from device, read pointer will automatically move to the newest sample.....
CHROCODILEDEMOLIB_API int32_t GetLastSample(uint32_t _hConnection, double ** _ppData, int32_t * _pnNumberOfSignals);
CHROCODILEDEMOLIB_API int32_t GetLastSampleMultiChannel(uint32_t _hConnection, double ** _ppData, int32_t * _pnNumberOfSignals, int32_t * _pnNumberOfGlobalSigs, int32_t * _pnNumberOfChannels);

//send command to device and wait for response
CHROCODILEDEMOLIB_API int32_t ExecCommand(uint32_t _hConnection, const char * _pstrCmd, const char ** _pstrResponse);
CHROCODILEDEMOLIB_API int32_t ExecCommandWithBinArg(uint32_t _hConnection, const char * _pstrCmd,
	const char * _pBlob, uint32_t _nBlobSize, const char ** _pstrResponse);
CHROCODILEDEMOLIB_API int32_t ExecCommandGeneral(uint32_t _hConnection, const char * _pstrCmd,
	const char * _pBlob, uint32_t  _nBlobSize, int32_t _bWaitForRsp, const char ** _pstrResponse);


//move the read pointer to last valid stream position
CHROCODILEDEMOLIB_API int32_t FlushInputBuffer(uint32_t _hConnection);

//get the counter which indicates the number of data stream failing 
CHROCODILEDEMOLIB_API int32_t GetStreamFailCounter(uint32_t _hConnection);

//set reader to the mode (auto buffer save mode), which automatically save the incoming data to the target buffer..
CHROCODILEDEMOLIB_API int32_t SetToAutoBufferSave(uint32_t _hConnection, double * _pBuffer, int32_t _nSampleNr);
CHROCODILEDEMOLIB_API int32_t SetToAutoBufferSaveWithSizeCheck(uint32_t _hConnection, double * _pBuffer, int32_t _nSampleNr, int32_t * _pBufferSize);
CHROCODILEDEMOLIB_API int32_t SetToAutoBufferSaveWithSizeCheckX(uint32_t _hConnection, double * _pBuffer, int32_t _nSampleNr, int64_t * _pBufferSize);
//get the sample number, which has been saved to the buffer in auto buffer save mode
CHROCODILEDEMOLIB_API int32_t GetAutoBufferSavedSampleNr(uint32_t _hConnection);

//set reader to normal mode...
CHROCODILEDEMOLIB_API int32_t ClearAutoBufferSave(uint32_t _hConnection);

//check whether reader still in the auto. save mode
CHROCODILEDEMOLIB_API int32_t IsAutoBufferSave(uint32_t _hConnection);


//set output signal by device
CHROCODILEDEMOLIB_API int32_t SetOutputSignals(uint32_t _hConnection, int32_t * _anSignals, int32_t _nSignalsCount);
CHROCODILEDEMOLIB_API int32_t GetDeviceOutputSignalIDs(uint32_t _hConnection, int32_t ** _pSignalIDbuffer, int32_t * _pSignalNr);
CHROCODILEDEMOLIB_API int32_t GetDLLOutputSignalIDs(uint32_t _hConnection, int32_t ** _pSignalIDbuffer, int32_t * _pSignalNr);
CHROCODILEDEMOLIB_API int32_t GetDeviceOutputSignalFormatIndex(uint32_t _hConnection, int32_t * _pSignalFormatIdx);
CHROCODILEDEMOLIB_API int32_t GetDeviceChannelNumber(uint32_t _hConnection);


CHROCODILEDEMOLIB_API int32_t SetMeasuringMode (uint32_t _hConnection, int32_t _nMode);
CHROCODILEDEMOLIB_API int32_t GetMeasuringMode(uint32_t _hConnection, int32_t * _pnMode);

CHROCODILEDEMOLIB_API int32_t GetFullScale (uint32_t _hConnection, int32_t * _pnFullScale);

CHROCODILEDEMOLIB_API int32_t SetScanRate (uint32_t _hConnection, float _nScanRate);
CHROCODILEDEMOLIB_API int32_t GetScanRate(uint32_t _hConnection, float * _pnScanRate);

CHROCODILEDEMOLIB_API int32_t SetDataAverage (uint32_t _hConnection, int32_t _nAVD);
CHROCODILEDEMOLIB_API int32_t GetDataAverage(uint32_t _hConnection, int32_t * _pnAVD);

CHROCODILEDEMOLIB_API int32_t SetSpectrumAverage (uint32_t _hConnection, int32_t _nAVS);
CHROCODILEDEMOLIB_API int32_t GetSpectrumAverage(uint32_t _hConnection, int32_t * _pnAVS);

CHROCODILEDEMOLIB_API int32_t SetRefractiveIndex (uint32_t _hConnection, float _nRefIdx);
CHROCODILEDEMOLIB_API int32_t GetRefractiveIndex(uint32_t _hConnection, float * _pnRefIdx);

CHROCODILEDEMOLIB_API int32_t SetABBENumber(uint32_t _hConnection, float _nABBE);
CHROCODILEDEMOLIB_API int32_t GetABBENumber(uint32_t _hConnection, float * _pnABBE);

CHROCODILEDEMOLIB_API int32_t SetRefractiveIndexTable(uint32_t _hConnection, int32_t _nIndex);
CHROCODILEDEMOLIB_API int32_t GetRefractiveIndexTable(uint32_t _hConnection, int32_t * _pnIndex);

CHROCODILEDEMOLIB_API int32_t SetMaterialLayers(uint32_t _hConnection, int32_t _nLayerCount, float * _anRefIdxs, float * _anABBEs, int32_t * _anTableIdxs);
CHROCODILEDEMOLIB_API int32_t GetMaterialLayers(uint32_t _hConnection, int32_t * _pnLayerCount, float ** _panRefIdxs, float ** _panABBEs, int32_t ** _panTableIdxs);


CHROCODILEDEMOLIB_API int32_t SetLampIntensity (uint32_t _hConnection, float _nIntensity);
CHROCODILEDEMOLIB_API int32_t GetLampIntensity(uint32_t _hConnection, float * _pnIntensity);

CHROCODILEDEMOLIB_API int32_t SetLightSouceAutoAdapt (uint32_t _hConnection, int32_t _bAuto, float _nLevel);
CHROCODILEDEMOLIB_API int32_t GetLightSouceAutoAdapt(uint32_t _hConnection, int32_t * _pbAuto, float * _pnLevel);

CHROCODILEDEMOLIB_API int32_t SetOpticalProbe (uint32_t _hConnection, int32_t _nIndex);
CHROCODILEDEMOLIB_API int32_t GetOpticalProbe(uint32_t _hConnection, int32_t * _pnIndex);



CHROCODILEDEMOLIB_API int32_t SetConfocalDetectionThreshold (uint32_t _hConnection, float _nThreshold);
CHROCODILEDEMOLIB_API int32_t GetConfocalDetectionThreshold(uint32_t _hConnection, float * _pnThreshold);

CHROCODILEDEMOLIB_API int32_t SetInterQualityThreshold (uint32_t _hConnection, float _nThreshold);
CHROCODILEDEMOLIB_API int32_t GetInterQualityThreshold(uint32_t _hConnection, float * _pnThreshold);

CHROCODILEDEMOLIB_API int32_t SetDetectionWindowActive (uint32_t _hConnection, int32_t _bActive);
CHROCODILEDEMOLIB_API int32_t GetDetectionWindowActive(uint32_t _hConnection, int32_t * _pbActive);

//valid for CHR�/MPS/CLS device
CHROCODILEDEMOLIB_API int32_t SetDetectionWindow (uint32_t _hConnection, float * _anDWs, int32_t _nWindowNr);
CHROCODILEDEMOLIB_API int32_t GetDetectionWindow(uint32_t _hConnection, float ** _panDWs, int32_t * _pnWindowNr);

//valid for first generation CHRocodile device
CHROCODILEDEMOLIB_API int32_t SetLimit (uint32_t _hConnection, int32_t _bLeft, float _nLimit);
CHROCODILEDEMOLIB_API int32_t GetLimit(uint32_t _hConnection, int32_t _bLeft, float * _pnLimit);

CHROCODILEDEMOLIB_API int32_t SetAnalogOutput (uint32_t _hConnection, int32_t _nIndex, int32_t _nSignalID, float _nMin, float _nMax);
CHROCODILEDEMOLIB_API int32_t GetAnalogOutput(uint32_t _hConnection, int32_t _nIndex, int32_t * _pnSignalID, float * _pnMin, float * _pnMax);

//valid for CHR�/MPS/CLS device
CHROCODILEDEMOLIB_API int32_t SetAnalogOutputExtended (uint32_t _hConnection, int32_t _nIndex, int32_t _nSignalID, 
		float _nMin, float _nMax, float _nVolMin, float _nVolMax, float _nVolInvalid);
CHROCODILEDEMOLIB_API int32_t GetAnalogOutputExtended(uint32_t _hConnection, int32_t _nIndex, int32_t * _pnSignalID,
	float * _pnMin, float * _pnMax, float * _pnVolMin, float * _pnVolMax, float * _pnVolInvalid);



//valid for CHR�/MPS/CLS device
CHROCODILEDEMOLIB_API int32_t SetMedian(uint32_t _hConnection, int32_t _nMedianWidth, float _nPercentile);
CHROCODILEDEMOLIB_API int32_t GetMedian(uint32_t _hConnection, int32_t * _pnMedianWidth, float * _pnPercentile);

//valid for CHR�/MPS/CLS device
CHROCODILEDEMOLIB_API int32_t SetNumberOfPeaks(uint32_t _hConnection, int32_t _nPeakNum);
CHROCODILEDEMOLIB_API int32_t GetNumberOfPeaks(uint32_t _hConnection, int32_t * _pnPeakNum);

//valid for CHR�/MPS/CLS device
CHROCODILEDEMOLIB_API int32_t SetPeakOrdering(uint32_t _hConnection, int32_t _nPeakOrdering);
CHROCODILEDEMOLIB_API int32_t GetPeakOrdering(uint32_t _hConnection, int32_t * _pnPeakOrdering);

//encoder counter related function
CHROCODILEDEMOLIB_API int32_t SetEncoderCounter(uint32_t _hConnection, int32_t _nAxis, int32_t _nPosition);
CHROCODILEDEMOLIB_API int32_t GetEncoderCounter(uint32_t _hConnection, int32_t _nAxis, int32_t * _pnPosition);

CHROCODILEDEMOLIB_API int32_t SetEncoderCounterSource(uint32_t _hConnection, int32_t _nAxis, int32_t _nSource);
CHROCODILEDEMOLIB_API int32_t SetEncoderPreloadFunction(uint32_t _hConnection, int32_t _nAxis, int32_t _nPreloadValue, int32_t _nPreloadFunctionSetting);
CHROCODILEDEMOLIB_API int32_t SetEncoderPreloadFunctionExtended(uint32_t _hConnection, int32_t _nAxis, int32_t _nPreloadValue, int32_t _nPreloadFunctionSetting, int32_t _nTriggerSource);
//encoder trigger related function
CHROCODILEDEMOLIB_API int32_t SetEncoderTriggerProperty(uint32_t _hConnection, int32_t _nEncoderAxis, int32_t _nStartPos, int32_t _nStopPos, float _nInterval,
	bool _bTriggerOnReturnMove);
CHROCODILEDEMOLIB_API int32_t SetEncoderTriggerEnabled(uint32_t _hConnection, bool _bEnabled);
CHROCODILEDEMOLIB_API int32_t SetDeviceTriggerMode(uint32_t _hConnection, int32_t _nTriggerMode);

CHROCODILEDEMOLIB_API int32_t SaveCurrentSettingInDevice(uint32_t _hConnection);


CHROCODILEDEMOLIB_API int32_t DarkReference(uint32_t _hConnection);
CHROCODILEDEMOLIB_API int32_t StartDataStream(uint32_t _hConnection);
CHROCODILEDEMOLIB_API int32_t StopDataStream(uint32_t _hConnection);

//download spectrum, spectrum type: 0, raw spectrum; 1, processed spectrum in confocal mode; 2, FT spectrum
//valid for first generation CHRocodile device and CHRocodile� device
CHROCODILEDEMOLIB_API int32_t DownloadDeviceSpectrum(uint32_t _hConnection, int32_t _nSpecType, int16_t ** _pSpectrumBuffer, int32_t * _pSpectrumLength);
//valid for MPS/CLS device
CHROCODILEDEMOLIB_API int32_t  DownloadDeviceSpectrumsMultiChannel(uint32_t _hConnection, int32_t _nSpecType, int32_t _nStartChannelIdx, int32_t _nChannelNumber,
	int16_t ** _pSpectrumBuffer, int32_t * _pSingleSpectrumLength);
CHROCODILEDEMOLIB_API int32_t  DownloadDeviceSpectrumMultiChannel(uint32_t _hConnection, int32_t _nSpecType, int32_t _nChannelIdx, 
		int16_t ** _pSpectrumBuffer, int32_t * _pSpectrumLength);
CHROCODILEDEMOLIB_API int32_t DownloadCurrentCalibrationTable(uint32_t _hConnection, char ** _pTable, int32_t * _nTableLength);
CHROCODILEDEMOLIB_API int32_t DownloadWaveLengthTable(uint32_t _hConnection, char ** _pTable, int32_t * _nTableLength);
CHROCODILEDEMOLIB_API int32_t DownloadRefractiveIndexTable(uint32_t _hConnection, int32_t _nTableIndex, char ** _pTable, int32_t * _nTableLength);

CHROCODILEDEMOLIB_API int32_t DownloadMultiChannelMaskTable(uint32_t _hConnection, int32_t _bCLS, char ** _pTable, int32_t * _nTableLength);


CHROCODILEDEMOLIB_API int32_t UploadConfocalCalibrationTableFromFile(uint32_t _hConnection, const char * _strFullFileName, int32_t _nProbeSNr, int32_t _nCHRTableIndex);

CHROCODILEDEMOLIB_API int32_t UploadWaveLengthTableFromFile(uint32_t _hConnection, const char * _strFullFileName);

CHROCODILEDEMOLIB_API int32_t UploadRefractiveIndexTableFromFile
 (uint32_t _hConnection, const char * _strFullFileName, const char * _strTableName, int32_t _nCHRTableIndex, float _nRefSRI);

CHROCODILEDEMOLIB_API int32_t UploadMultiChannelMaskTable(uint32_t _hConnection, const char * _strFullFileName);

CHROCODILEDEMOLIB_API int32_t UploadFirmware(uint32_t _hConnection, const char * _strFullFileName);


//CLS profile interpolation
CHROCODILEDEMOLIB_API int32_t SetMultiChannelProfileInterpolation(uint32_t _hConnection, int32_t _nMaxHoleSize);

CHROCODILEDEMOLIB_API int32_t SetDLLOutputDataFormatMode(uint32_t _hConnection, int32_t _eMode);

CHROCODILEDEMOLIB_API int32_t GetDLLOutputDataFormatMode(uint32_t _hConnection);

CHROCODILEDEMOLIB_API int32_t isClientBehind(uint32_t _hConnection);

CHROCODILEDEMOLIB_API int32_t SetDLLLogFileDirectory(const char * _pstrDirectory, int32_t _nMaxFileSize, int32_t _nMaxFileNumber);


CHROCODILEDEMOLIB_API int32_t DumpDLLInternalState(const char * _pstrDirectory);






#ifdef _WIN32
	void CloseAllConnections();
#else
	void CloseAllConnections(); //__attribute__((destructor));
#endif

#ifdef __cplusplus
}
#endif

#endif
