
#include "graphics.h"
#include "GL/glut.h"
#include "springmass.h"
#include "string"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>


int main(int argc, char** argv)
{
  std::ifstream inFile;

  std::string filename = argv[1];     //provide the file name as argument in the terminal

  inFile.open(filename);

  int dimension;
  int m;
  int n;
  int divs;
  double mass;
  double sf;
  std::stringstream ss;
  std::vector<Mass*> massesV;
  std::vector<Spring*> springsV;

  if (inFile.is_open()) {             // If file is accessible parse it
    std::string temp;
    char temp2;
    getline(inFile,temp);
    ss << temp;
    ss >> dimension;
    ss.clear();
    getline(inFile,temp);
    ss << temp;
    ss >> m;
    ss >> temp2;
    ss >> n;
    ss.clear();
    getline(inFile,temp);
    ss << temp;
    ss >> sf;
    ss.clear();
    getline(inFile,temp);
    ss << temp;
    ss >> divs;
    ss.clear();

    for(int i = 0; i < m; ++i) {       // For every mass parse their intialisation values
      getline(inFile,temp);
      double mass;
      double radius;
      Vector3 position;
      Vector3 velocity;
      
      ss << temp;
      ss >> mass;
      ss >> temp2;
      ss >> radius;
      ss >> temp2;
      ss >> position.x;
      ss >> temp2;
      ss >> position.y;
      ss >> temp2;
      ss >> position.z;
      ss >> temp2;
      ss >> velocity.x;
      ss >> temp2;
      ss >> velocity.y;
      ss >> temp2;
      ss >> velocity.z;
      ss.clear();
      massesV.push_back(new Mass(position, velocity, mass, radius));  // Place the pointers to the mass objects in a vector
    }

    for (int i = 0;i < n; ++i) {  //For every spring parse their initialisation values
      getline(inFile,temp);
      double stiffness;
      double damping;
      double natLen;
      int m1;
      int m2;

      ss << temp;
      ss >> natLen;
      ss >> temp2;
      ss >> stiffness;
      ss >> temp2;
      ss >> damping;
      ss >> temp2;
      ss >> m1;
      ss >> temp2;
      ss >> m2;
      ss.clear();
      // Place the pointers to each spring object in a vector
      springsV.push_back(new Spring(massesV[m1-1], massesV[m2-1], natLen, stiffness, damping));
    }
    
  }
  
  Mass ** masses = &massesV[0];       // Take the pointer to the first instance
  Spring ** springs = &springsV[0];

  SpringMass sm(masses, springs, m,  n, 0);

  
  glutInit(&argc,argv) ;                                    //Initialise glut

  if (dimension == 2) {
    Figure2D figure(springs, masses, &sm, m, n, sf, divs);  //Instantiate the appropriate figure
  } else if (dimension == 3) {
    Figure3D figure(springs, masses, &sm, m, n, 1); 
  }
  
  return 0 ;
}
