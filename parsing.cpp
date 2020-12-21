#include "world-objects.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

vector<Mesh> parse_meshes(string f_path){
  ifstream input;
  int num_vertices = 0;
  int num_norm_vectors = 0;
  input.open(f_path);
  vector<Mesh> mesh_vector;
  if (input.is_open()) {
    string line;
    Mesh current_mesh;
    while (!input.eof()) {
      getline(input,line);
      if (line.length()>0){
        if (line[0]!='#') {
          if (line[0]=='o') {
            if (current_mesh.name != "") {
              current_mesh.construct_faces();
              mesh_vector.push_back(current_mesh);
            }
            current_mesh = *(new Mesh());
            current_mesh.name = line.substr(2);
            current_mesh.normal_vectors_index_start = num_norm_vectors;
            current_mesh.vertices_index_start = num_vertices;
          }
          if (line.substr(0,2)=="v ") {
            num_vertices++;
            string coord;
            int start_index=2;
            Vector3 vertex;
            for (int o=0;o<3;o++){
              for (int i=start_index;i<line.length();i++){
                if (line[i] != ' '){
                  coord+=line[i];
                }else{
                  start_index=i+1;
                  break;
                }
              }
              switch (o){
                case 0:
                  vertex.x=stod(coord);
                case 1:
                  vertex.y=stod(coord);
                case 2:
                  vertex.z=stod(coord);
              }
              coord="";
            }
            current_mesh.vertices.push_back(vertex);
          }
          else if (line.substr(0,2)=="vn") {
            num_norm_vectors++;
            string component;
            int start_index=3;
            Vector3 vector;
            for (int o=0;o<3;o++){
              for (int i=start_index;i<line.length();i++){
                if (line[i] != ' '){
                  component+=line[i];
                }else{
                  start_index=i+1;
                  break;
                }
              }
              switch (o){
                case 0:
                  vector.x=stod(component);
                case 1:
                  vector.y=stod(component);
                case 2:
                  vector.z=stod(component);
              }
              component="";
            }
            current_mesh.normal_vectors.push_back(vector);
          }
          else if (line.substr(0,2)=="vt") {

          }
          else if (line.substr(0,2)=="f ") {
            string tuple;
            int start_index=2;
            vector<vector<int>> tuples;
            int o=0;
            while (true){
              for (int i=start_index;i<line.length();i++){
                if (line[i] != ' ' && i != line.length()-1){
                  tuple+=line[i];
                }else{
                  start_index=i+1;
                  if (start_index==line.length()){
                    tuple+=line[i];
                  }
                  break;
                }
              }
              string slice;
              vector<int> tuple_vec;
              for (int i=0;i<tuple.length();i++){
                if (tuple[i] != '/' && i != tuple.length()-1){
                  slice+=tuple[i];
                }else{
                  if (i==tuple.length()-1){
                    slice+=tuple[i];
                  }
                  tuple_vec.push_back(stoi(slice));
                  slice="";
                }
              }
              tuples.push_back(tuple_vec);
              o++;
              if (start_index==line.length()){
                break;
              }
              tuple="";
            }
            current_mesh.face_components.push_back(tuples);
          }
        }
      }
    }
    if (current_mesh.name != ""){
      current_mesh.construct_faces();
      mesh_vector.push_back(current_mesh);
    }
  }
  input.close();
  cout << mesh_vector.size() << " OBJECT" << ((mesh_vector.size() == 1) ? "" : "S") <<" PARSED" << endl;
  return mesh_vector;
}
