#include "parsing.cpp"

int main(){
  vector<Mesh> mesh_vector = parse_meshes("test.obj");
  vector<LightSource> light_source_vector;
  for (int i=0;i<mesh_vector.size();i++){
    cout << "NAME: " << mesh_vector[i].name << endl;
    cout << "VERTICES: " << mesh_vector[i].vertices.size() << endl;
    cout << "FACES: " << mesh_vector[i].faces.size() << endl;
    cout << "--------" << endl;
  }
  LightSource light_source = LightSource();
  light_source.position.x=10;
  light_source.position.y=10;
  light_source.position.z=10;
  light_source_vector.push_back(light_source);
  Camera main_camera;
  main_camera.position.x=-10;
  main_camera.position.x=10;
  main_camera.position.x=10;
  main_camera.direction.x=1;
  main_camera.direction.y=-1;
  main_camera.direction.z=-1;
  int samples=10;
  for (int i=0;i<samples;i++){
    main_camera.sample(mesh_vector,light_source_vector);
  }
  //draw image based on grayscale values of main_camera.pixels
  return 0;
}
