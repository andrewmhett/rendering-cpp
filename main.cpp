#include "parsing.cpp"

int main(){
  vector<Mesh> mesh_vector = parse_meshes("test.obj");
  for (int i=0;i<mesh_vector.size();i++){
    cout << "NAME: " << mesh_vector[i].name << endl;
    cout << "VERTICES: " << mesh_vector[i].vertices.size() << endl;
    cout << "FACES: " << mesh_vector[i].faces.size() << endl;
  }
  return 0;
}
