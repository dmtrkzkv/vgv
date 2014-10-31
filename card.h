#ifndef _card_
#define _card_

#include <imageio.h>
#include <point2d.h>
#include <vector>

using namespace std;

class card {
	bitmap bmp;
	bitmap cardReader;
	bitmap cards[104];
public:
	int getValue();
	void RefreshBitmap();
	bool table(point2d p, int redthreshold);
	point2d* getBoundaryPoints(point2d firstPoint, int threshold, float distance, int& num);
	point2d getOrigin(point2d its1, point2d its2, point2d its3, point2d its4);
	point2d getTopPoint(point2d its1, point2d its2, point2d its3, point2d its4);
	double getAngle(double avAngle, point2d origin, point2d its1, point2d its2, point2d its3, point2d its4);
	point2d detectBegin(int frame, bitmap bmp, point2d begin);
	double getAngle(point2d origin, point2d its1, point2d its2, point2d its3, point2d its4);
	double averageAngle(point2d its1, point2d its2, point2d its3, point2d its4);
	double getLength(point2d its,point2d its2,point2d its3,point2d its4);
	bool rotateMathPositive (point2d origin, point2d its1, point2d its2, point2d its3, point2d its4);
	void rotateBmp(bitmap* newBmp,point2d origin,double angle,double length);
	void blackWhite(bitmap* bmp);
	void Demo();
	void loadAllImages();
};

#endif
