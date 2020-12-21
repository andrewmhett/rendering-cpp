#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include "Vector3D.h"

using namespace std;

#define PI 3.14159265

class Face{
public:
  vector<Vector3> vertices;
  Vector3 normal_vector;
  double max_vertex_dist = 0;
  void calculate_maximum_distance(){
    for (int i=0;i<vertices.size();i++){
      for (int v=0;v<vertices.size();v++){
        if (vertices[i].dist(vertices[v])>max_vertex_dist){
          max_vertex_dist=vertices[i].dist(vertices[v]);
        }
      }
    }
  }
};

class Mesh{
public:
  string name;
  vector<Vector3> vertices;
  vector<Vector3> normal_vectors;
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
      face.calculate_maximum_distance();
      faces.push_back(face);
    }
  }
};

class Photon{
public:
  Vector3 direction;
  Vector3 origin;
  int bounces = 0;
  int max_bounces = 5;
  void project(vector<Mesh> meshes){
    bool bounced;
    for (int i=0;i<meshes.size();i++){
      for (int f=0;f<meshes[i].faces.size();f++){
        if (meshes[i].faces[f].normal_vector.dot(direction)>0){
          Vector3 n = meshes[i].faces[f].normal_vector;
          Vector3 samp_vec = meshes[i].faces[f].vertices[0];
          double c = -1*(n.x*origin.x)
            +(n.x*samp_vec.x)
            +(-1*(n.y*origin.y))
            +(n.y*samp_vec.y)
            +(-1*(n.z*origin.z))
            +(n.z*samp_vec.z);
          double t = c/(n.x*direction.x
            +(n.y*direction.y)
            +(n.z*direction.z));
          double x = origin.x+direction.x*t;
          double y = origin.y+direction.y*t;
          double z = origin.z+direction.z*t;
          Vector3 intersection_point;
          intersection_point.x=x;
          intersection_point.y=y;
          intersection_point.z=z;
          bool miss = false;
          for (int v=0;v<meshes[i].faces[f].vertices.size();v++){
            if (meshes[i].faces[f].vertices[v].dist(intersection_point) > meshes[i].faces[f].max_vertex_dist){
              miss=true;
              break;
            }
          }
          if (!miss){
            bounces++;
            if (bounces<max_bounces){
              Photon bounce_photon;
              bounce_photon.origin=intersection_point;
              bounce_photon.direction=direction-(n*2*(direction.dot(n)));
              bounce_photon.max_bounces=bounces;
              bounce_photon.bounces=bounces;
              bounce_photon.project(meshes);
            }
            bounced=true;
            break;
          }
        }
      }
      if (bounced){
        break;
      }
    }

  }
};

class LightSource{
public:
  Vector3 position;
  double radius;
};

class Camera{
  Vector3 direction;
  Vector3 position;
  double focal_length;
  int x_pixels=1440;
  int y_pixels=2560;
  double diagonal_size = .573;
  void sample(){

  }
};
