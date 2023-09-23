#pragma once

#define OCR_OK									   0					//成功
#define OCR_FAIL								-100					//失败
#define OCR_DET_MODEL_PATH_EMPTY				(OCR_FAIL -  1)			//OCR检测模型加载失败
#define OCR_CLS_MODEL_PATH_EMPTY				(OCR_FAIL -  2)			//OCR方向分类模型加载失败
#define OCR_REC_MODEL_PATH_EMPTY				(OCR_FAIL -  3)			//OCR识别模型加载失败
#define OCR_KEY_FILE_PATH_EMPTY					(OCR_FAIL -  4)			//OCR字典文件加载失败
#define OCR_MEMORY_ALLOCATE_FAILED				(OCR_FAIL -  5)			//动态内存申请失败，内存不足
#define OCR_INIT_FAILED							(OCR_FAIL -  6)			//OCR模型初始化失败
#define OCR_ERROR_PARAM							(OCR_FAIL -  7)			//输入参数错误
#define OCR_ERROR_NOT_INIT						(OCR_FAIL -  8)			//OCR模型未初始化成功
#define OCR_ERROR_IMG_EMPTY						(OCR_FAIL -  9)			//输入图像为空
#define OCR_DETECT_NO_TEXT						(OCR_FAIL -  10)		//图像中未检测到字符


#define SetErrorCode(pErrorCode, ErrorCode) do \
{\
	if(pErrorCode!=nullptr)\
	{\
		*pErrorCode = ErrorCode;\
	}\
}while(0)


#ifdef _MSC_VER
#ifdef OCR_EXPORTS
#define OCR_API __declspec(dllexport)
#else
#define OCR_API __declspec(dllimport)
#endif
#else
#define OCR_API __attribute__ ((visibility("default")))
#endif