#pragma once
#include "OcrliteMacros.h"
#include "OcrStruct.h"

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	* @brief 获取SDK版本号
	* @param char* buf: 输出缓冲区，内存由外部调用者维护
	* @param int* pBufLen: 输入输出参数，指示输出缓冲区长度的指示。调用前，表示缓冲区长度，调用后，表示接口向缓冲区写入的字符数
	* @param int* pErrorCode: 错误码，输出参数
	*/
	OCR_API void OCR_GetVersion(char* buf, int* pBufLen, int* pErrorCode);

	/**
	* @brief PPOCR模型初始化
	* @param const char* kDetModelPath: 文本检测模型路径
	* @param const char* kClsModelPath: 文本方向分类模型路径
	* @param const char* kRecModelPath: 文本识别模型路径
	* @param const char* kKeyFilePath: 字典文件路径
	* @param int nThreads: OCR模型推理线程数
	* @param int gpuIndex: gpu序号
	* @param int* pErrorCode: 错误码，输出参数
	* @return OcrHandle: OCR模型指针，需要OCR_Destroy释放
	*/
	OCR_API OcrHandle OCR_Create(const char* kDetModelPath, const char* kClsModelPath, const char* kRecModelPath,
		const char* kKeyFilePath, int nThreads, int gpuIndex, int* pErrorCode);

	/**
	* @brief OCR模型释放
	* @param OcrHandle handle: OCR模型指针，来自OCR_Create
	* @param int* pErrorCode: 错误码，输出参数
	*/
	OCR_API void OCR_Destroy(OcrHandle handle, int* pErrorCode);

	/**
	* @brief OCR识别识别输入图像中的文本
	* @param OcrHandle handle: OCR模型指针，来自OCR_Create
	* @param OCRIMG* pImage: 输入图像结构体指针
	* @param OCRParam* pParam: OCR参数配置结构体指针
	* @param int* pErrorCode: 错误码，输出参数
	* @return OCRResult* pOcrResult: OCR识别结果
	*/
	OCR_API OcrResult* OCR_Run(OcrHandle handle, OcrImg* pImage, OcrParam* pParam, int* pErrorCode);

	/**
	* @brief 单次OCR识别结果的内存释放
	* @param OCRResult* pOcrResult: OCR识别结果，来自OCR_Run
	* @param int* pErrorCode: 错误码，输出参数
	*/
	OCR_API void OCR_ReleaseResult(OcrResult* pOcrResult, int* pErrorCode);

#ifdef __cplusplus
}
#endif