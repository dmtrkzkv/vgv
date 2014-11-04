#include <imageio.h>
#include <stdlib.h>
#include <iostream>
#include <card.h>
//#include <vbl_timer.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

//#define DATABASE

/*Noch zum machen:
 *
 * falls die karte nicht gefunden wird!!
 *
 * doppeltes main mit dem #ifdef DATABASE, so dass es falls es nicht definiert ist das Database mal erzeugt, sonst macht er das normale main.
 *
 * karten konfrontieren (nearest neighbour), falls kein gutes resultat, layer shifting.
 *
 * spieler implementieren --> eeeeasyyy!!! ;D
 * */

using namespace std;

void display(const char* timetype, long ms) {
	long s = 0, min = 0, h = 0;

	s = ms / 1000;
	ms -= 1000 * s;
	min = s / 60;
	s -= 60 * min;
	h = min / 60;
	min -= 60 * h;

	printf("%s (h:min:sec.ms) %ld:%02ld:%02ld.%03ld\n", timetype, h, min, s, ms);
}

bool anotherCard(int totalVal) {
	return totalVal < 20;
}

// int main(int, const char **argv){
//   card crd;
//   int totalVal=0;
//   char a[100];
//   do
//   {
//     cout << "Enter druecken, sobald Karte bereit ist!" << endl;
//     cin.getline(a,10); 
//     crd.RefreshBitmap();

//     vbl_timer watch;
//hrtime_t start = gethrtime();

//     totalVal += crd.getValue();
//     cout << "Points: " << totalVal << endl;
//     display("Time: ",watch.real());
//hrtime_t end = gethrtime();
//cout<<"Time: "<<end-start<<" nanoseconds"<<endl;
//   } while (anotherCard( totalVal ) );
// }

//---------------demo------------------------
#ifdef DATABASE


	int main(int, const char **) {

		printf("Database defined");

		card crd; //definiere karte crd

		crd.RefreshBitmap();//oeffne bild
		try {
			crd.Demo();
		}
		catch (char const* msg) {
			cout << msg;
		}

	}

#else

/*
 * Loads all images from the dataset directory and saves them in an array.
 * The images should be named "img1.pnm', "img2.pnm", "img3.pnm", etc. in order the function
 * could read them.
 */
	int main() {

		printf("Database defined");

		card crd;
		string folder = "../data/card_images_new/";
		string type;
		string file;

		for (int i = 1; i <= 2; i++) {
			for(int k=0 ; k<4; k++) {
				switch(k) {
				case 0:
					type="e";
					break;
				case 1:
					type="h";
					break;
				case 2:
					type="k";
					break;
				case 3:
					type="s";
					break;
				}
				for (int j=1; j<=10; j++){
					file = folder+type+ (char) (j) + "." + (char) (i) + ".pnm";
					cout << file << endl;
				}

			}


		}
	}

#endif

