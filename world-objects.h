#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Face{
public:
  vector<vector<double>> vertices;
  vector<double> normal_vector;
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
