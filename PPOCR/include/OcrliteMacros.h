#pragma once

#define OCR_OK									   0					//�ɹ�
#define OCR_FAIL								-100					//ʧ��
#define OCR_DET_MODEL_PATH_EMPTY				(OCR_FAIL -  1)			//OCR���ģ�ͼ���ʧ��
#define OCR_CLS_MODEL_PATH_EMPTY				(OCR_FAIL -  2)			//OCR�������ģ�ͼ���ʧ��
#define OCR_REC_MODEL_PATH_EMPTY				(OCR_FAIL -  3)			//OCRʶ��ģ�ͼ���ʧ��
#define OCR_KEY_FILE_PATH_EMPTY					(OCR_FAIL -  4)			//OCR�ֵ��ļ�����ʧ��
#define OCR_MEMORY_ALLOCATE_FAILED				(OCR_FAIL -  5)			//��̬�ڴ�����ʧ�ܣ��ڴ治��
#define OCR_INIT_FAILED							(OCR_FAIL -  6)			//OCRģ�ͳ�ʼ��ʧ��
#define OCR_ERROR_PARAM							(OCR_FAIL -  7)			//�����������
#define OCR_ERROR_NOT_INIT						(OCR_FAIL -  8)			//OCRģ��δ��ʼ���ɹ�
#define OCR_ERROR_IMG_EMPTY						(OCR_FAIL -  9)			//����ͼ��Ϊ��
#define OCR_DETECT_NO_TEXT						(OCR_FAIL -  10)		//ͼ����δ��⵽�ַ�


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