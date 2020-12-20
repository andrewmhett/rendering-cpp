#include <vector>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

#define PI 3.14159265

class Photon{
public:
  vector<double> direction;
  vector<double> position;
  double wavelength;
  void travel(){
    vector<double> new_position(3);
    for (int i=0;i<3;i++){
      new_position[i]=position[i]+direction[i];
    }
  }
};

class Emitter{
public:
  double angular_photon_density;
  vector<int> photon_rgb;
  vector<double> position;
  void emit(vector<Photon> photons){
    for (double degree_xz=0;degree_xz<360;degree_xz+=(1/angular_photon_density)){
      for (double degree_y=0;degree_y<360;degree_y+=(1/angular_photon_density)){
        Photon photon = Photon();
        photon.position=position;
        photon.rgb=photon_rgb;
        photon.direction[0]=sin((PI*degree_xz)/180);
        photon.direction[1]=sin((PI*degree_y)/180);
        photon.direction[2]=cos((PI*degree_xz)/180);
        photons.push_back(photon);
      }
    }
  }
};

class Camera{
  vector<double> direction;
  vector<double> position;
  double focal_length;
};

class Face{
public:
  vector<vector<double>> vertices;
  vector<double> normal_vector;
  void check_collision(Photon particle){
    //check distance of photon from face
  }
};

class Mesh{
public:
  string name;
  vector<vector<double>> vertices;
  vector<vector<double>> normal_vectors;
  vector<vector<vector<int>>> face_components;
  vector<Face> faces;
  int vertices_index_start;
  int normal_vectors_index_start;
  void construct_faces(){
    for (int i=0;i<face_components.size();i++){
      Face face;
      face.normal_vector = normal_vectors[face_components[i][0][2]-1-normal_vectors_index_start];
      for (int v=0;v<face_components[i].size();v++){
        face.vertices.push_back(vertices[face_components[i][v][0]-1-vertices_index_start]);
      }
      faces.push_back(face);
    }
  }
};
