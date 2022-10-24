#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <valarray>

using namespace std;

// En ruvennut keksimään pyörää uudelleen niin valmiista esimerkistä mukaillen
// funktio joka ottaa desimaaliluvun sisään ja amuuttaa sen murtoluvuksi.
// Muunnoksen tarkkuutta voi säädellä kierros ja tarkkuus parametreilla.
string murtoluvuksi( double in_, int kierroksia=10, double tarkkuus=5e-4) {
  int negatiivinen = (in_<0) ? -1 : 1;
  in_ = abs(in_);

  double kokonaisosa, uusi;
  double desimaaliosa = in_ - floor(in_);  
  unsigned int laskuri = 0;
  
  valarray<double> vec1{double((int)in_),1}, vec2{1,0}, temp;
  while (desimaaliosa > tarkkuus && laskuri < kierroksia) {
    uusi = 1 / desimaaliosa;
    kokonaisosa = int(uusi);
     
    temp = vec1;
    vec1 = kokonaisosa*vec1+vec2;
    vec2 = temp;

    desimaaliosa = uusi - kokonaisosa;
    laskuri++;
    }
  
  ostringstream outvalue;
  if (vec1[1]==1) {
    outvalue << negatiivinen*vec1[0];
  } else {
    outvalue << negatiivinen*vec1[0] << "/" << vec1[1];
  }
  return outvalue.str();
}

string IntersectingLines(string strArr[], int arrLength) {
  //Nopealla googletuksella Cramerin yhtälö on
  // (x,y) = (b1*c2 - b2*c1)/(a1b2-a2b1) , (c1a2-c2a1)/(a1b2-a2b1)
    
  // Yksinkertainen arvojen sisäänluku
  int x1,x2,x3,x4,y1,y2,y3,y4;
  char token;
  istringstream is1(strArr[0]); is1 >> token >> x1 >> token >> y1 >> token;
  istringstream is2(strArr[1]); is2 >> token >> x2 >> token >> y2 >> token;
  istringstream is3(strArr[2]); is3 >> token >> x3 >> token >> y3 >> token;
  istringstream is4(strArr[3]); is4 >> token >> x4 >> token >> y4 >> token;

  // Viivojen koordinaattien erot, deltat
  int x1d = x2 - x1, y1d = y2-y1;
  int x2d = x4 - x3, y2d = y4-y3;

  float determinant = y1d * x2d - x1d * y2d;
  if (determinant == 0) return "no intersection";

  float det1 = (x1d*y1)-(y1d*x1);
  float det2 = (x2d*y3)-(y2d*x3);

  float intersect_x = (x1d*det2 - det1*x2d) / determinant;
  float intersect_y = (y1d*det2 - det1*y2d) / determinant;
  
  ostringstream retString;
  retString << "(" << murtoluvuksi(intersect_x) << "," << murtoluvuksi(intersect_y) << ")";

  return retString.str();

}

int main(void) { 
   
  // keep this function call here
  string A[] = coderbyteInternalStdinFunction(stdin);
  int arrLength = sizeof(A) / sizeof(*A);
  cout << IntersectingLines(A, arrLength);
  return 0;
    
}pa