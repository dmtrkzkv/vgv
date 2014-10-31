using namespace std;
#include <card.h>
#include <stdlib.h>
#include <line.h>
#include <math.h>
#include <iostream>
#include <limits.h>
#include <exception>
#include <vector>

double const pi = 3.14159265359;

void card::RefreshBitmap() { //oeffnet bild
	// uncomment when you want to use a new snapshot from the camera instead of using the same image over and over ...
	// system("/home/ppsadm/NewAndBetter/requestframe.sh"); // image will be copied to ./frame.pnm

	bmp.read("../data/card_images_new/h4.1.pnm");
}

/*
 * Loads all images from the dataset directory and saves them in an array.
 * The images should be named "img1.pnm', "img2.pnm", "img3.pnm", etc. in order the function
 * could read them.
 */
void card::loadAllImages() {
	string file;
	for (int i = 0; i < 104; i++) {
		file = "../data/dataset/img";
		file = file + (char) (i + 1) + ".pnm";
		const char* filename = new char[file.length()+1]; // we cannot pass a string type argument to the bitmap::read function
		filename = file.c_str(); // here we convert the string type to const char*
		cardReader.read(filename); // we need a const char* type of argument here
		cards[i] = cardReader;
	}
}

int card::getValue(void) {
	return 5;
}

bool card::table(point2d p, int redthreshold) { //false falls der punkt p nicht weiss und im Bild ist, sonst true
	if (p.X() >= 0 && p.X() < bmp.getHeigth() && p.Y() >= 0 && p.Y() < bmp.getWidth()) {
		if (bmp.map[p.X()][p.Y()].r >= redthreshold)
			return false;
		else
			return true;
	} else
		return true;
}

point2d* card::getBoundaryPoints(point2d firstPoint, int threshold, float distance, int& num) { //speichert im array result alle punkte, die an der grenze der karte liegen
	const int maxsize = 200;
	const double pi = 3.14159265;
	const double delta_angle = 5.0 / 180.0 * pi;//5.0 degree
	num = 0;
	point2d* result = new point2d[maxsize];

	//get out from the table:
	double angle = pi / 2.0;//90 degree
	while (angle < 10.0 && !table(point2d(firstPoint, angle, distance), threshold))//waehle den angle sodass es zur karte zeigt
		angle += (pi / 2.0);

	point2d center;
	point2d nextPoint = firstPoint;

	do {
		//advance:
		center = nextPoint;

		//store center:
		result[num] = center;
		num++;
		//find next point:
		//move outside if necessary:
		while (!table(point2d(center, angle, distance), threshold))//geht weiter solange es gruen ist
			angle -= delta_angle;
		//move inside if necessary:
		while (table(point2d(center, angle, distance), threshold))//geht weiter solange es weiss ist
			angle += delta_angle;
		nextPoint = point2d(center, angle, distance);
	} while (firstPoint.distance(nextPoint) > 0.75 * distance);

	return result;
}

//origin ist der Ecke der am obersten ist (und vor der lange Kante)
point2d card::getOrigin(point2d its1, point2d its2, point2d its3, point2d its4) {

	if(its1.distance(its2) > its2.distance(its3)){
		if(its1.x < its3.x) {
			return its1;
		}
		else if(its1.x == its3.x){
			if(its1.y < its3.y)
				return its1;
			else
				return its3;
		}
		else {
			return its3;
		}
	}
	else {
		if(its2.x < its4.x)
			return its2;
		else if(its2.x == its4.x) {
			if(its2.y < its4.y)
				return its2;
			else
				return its4;
		}
		else
			return its4;

	}
}
/*
 *  This function computes angle at the origin
 */
double card::getAngle(point2d origin, point2d its1, point2d its2, point2d its3, point2d its4) {
	point2d ref;

	if(origin == its1)
		ref = its4;
	else if(origin == its2)
		ref = its1;
	else if(origin == its3)
		ref = its2;
	else if(origin == its4)
		ref = its3;


	double dx = ref.x - origin.x;
	double dy = ref.y - origin.y;

		if(dy > 0) {

			double delta=dx/dy;

			return atan(delta);

		}

		else {

			double delta = (-dy/dx);
			return atan(delta) + pi/2;

		}

}

/*
 * This function uses the average angle value, delivered by the averageAngle function, to
 * transform it into an average angle that will be passed to the rotateBmp function.
 * This angle can take negative values and values > pi/2
 */

double card::getAngle(double avAngle, point2d origin, point2d its1, point2d its2, point2d its3, point2d its4) {
	point2d ref;

	if(origin == its1)
		ref = its4;
	else if(origin == its2)
		ref = its1;
	else if(origin == its3)
		ref = its2;
	else if(origin == its4)
		ref = its3;


	double dx = ref.x - origin.x;
	double dy = ref.y - origin.y;

		if(dy > 0) {
			if (dx >= 0)
				return avAngle;
			else
				return -avAngle;

		}

		else {
			if (dx >= 0)
				return pi - avAngle;
			else
				return avAngle - pi;

		}

}

/*
 * Average angle computes the angles adjacent to all four sides fo the card and thus
 * increases the precision of angle determination. The angle delivered is always <= pi/2
 * and >= 0.
 */

double card::averageAngle(point2d its1, point2d its2, point2d its3, point2d its4) {
	double dx, dy;
	double tangens1, tangens2, tangens3, tangens4;
		if (its1.distance(its4) < its1.distance(its2)) {
			dx = abs(its1.x - its4.x);
			dy = abs(its1.y - its4.y);
			tangens1 = dx/dy;
			dx = abs(its1.x - its2.x);
			dy = abs(its1.y - its2.y);
			tangens2 = dy/dx;
			dx = abs(its2.x - its3.x);
			dy = abs(its2.y - its3.y);
			tangens3 = dx/dy;
			dx = abs(its4.x - its3.x);
			dy = abs(its4.y - its3.y);
			tangens4 = dy/dx;
		}
		else {
			dx = abs(its1.x - its4.x);
			dy = abs(its1.y - its4.y);
			tangens1 = dy/dx;
			dx = abs(its1.x - its2.x);
			dy = abs(its1.y - its2.y);
			tangens2 = dx/dy;
			dx = abs(its2.x - its3.x);
			dy = abs(its2.y - its3.y);
			tangens3 = dy/dx;
			dx = abs(its4.x - its3.x);
			dy = abs(its4.y - its3.y);
			tangens4 = dx/dy;
		}

	return atan((tangens1 + tangens2 + tangens3 + tangens4)/4);
}



double card::getLength(point2d its,point2d its2,point2d its3,point2d its4){ //gibt die laenge der kuerzere seite zurueck
	if(its.distance(its2) > its.distance(its4))
		return its.distance(its4);
	else
		return its.distance(its2);

}

/*
 * topPoint is the point with smallest x-value (i.e. the "highest" point). Not to confuse with the
 * origin!
 */
point2d card::getTopPoint(point2d its1, point2d its2, point2d its3, point2d its4) {
	point2d points [3] = {its2, its3, its4};
	int minX = its1.X();
	point2d newOrigin = its1;
	for (int i = 0; i <= 2; i++) {
		if (points[i].X() < minX) {
			newOrigin = points[i];
			minX = points[i].X();
		}
	}
	return newOrigin;
}

/*
 * Returns true if the card should be rotated counterclockwise. False otherwise.
 * At the time being the function is not used anywhere.
 */
bool card::rotateMathPositive (point2d origin, point2d its1, point2d its2, point2d its3, point2d its4) {
	if (origin == its1) {
		if (its1.distance(its4) < its1.distance(its2))
			return true;
		else return false;
	}
	else if (origin == its2) {
			if (its2.distance(its1) < its1.distance(its3))
				return true;
			else return false;
		}
	else if (origin == its3) {
			if (its3.distance(its2) < its3.distance(its4))
				return true;
			else return false;
		}
	else {
			if (its4.distance(its3) < its4.distance(its1))
				return true;
			else return false;
		}
}

void card::rotateBmp(bitmap* newBmp, point2d origin,double angle,double length){

	int width = newBmp->getWidth();
	int height = newBmp->getHeigth();

	double scale = length/width;

	  for (int i = 0; i < height; i++) {
	     for (int j = 0; j < width; j++) { //i und j sind Koordinaten im newBmp

	    	int _i = (int) (scale * (cos(angle) * i + sin(angle) * j) + origin.x);//entsprechende Punkte im alten bmp
	    	int _j = (int) (scale * (-sin(angle) * i + cos(angle) * j) + origin.y);

	    	newBmp->map[i][j].r=bmp.map[_i][_j].r;
	    	newBmp->map[i][j].g=bmp.map[_i][_j].g;
	    	newBmp->map[i][j].b=bmp.map[_i][_j].b;
	  		}
	  }

	  cout << "scale: " << scale << " origin: " << origin.x << ";" << origin.y << endl;

}

void card::blackWhite(bitmap* bmp){
	int width = bmp->getWidth();
	int height = bmp->getHeigth();

	int thresholdGreen=128;
	//int thresholdRed = 90;

	for (int i = 0; i < height; i++) {
		     for (int j = 0; j < width; j++) {
		    	 if(bmp->map[i][j].g > thresholdGreen){//wir haben weiss oder gruen, konvertieren es in weiss
		    		 bmp->map[i][j].r=255;
		    		 bmp->map[i][j].g=255;
		    		 bmp->map[i][j].b=255;

		    	 }
		    	 else {
		    		 bmp->map[i][j].r=0;
					 bmp->map[i][j].g=0;
					 bmp->map[i][j].b=0;
		    	 }
		     }
	}


	//"Ecke-Vorbereitung": color top left black angle on the card into white

	for (int i = 0; i < bmp->getHeigth(); i++) {
		if (bmp->map[i][0].r == 255) break;
		for(int j = 0; j < bmp->getWidth(); j++) {
			if (bmp->map[i][j].r == 255) break;
			bmp->map[i][j].r = 255;
			bmp->map[i][j].g = 255;
			bmp->map[i][j].b = 255;
		}

	}
	// paint the first 3 left columns into white
	int j = 0;
	while (j != 3) {
		for (int k = 0; k < bmp->heigth; k++) {
			bmp->map[k][j].r = 255;
			bmp->map[k][j].g = 255;
			bmp->map[k][j].b = 255;
		}
		j++;
	}
	// paint the first 3 top rows into white
	j = 0;
	while(j != 3) {
		for (int k = 0; k < bmp->width; k++) {
				bmp->map[j][k].r = 255;
				bmp->map[j][k].g = 255;
				bmp->map[j][k].b = 255;
		}
	}
}
/*
 * Searches for the card on the table
 */
point2d card::detectBegin(int frame, bitmap bmp, point2d begin) {

	int stepOffset = 60;
	int direction = -1;
	int nextStep;
	int i;

	while (begin.Y() != 0 || begin.Y() != bmp.getWidth()) {
		i = frame;
		while (i < bmp.getHeigth() - frame && bmp.map[i][begin.Y()].r < 128)
			i++;
		if (i < bmp.getHeigth() - frame) {
			begin.setX(i);
			break;
		}
		else {
			nextStep = direction * (abs(nextStep) + stepOffset);
			begin.setY(begin.Y() + nextStep);
			direction = -direction;
		}
	}
	if (begin.Y() == 0 || begin.Y() == bmp.getWidth()) throw ("There is no card on the table.");

	return begin;
}
void card::Demo() {
	const int frame = 5;
	point2d startDetect(0, bmp.getWidth() / 2);

	/*int i, j;

	j = bmp.getWidth() / 2;
	i = frame;
	while (i < bmp.getHeigth() - frame &&  bmp.map[i][j].r < 128)
		i++;
	point2d begin(i, j);*/

	point2d begin = detectBegin(frame, bmp, startDetect);

	int boundarypointnum;
	point2d* boundary = getBoundaryPoints(begin, 128, 20, boundarypointnum);

	/////////////////////
	int it = 0;
	while (!line::in_line(boundary[it], boundary[it + 2], boundary[it + 4]))
		it++;
	line first_line(boundary[it], boundary[it + 4]);
	it = it + 4;

	while (first_line.distance(boundary[it]) < 20)
		it++;
	while (!line::in_line(boundary[it], boundary[it + 2], boundary[it + 4]))
		it++;
	line second_line(boundary[it], boundary[it + 4]);

	point2d its = first_line.intersection(second_line);

	/*
	 * TODO determine its2, its3, its4! Ich hab vergessen, wie wir es gemacht haben.
	 * Dann muss man wieder origin und angle erstellen mithilfe der schon implementierte Methode.
	 */

	/////////////////////////////

#ifdef DISPLAY

	bmp.setPen(255,255,0);
	for (int i = 0; i < boundarypointnum; i++)
		bmp.dot(boundary[i].X(), boundary[i].Y());

	//////////////
	bmp.setPen(255,0,0);
	bmp.dot(its.X(), its.Y());

	bmp.setPen(0,255,0);
	bmp.line(first_line.p1.X(), first_line.p1.Y(), first_line.p2.X(), first_line.p2.Y());
	bmp.setPen(0,0,255);
	bmp.line(second_line.p1.X(), second_line.p1.Y(), second_line.p2.X(), second_line.p2.Y());
	////////////////////////////////////
	/*
	 * TODO Hier muss man bmp rotieren und nach schwarzweiss transformieren.
	 */

	//bmp.write("home/zsemlye/Images/copy.pnm");
	bmp.write("./copy.pnm");
	system("eog ./copy.pnm");
#endif
}
