#include <utility>
#include <algorithm>
#include <cmath>

typedef struct { long long x,y; } pkt;
typedef struct { double x,y; } pktd;
typedef struct { long long A,B,C; } prosta;
typedef struct { double A,B,C; } prostad;
#define pi (2*acos(0.0))

// Iloczyn wektorowy
inline long long wekt(pkt p0,pkt p1,pkt p2)
{
	return (p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y);
}

// Iloczyn skalarny
inline long long skal(pkt p0,pkt p1,pkt p2)
{
	return (p1.x-p0.x)*(p2.x-p0.x)+(p2.y-p0.y)*(p1.y-p0.y);
}

/* Przeciecie dwoch prostych; 0- 1 pkt, 1- || rozlaczne, 2- te same */
inline std::pair<int,pktd> przec(prosta w1,prosta w2) 
{
	pktd w;
	if (w1.A*w2.B==w1.B*w2.A)
	{
		if (w1.B*w2.C==w1.C*w2.B) return std::make_pair(2,w);
		else return std::make_pair(1,w);
	}
	double m=double(w1.A*w2.B-w2.A*w1.B);
	w.x=double(w1.B*w2.C-w2.B*w1.C)/m;
	w.y=double(w1.C*w2.A-w2.C*w1.A)/m;
	return std::make_pair(0,w);
}

inline long long sgn(long long x) {
	return x>0?1:x<0?-1:0;
}

// Czy odcinki p1p2 i p3p4 sie przecinaja?
inline bool intersect(pkt p1,pkt p2,pkt p3,pkt p4)
{
	long long x1=std::min(p1.x,p2.x),y1=std::min(p1.y,p2.y);
	long long x2=std::max(p1.x,p2.x),y2=std::max(p1.y,p2.y);
	long long x3=std::min(p3.x,p4.x),y3=std::min(p3.y,p4.y);
	long long x4=std::max(p3.x,p4.x),y4=std::max(p3.y,p4.y);
	if (x2<x3 || x1>x4 || y2<y3 || y1>y4)
	{
		return false;
	}

	return sgn(wekt(p1,p3,p2))*sgn(wekt(p1,p4,p2))<=0
		&& sgn(wekt(p3,p1,p4))*sgn(wekt(p3,p2,p4))<=0;
}

inline bool operator==(pkt p1,pkt p2)
{
	return p1.x==p2.x && p1.y==p2.y;
}

/* Zamiana para pktow->prosta */
inline prosta prostuj(pkt p1,pkt p2)
{
	prosta w;
	w.A=p1.y-p2.y;
	w.B=p2.x-p1.x;
	w.C=p1.x*(p2.y-p1.y)-p1.y*(p2.x-p1.x);
	return w;
}

/* Zamiana para pktow->prosta */
inline prostad prostuj(pktd p1,pktd p2)
{
	prostad w;
	w.A=p1.y-p2.y;
	w.B=p2.x-p1.x;
	w.C=p1.x*(p2.y-p1.y)-p1.y*(p2.x-p1.x);
	return w;
}

/* Przeciecie 2 odcinkow; 0- 1 pkt, 1- rozlaczne, 2- duzo pktow */
inline std::pair<int,pktd> przec(pkt p1,pkt p2,pkt p3,pkt p4)
{
	pktd w;
	if (!intersect(p1,p2,p3,p4)) return std::make_pair(1,w);
	if (p1==p2)
	{ w.x=p1.x; w.y=p1.y; return std::make_pair(0,w); }
	if (p3==p4)
	{ w.x=p3.x; w.y=p3.y; return std::make_pair(0,w); }
	std::pair<int,pktd> wyn=przec(prostuj(p1,p2),prostuj(p3,p4));
	if (2 == wyn.first) // czy moze nie 1-pktowe przeciecie?
	{
		if (p1==p3 || p1==p4) std::swap(p1,p2);
		if (p2==p4) std::swap(p3,p4);
		w.x=p2.x; w.y=p2.y;
		if (p2==p3 && skal(p2,p1,p4)<0) return std::make_pair(0,w);
	}
	return wyn;
}
