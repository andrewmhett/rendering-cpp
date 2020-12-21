#include "parsing.cpp"

int main(){
  vector<Mesh> mesh_vector = parse_meshes("test.obj");
  for (int i=0;i<mesh_vector.size();i++){
    cout << "NAME: " << mesh_vector[i].name << endl;
    cout << "VERTICES: " << mesh_vector[i].vertices.size() << endl;
    cout << "FACES: " << mesh_vector[i].faces.size() << endl;
    cout << "--------" << endl;
  }
  int samples = 10;
  LightSource light_source = LightSource();
  light_source.position.x=10;
  light_source.position.y=10;
  light_source.position.z=10;
  light_source.radius=1.5;
  return 0;
}
