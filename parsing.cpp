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
          current_mesh.faces.push_back(tuples);
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
