#include "parsing.cpp"
#include "CImg/CImg.h"
using namespace cimg_library;

int main(){
  vector<Mesh> mesh_vector = parse_meshes("test.obj");
  vector<LightSource> light_source_vector;
  LightSource light_source = LightSource();
  light_source.position.x=10;
  light_source.position.y=10;
  light_source.position.z=10;
  light_source_vector.push_back(light_source);
  Camera main_camera;
  main_camera.position.x=-10;
  main_camera.position.y=10;
  main_camera.position.z=10;
  main_camera.direction.x=1;
  main_camera.direction.y=-1;
  main_camera.direction.z=-1;
  main_camera.initialize_pixels();
  //only a single sample is necessary due to hemispherical diffusion
  main_camera.sample(mesh_vector,light_source_vector);
  double max_grayscale;
  double max_brightness=244;
  for (int x=0;x<main_camera.x_pixels;x++){
    for (int y=0;y<main_camera.y_pixels;y++){
      if (main_camera.pixels[x][y].grayscale_value>max_grayscale){
        max_grayscale=main_camera.pixels[x][y].grayscale_value;
      }
    }
  }
  CImg<double> img(main_camera.x_pixels,main_camera.y_pixels);
  img.fill(0);
  for (int x=0;x<main_camera.x_pixels;x++){
    for (int y=0;y<main_camera.y_pixels;y++){
      const double color[] = {max_brightness*main_camera.pixels[x][y].grayscale_value/max_grayscale};
      img.draw_point(main_camera.x_pixels-x,y,color);
    }
  }
  img.save("render.png");
  img.display();
  return 0;
}
