
#ifndef TYPEDEF_H
#define TYPEDEF_H

#define  MAXNEIGHBOR 10
#define  NGRID 100
#define  NUMALLOC 100
#define  MAXNEIGHBORPOINT 100
#define  MyE  2.71828
#define INFINITE_MAX 65535

const double Infinite_Min=0.0000001;

struct POINT2D
{
	double x,y;
	float  k;	//����б��ֵ��
	int    id;  //���������ţ�
};

struct POINT3D
{
	double x,y,z;
	bool   bIsOutSide;
	bool   IsNull;
};

struct  stFACE
{
	int n;  //the number of vertices
	int vI[10]; //vertex index
	bool IsNull;
};

struct TriAngle{ 
	POINT3D v1;
	POINT3D v2;
	POINT3D v3;
};

struct newFACE
{
	int flag[3];//the point belongs to the boundary of the interior
	int index[3];//the index of the three points
};

struct LINE
{
	float A,B,C;   //ֱ�ߵķ���
	float Xs,Ys,Zs;//ֱ���ϵ�һ����
};


struct GEOIMG_EDGE_
{
	POINT3D P[2];    //the end point of edge
	int start;
	int end;
	float length;
	bool    bIsBound;//the edge is boundary
};

struct GEOIMG_EDGE
{
	int start;
	int end;
	bool isVisited;
	int neighborTri[MAXNEIGHBOR];// at most 10 neighbor Triangles
	bool IsNull;
};


struct SideBand 
{
	int start;
	int end;
	float length;
	int mate;
};

struct MapPoint 
{
	
	POINT2D  p; //2D���λ��
	
	int pointIndex;  //��Ӧ��ά���index
	//��Ӧ��ά�������ֵ��Ҳ������RGBֵ��ʾ����
	float fx; 
	float fy;
	float fz;
};


struct BndPoint 
{
	int sideBandIndex;
	int pointIndex;
	int neighborPnt[MAXNEIGHBORPOINT];//���������ڲ�ĵ�ļ���
};


 
#endif