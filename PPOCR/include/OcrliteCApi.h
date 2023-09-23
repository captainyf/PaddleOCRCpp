#pragma once
#include "OcrliteMacros.h"
#include "OcrStruct.h"

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	* @brief ��ȡSDK�汾��
	* @param char* buf: ������������ڴ����ⲿ������ά��
	* @param int* pBufLen: �������������ָʾ������������ȵ�ָʾ������ǰ����ʾ���������ȣ����ú󣬱�ʾ�ӿ��򻺳���д����ַ���
	* @param int* pErrorCode: �����룬�������
	*/
	OCR_API void OCR_GetVersion(char* buf, int* pBufLen, int* pErrorCode);

	/**
	* @brief PPOCRģ�ͳ�ʼ��
	* @param const char* kDetModelPath: �ı����ģ��·��
	* @param const char* kClsModelPath: �ı��������ģ��·��
	* @param const char* kRecModelPath: �ı�ʶ��ģ��·��
	* @param const char* kKeyFilePath: �ֵ��ļ�·��
	* @param int nThreads: OCRģ�������߳���
	* @param int gpuIndex: gpu���
	* @param int* pErrorCode: �����룬�������
	* @return OcrHandle: OCRģ��ָ�룬��ҪOCR_Destroy�ͷ�
	*/
	OCR_API OcrHandle OCR_Create(const char* kDetModelPath, const char* kClsModelPath, const char* kRecModelPath,
		const char* kKeyFilePath, int nThreads, int gpuIndex, int* pErrorCode);

	/**
	* @brief OCRģ���ͷ�
	* @param OcrHandle handle: OCRģ��ָ�룬����OCR_Create
	* @param int* pErrorCode: �����룬�������
	*/
	OCR_API void OCR_Destroy(OcrHandle handle, int* pErrorCode);

	/**
	* @brief OCRʶ��ʶ������ͼ���е��ı�
	* @param OcrHandle handle: OCRģ��ָ�룬����OCR_Create
	* @param OCRIMG* pImage: ����ͼ��ṹ��ָ��
	* @param OCRParam* pParam: OCR�������ýṹ��ָ��
	* @param int* pErrorCode: �����룬�������
	* @return OCRResult* pOcrResult: OCRʶ����
	*/
	OCR_API OcrResult* OCR_Run(OcrHandle handle, OcrImg* pImage, OcrParam* pParam, int* pErrorCode);

	/**
	* @brief ����OCRʶ�������ڴ��ͷ�
	* @param OCRResult* pOcrResult: OCRʶ����������OCR_Run
	* @param int* pErrorCode: �����룬�������
	*/
	OCR_API void OCR_ReleaseResult(OcrResult* pOcrResult, int* pErrorCode);

#ifdef __cplusplus
}
#endif