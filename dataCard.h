#include <card.h>


class dataCard {
	char* name; // name according to the filename
	char Class; // Classes: 2, 3, 4, 5, 6, 7, 8, 9, T, J, Q, K, A
	double distanceToQuery;
public:
	void resetDistance();
	int numBlackTotal (bitmap* bmp);
	bitmap image;
	int numOfBlackPixels;
	char* getName();
	void setName(char* n);
	void setClass (char* c);
	char* getClass();
	void setDistance(double distance);
	double getDistance ();
	dataCard(const char* fname);
	dataCard() : name(NULL), Class('n'), distanceToQuery(0), numOfBlackPixels(0){}
	virtual ~dataCard();
};
