#ifndef GRAPHICS_H_
#define GRAPHICS_H_


#include"typedef.h"


/* ɨ���ֵͼ����㷨 */
//����ɨ�����õ��Ľ������Ŀ������
//int ScanBnrImg(LINE l,CBmp &bmp,POINT2D **pPts=NULL);

//int ScanBnrImgDDA(LINE l,CBmp &bmp,POINT2D **pPts=NULL);

//ɨ������ͼ�������Ե�㣺
//int   FullScan(CBmp &bmp,POINT2D **pPts=NULL);
void  OrderPts(POINT2D *pPts,int &nPtSum,int nWid,int nHei);


//�ռ�ֱ�ߺ�ƽ��Ľ���
bool IsBackgrd(COLORREF c);

//EDGE  EdgeIntersection(EDGE e1,EDGE e2,POINT3D vp);

void DrawPt_(POINT2D pt, CDC *pDC);

POINT2D LineCross(LINE l1,LINE l2);

POINT3D CrossValue(POINT3D p1,POINT3D p2);


#endif
