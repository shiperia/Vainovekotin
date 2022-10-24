#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct coord {
  int x;
  int y;
};

// Lasketaan determinantti joka kertoo kiertävätkö pisteet
  // a,b ja c myötä-, vai vastapäivään
int orient(coord a, coord b, coord c) {
  int v = ((b.y - a.y) * (c.x - b.x)) - ((b.x - a.x) * (c.y - b.y));
  if (v==0) return 0; // Kolme pistettä muodostavat suoran.
  return (v>0) ? 1 : 2;
}

string ConvexHullPoints(string strArr[], int arrLength) {
  // Muistelen joskus tehneeni jotain vastaavaa lahjankäärintä-algoritmilla
  // joten koitetaan sillä. Samanlainen sisäänlukutapa kuin edellisessä
  // tehtävässä kun tuntui kerran toimivan.
  vector<coord> inputs;

  //Triviaali tulos
  if (arrLength == 3) return "3";

  char token;
  for (unsigned int i=0;i<arrLength;i++) {
    int x,y;
    istringstream in_( strArr[i] );
    in_ >> token >> x >> token >> y >> token;
    inputs.push_back( coord{x,y} );
  }

  // Aloitetaan kaikkein vasemmanpuolimmaisesta pisteestä (pienin x lajitteluehtoina
  auto start = std::min_element(inputs.begin(), inputs.end(),
                    [=](coord a, coord b){ return a.x < b.x; });
  auto startIndex = std::distance(inputs.begin(), start);

  // varataan tilaa tuloksille
  vector<int> convexHull(arrLength,-1);

  int p = startIndex, q;
  do {
    q = (p+1) % inputs.size();
    // Etsitään josko pisteille p ja q kaikki kolmiot i kolmantena kulmana kiertävät vastapäivään
    for (unsigned int i=0;i<inputs.size();i++)
      if (orient(inputs.at(p), inputs.at(i), inputs.at(q)) == 2) q = i;

    convexHull.at(p) = q;
    p = q;
  } while (p != startIndex); //Jatketaan kunnes palataan aloituspisteeseen

  // Lasketaan kuinka monta osumaa läydettiin ja palautetaan arvo.
  unsigned int totalPoints=0;
  for (unsigned int i=0;i<convexHull.size();i++) {
    if (convexHull.at(i) != -1) {
      totalPoints++;
    }
  }

  stringstream ss;
  ss << totalPoints;

  return ss.str();
}

int main(void) {

  // keep this function call here
  string A[] = coderbyteInternalStdinFunction(stdin);
  int arrLength = sizeof(A) / sizeof(*A);
  cout << ConvexHullPoints(A, arrLength);
  return 0;

}
