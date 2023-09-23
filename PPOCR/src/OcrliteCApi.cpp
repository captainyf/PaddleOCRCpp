#include "version.h"
#include "OcrliteCApi.h"
#include "OcrLite.h"

void OCR_GetVersion(char* buf, int* pBufLen, int* pErrorCode)
{
	if (buf == nullptr || pBufLen == nullptr)
	{
		SetErrorCode(pErrorCode, OCR_ERROR_PARAM);
		return;
	}
	if (*pBufLen < 0)
	{
		SetErrorCode(pErrorCode, OCR_ERROR_PARAM);
		return;
	}

	auto pVersion = PPOCR::GetVersion();

	if (*pBufLen > strlen(pVersion))
	{
		memcpy(buf, pVersion, strlen(pVersion));
		*pBufLen = strlen(pVersion);
	}
	else
		memcpy(buf, pVersion, *pBufLen);

	SetErrorCode(pErrorCode, OCR_OK);
	return;
}

OcrHandle OCR_Create(const char* kDetModelPath, const char* kClsModelPath, const char* kRecModelPath,
	const char* kKeyFilePath, int nThreads, int gpuIndex, int* pErrorCode)
{
	if (kDetModelPath == nullptr)
	{
		SetErrorCode(pErrorCode, OCR_DET_MODEL_PATH_EMPTY);
		return nullptr;
	}
	if (kClsModelPath == nullptr)
	{
		SetErrorCode(pErrorCode, OCR_CLS_MODEL_PATH_EMPTY);
		return nullptr;
	}
	if (kRecModelPath == nullptr)
	{
		SetErrorCode(pErrorCode, OCR_REC_MODEL_PATH_EMPTY);
		return nullptr;
	}
	if (kKeyFilePath == nullptr)
	{
		SetErrorCode(pErrorCode, OCR_KEY_FILE_PATH_EMPTY);
		return nullptr;
	}

	OcrLite* pOcrObj = new OcrLite;
	if (pOcrObj == nullptr)
	{
		SetErrorCode(pErrorCode, OCR_MEMORY_ALLOCATE_FAILED);
		return nullptr;
	}

	pOcrObj->setNumThread(nThreads);
	pOcrObj->setGpuIndex(gpuIndex);
	bool bInit = pOcrObj->initModels(kDetModelPath, kClsModelPath, kRecModelPath, kKeyFilePath);
	if (!bInit)
	{
		if (pOcrObj)
		{
			delete pOcrObj;
			pOcrObj = nullptr;
		}
		SetErrorCode(pErrorCode, OCR_INIT_FAILED);
		return nullptr;
	}

	SetErrorCode(pErrorCode, OCR_OK);
	return (OcrHandle)pOcrObj;
}

void OCR_Destroy(OcrHandle handle, int* pErrorCode)
{
	OcrLite* pOcrObj = (OcrLite*)handle;
	if (pOcrObj)
	{
		delete pOcrObj;
		pOcrObj = nullptr;
	}

	SetErrorCode(pErrorCode, OCR_OK);
	return;
}

OcrResult* OCR_Run(OcrHandle handle, OcrImg* pImage, OcrParam* pParam, int* pErrorCode)
{
	if (handle == nullptr)
	{
		SetErrorCode(pErrorCode, OCR_ERROR_NOT_INIT);
		return nullptr;
	}
	if (pImage == nullptr)
	{
		SetErrorCode(pErrorCode, OCR_ERROR_IMG_EMPTY);
		return nullptr;
	}
	if (pParam == nullptr)
	{
		SetErrorCode(pErrorCode, OCR_ERROR_PARAM);
		return nullptr;
	}

	cv::Mat src;
	if (pImage->channel == 3 || pImage->channel == 1)
	{
		if (pImage->channel == 1)
		{
			cv::Mat srcGray = cv::Mat(pImage->height, pImage->width, CV_8UC1, pImage->pData);
			cv::cvtColor(srcGray, src, cv::COLOR_GRAY2BGR);
		}
		else
			src = cv::Mat(pImage->height, pImage->width, CV_8UC3, pImage->pData);
	}

	OcrParam ocrParam = *pParam;
	if (ocrParam.padding == 0)
		ocrParam.padding = 0;
	if (ocrParam.maxSideLen == 0)
		ocrParam.maxSideLen = 960;
	if (ocrParam.boxScoreThresh == 0)
		ocrParam.boxScoreThresh = 0.5f;
	if (ocrParam.boxThresh == 0)
		ocrParam.boxThresh = 0.3f;
	if (ocrParam.unClipRatio == 0)
		ocrParam.unClipRatio = 2.0f;
	if (ocrParam.doAngle == 0)
		ocrParam.doAngle = 1;
	if (ocrParam.mostAngle = 0)
		ocrParam.mostAngle = 1;
	if (ocrParam.boxScoreThresh == 0)
		ocrParam.boxScoreThresh = 0.5f;		//ÔÝÎ´Ê¹ÓÃ

	OcrLite* pOcrObj = (OcrLite*)handle;
	OcrResult* pOcrResult = new OcrResult;
	*pOcrResult = pOcrObj->detect(src, ocrParam.padding, ocrParam.maxSideLen,
		ocrParam.boxScoreThresh, ocrParam.boxThresh, ocrParam.unClipRatio, ocrParam.doAngle,
		ocrParam.mostAngle);

	if (pOcrResult->strRes.size() == 0)
	{
		SetErrorCode(pErrorCode, OCR_DETECT_NO_TEXT);
		return nullptr;
	}

	SetErrorCode(pErrorCode, OCR_OK);
	return pOcrResult;
}

void OCR_ReleaseResult(OcrResult* pOcrResult, int* pErrorCode)
{
	if (pOcrResult)
	{
		delete pOcrResult;
		pOcrResult = nullptr;
	}

	SetErrorCode(pErrorCode, OCR_OK);
	return;
}
