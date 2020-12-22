#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include "Vector3D.h"

using namespace std;

#define PI 3.14159265

class LightSource{
public:
  Vector3 position;
  double radius=1.5;
};

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

class Ray{
public:
  Vector3 direction;
  Vector3 origin;
  int bounces = 0;
  int max_bounces = 5;
  double project(vector<Mesh> meshes, vector<LightSource> light_sources){
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
              Ray bounce_ray;
              bounce_ray.origin=intersection_point;
              bounce_ray.direction=direction-(n*2*(direction.dot(n)));
              //add random-ness to bounce vector for diffusion
              bounce_ray.direction.x+=(rand()-RAND_MAX/2)/RAND_MAX;
              bounce_ray.direction.y+=(rand()-RAND_MAX/2)/RAND_MAX;
              bounce_ray.direction.z+=(rand()-RAND_MAX/2)/RAND_MAX;
              bounce_ray.max_bounces=bounces;
              bounce_ray.bounces=bounces;
              bounce_ray.project(meshes,light_sources);
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
    if (!bounced){
      for (int i=0;i<light_sources.size();i++){
        Vector3 n = direction*-1;
        Vector3 samp_vec = light_sources[i].position;
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
        if (light_sources[i].position.dist(intersection_point) < light_sources[i].radius){
          double dist = light_sources[i].position.dist(intersection_point);
          double scale = PI/light_sources[i].radius;
          return 1+cos(scale*dist);
        }
      }
    }
    return 0;
  }
};

class Pixel{
public:
  Vector3 position;
  int grayscale_value = 0;
  double trace_ray(Vector3 direction,vector<Mesh> meshes, vector<LightSource> light_sources){
      // trace a ray from this pixel in the direction specified and return a
      // bool describing whether or not the the ray intersects a light source
      Ray ray;
      ray.origin = position;
      ray.direction=direction;
      return ray.project(meshes,light_sources);
  }
};

class Camera{
public:
  Vector3 direction;
  Vector3 position;
  double focal_length=2;
  int x_pixels=2560;
  int y_pixels=1440;
  double diagonal_size = 1;
  vector<vector<Pixel>> pixels;
  void initialize_pixels(){
    pixels.reserve(x_pixels);
    for (int i=0;i<x_pixels;i++){
      pixels[i].reserve(y_pixels);
    }
    double pixel_size = (diagonal_size*pow(pow(x_pixels,2)+pow(y_pixels,2),.5))
          / (pow(x_pixels,2)+pow(y_pixels,2));
    Vector3 global_vertical;
    global_vertical.x=0;
    global_vertical.y=0;
    global_vertical.z=1;
    Vector3 viewport_horizontal = global_vertical.cross(direction);
    Vector3 viewport_vertical = direction.cross(viewport_horizontal);
    for (int x=0;x<x_pixels;x++){
      for (int y=0;y<y_pixels;y++){
        Pixel pixel;
        pixel.position = position+(direction*focal_length)
          +(viewport_horizontal*x*pixel_size)
          +(viewport_vertical*y*pixel_size);
        pixels[x][y]=pixel;
      }
    }
  }
  void sample(vector<Mesh> meshes, vector<LightSource> light_sources){
    for (int x=0;x<x_pixels;x++){
      for (int y=0;y<y_pixels;y++){
        for (int xy_rotation=-90;xy_rotation<90;xy_rotation++){
          for (int z_rotation=-90;z_rotation<90;z_rotation++){
            Vector3 ray_direction = direction;
            ray_direction.x+=cos(xy_rotation*PI/180);
            ray_direction.y+=sin(xy_rotation*PI/180);
            ray_direction.z+=sin(z_rotation*PI/180);
            pixels[x][y].grayscale_value
              +=pixels[x][y].trace_ray(ray_direction,meshes,light_sources);
          }
        }
      }
    }
  }
};
