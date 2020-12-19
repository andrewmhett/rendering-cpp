#include "world-objects.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

vector<Mesh*> parse_meshes(string f_path){
  ifstream input;
  input.open(f_path);
  vector<Mesh*> mesh_vector;
  if (input.is_open()) {
    string line;
    static Mesh current_mesh;
    while (!input.eof()) {
        getline(input,line);
        if (line[0]!='#') {
          if (line[0]=='o') {
            if (current_mesh.name != "") {
              mesh_vector.push_back(&current_mesh);
            }
            current_mesh = Mesh();
            current_mesh.name = line.substr(2);
        }
        if (line.substr(0,2)=="v ") {
          string coord;
          int start_index=2;
          vector<double> vertex(3);
          for (int o=0;o<3;o++){
            for (int i=start_index;i<line.length();i++){
              if (line[i] != ' '){
                coord+=line[i];
              }else{
                start_index=i+1;
                break;
              }
            }
            vertex[o]=stod(coord);
            coord="";
          }
          current_mesh.vertices.push_back(vertex);
        }
        else if (line.substr(0,2)=="vn") {
          string component;
          int start_index=3;
          vector<double> vector(3);
          for (int o=0;o<3;o++){
            for (int i=start_index;i<line.length();i++){
              if (line[i] != ' '){
                component+=line[i];
              }else{
                start_index=i+1;
                break;
              }
            }
            vector[o]=stod(component);
            component="";
          }
          current_mesh.normal_vectors.push_back(vector);
        }
        else if (line.substr(0,2)=="vt") {

        }
        else if (line.substr(0,2)=="f ") {

        }
      }
    }
    if (current_mesh.name != ""){
      mesh_vector.push_back(&current_mesh);
    }
  }
  input.close();
  cout << mesh_vector.size() << " OBJECT" << ((mesh_vector.size() == 1) ? "" : "S") <<" PARSED" << endl;
  return mesh_vector;
}
