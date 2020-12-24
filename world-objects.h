#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include "Vector3D.h"
#include <limits>

using namespace std;

#define PI 3.14159265

class LightSource{
public:
  Vector3 position;
  double radius=2;
};

class Face{
public:
  vector<Vector3> vertices;
  Vector3 normal_vector;
  vector<pair<Vector3,Vector3>> edges;
  // double max_collision_distance=0;
  // void calculate_max_distance(){
    // for (int e=0;e<edges.size();e++){
    //   double dist = edges[e].first.dist(edges[e].second);
    //   if (dist>max_collision_distance){
    //     max_collision_distance=dist;
    //   }
    // }
  // }
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
      pair<Vector3,Vector3> edge;
      int counter=0;
      for (int v=0;v<face_components[i].size();v++){
        counter++;
        switch (counter){
          case 1:
            edge.first=vertices[face_components[i][v][0]-1-vertices_index_start];
          case 2:
            edge.second=vertices[face_components[i][v][0]-1-vertices_index_start];
            face.edges.push_back(edge);
            edge.first=edge.second;
            counter=1;
        }
        face.vertices.push_back(vertices[face_components[i][v][0]-1-vertices_index_start]);
      }
      face.edges[0].second=edge.second;
    //face.compute_area();
    //face.calculate_max_distance();
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
    direction=direction.normalize();
    for (int i=0;i<meshes.size();i++){
      for (int f=0;f<meshes[i].faces.size();f++){
        if (meshes[i].faces[f].normal_vector.dot(direction)<0){
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
          Vector3 comp_point=meshes[i].faces[f].vertices[0];
          Vector3 global_vertical;
          global_vertical.x=0;
          global_vertical.y=0;
          global_vertical.z=1;
          int intersections=0;
          // for (int e=0;e<meshes[i].faces[f].edges.size();e++){
          //   Vector3 a = meshes[i].faces[f].edges[e].first;
          //   Vector3 b = meshes[i].faces[f].edges[e].second;
          //   Vector3 c = intersection_point;
          //   Vector3 d = c+(global_vertical.cross(n)*2*(a.dist(c)));
          //   cout << t.normalize().display() << endl;
          //   cout << u.normalize().display() << endl;
          //   exit(0);
          //   if (t.normalize()==u.normalize()){
          //     intersections++;
          //   }
          // }
          for (int e=0;e<meshes[i].faces[f].edges.size();e++){
            pair<Vector3,Vector3> edge = meshes[i].faces[f].edges[e];
            Vector3 horizontal = global_vertical.cross(n).normalize();
            if (abs((edge.first-edge.second).normalize().dot(horizontal))!=1){
              Vector3 a;
              a.y=n.cross(global_vertical.cross(n)).dot(edge.first);
              a.x=global_vertical.cross(n).dot(edge.first);
              Vector3 b;
              b.y=n.cross(global_vertical.cross(n)).dot(edge.second);
              b.x=global_vertical.cross(n).dot(edge.second);
              Vector3 i;
              i.y=n.cross(global_vertical.cross(n)).dot(intersection_point);
              i.x=global_vertical.cross(n).dot(intersection_point);
              double min_y, max_y, min_x, max_x;
              min_y=min(a.y,b.y);
              max_y=max(a.y,b.y);
              min_x=min(a.x,b.x);
              if (i.y<max_y && i.y>min_y && i.x<min_x){
                intersections++;
              }
            }
          }
          if (intersections%2==0){
            miss=true;
            break;
          }
          // for (int v=1;v<meshes[i].faces[f].vertices.size();v++){
          //  if (intersection_point.dist(meshes[i].faces[f].vertices[v])
          //    >meshes[i].faces[f].max_collision_distance){
          //    miss=true;
          //    break;
          //  }
        // }
          if (!miss){
            bounces++;
            if (bounces<max_bounces){
              Ray bounce_ray;
              bounce_ray.origin=intersection_point;
              double diffusion_intensity = 0;
              double diffusion_density = .03;
              //cout << intersection_point.display() << endl;
              for (double x_rotation=-90;x_rotation<90;x_rotation+=1/diffusion_density){
                for (double y_rotation=-90;y_rotation<90;y_rotation+=1/diffusion_density){
                  for (double z_rotation=-90;z_rotation<90;z_rotation+=1/diffusion_density){
                    bounce_ray.direction=n.normalize();
                    bounce_ray.direction.x+=sin(x_rotation*PI/180);
                    bounce_ray.direction.y+=sin(y_rotation*PI/180);
                    bounce_ray.direction.z+=sin(z_rotation*PI/180);
                    if (bounce_ray.direction.dot(n)>=0){
                      bounce_ray.direction = bounce_ray.direction.normalize();
                      //cout << intersection_point.display()+"+"+bounce_ray.direction.display()+"t" << endl;
                      bounce_ray.max_bounces=bounces;
                      bounce_ray.bounces=bounces;
                      diffusion_intensity+=bounce_ray.project(meshes,light_sources);
                    }
                  }
                }
              }
              return diffusion_intensity;
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
          double scale = PI/(2*light_sources[i].radius);
          return (double)(cos(scale*dist));
        }
      }
    }
    return 0.0;
  }
};

class Pixel{
public:
  Vector3 position;
  double grayscale_value = 0.0;
  void trace_ray(Vector3 direction,vector<Mesh> meshes, vector<LightSource> light_sources){
      Ray ray;
      ray.origin = position;
      ray.direction=direction;
      double ray_intensity=ray.project(meshes,light_sources);
      grayscale_value+=ray_intensity;
  }
};

class Camera{
public:
  Vector3 direction;
  Vector3 position;
  double focal_length=3;
  int x_pixels=500;
  int y_pixels=500;
  double diagonal_size = 2;
  vector<vector<Pixel>> pixels;
  void initialize_pixels(){
    pixels.reserve(x_pixels);
    for (int i=0;i<x_pixels;i++){
      pixels.push_back(vector<Pixel>());
      pixels[i].reserve(y_pixels);
    }
    double pixel_size = (diagonal_size*pow(pow(x_pixels,2)+pow(y_pixels,2),.5))
          / (pow(x_pixels,2)+pow(y_pixels,2));
    Vector3 global_vertical;
    global_vertical.x=0;
    global_vertical.y=0;
    global_vertical.z=1;
    Vector3 viewport_horizontal = global_vertical.cross(direction).normalize();
    Vector3 viewport_vertical = direction.cross(viewport_horizontal).normalize();
    int x_offset = -1*x_pixels/2;
    int y_offset = -1*y_pixels/2;
    double scale_x = PI/x_pixels;
    double scale_y = PI/y_pixels;
    for (int x=0;x<x_pixels;x++){
      for (int y=0;y<y_pixels;y++){
        Pixel pixel;
        pixel.position = position+(direction*focal_length)
          +(viewport_horizontal*(x+x_offset)*pixel_size)
          +(viewport_vertical*(y+y_offset)*pixel_size);
        pixels[x].push_back(pixel);
      }
    }
    cout << "PIXELS INITIALIZED" << endl;
  }
  void sample(vector<Mesh> meshes, vector<LightSource> light_sources){
    int old_percent=0;
    for (int x=0;x<x_pixels;x++){
      int percent = 100*(x*y_pixels)/(x_pixels*y_pixels);
      if (percent != old_percent){
        cout << 100*(x*y_pixels)/(x_pixels*y_pixels) << "%" << endl;
        old_percent=100*(x*y_pixels)/(x_pixels*y_pixels);
      }
      for (int y=0;y<y_pixels;y++){
        Vector3 ray_direction=(pixels[x][y].position-position).normalize();
        pixels[x][y].trace_ray(ray_direction,meshes,light_sources);
      }
    }
  }
};
