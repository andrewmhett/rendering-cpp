#include <vector>
#include <math.h>

using namespace std;

class Vector3{
public:
  double x;
  double y;
  double z;
  string display(){
    return "("+to_string(x)+", "+to_string(y)+", "+to_string(z)+")";
  }
  double dot(Vector3 comp_vec){
    return (comp_vec.x*x)
    +(comp_vec.y*y)
    +(comp_vec.z*z);
  }
  Vector3 cross(Vector3 comp_vec){
    Vector3 ret_vector;
    ret_vector.x = y*comp_vec.z-z*comp_vec.y;
    ret_vector.y = z*comp_vec.x-x*comp_vec.z;
    ret_vector.z = x*comp_vec.y-y*comp_vec.x;
    return ret_vector;
  }
  Vector3 normalize(){
    Vector3 ret_vector;
    ret_vector.x=x/magnitude();
    ret_vector.y=y/magnitude();
    ret_vector.z=z/magnitude();
    return ret_vector;
  }
  double dist(Vector3 comp_vec){
    Vector3 delta_vector;
    delta_vector.x=x-comp_vec.x;
    delta_vector.y=y-comp_vec.y;
    delta_vector.z=z-comp_vec.z;
    return pow(pow(delta_vector.x,2)+pow(delta_vector.y,2)+pow(delta_vector.z,2),0.5);
  }
  double magnitude(){
    return pow(pow(x,2)+pow(y,2)+pow(z,2),0.5);
  }
  Vector3 operator+(const Vector3& v) {
    Vector3 ret_vector;
    ret_vector.x=v.x+x;
    ret_vector.y=v.y+y;
    ret_vector.z=v.z+z;
    return ret_vector;
  }
  void operator+=(const Vector3& v){
    x+=v.x;
    y+=v.y;
    z+=v.z;
  }
  Vector3 operator*(double d) {
    Vector3 ret_vector;
    ret_vector.x=d*x;
    ret_vector.y=d*y;
    ret_vector.z=d*z;
    return ret_vector;
  }
  Vector3 operator/(Vector3 v){
    Vector3 ret_vector;
    ret_vector.x=x/v.x;
    if (v.x==0){
      ret_vector.x=0;
    }
    ret_vector.y=y/v.y;
    if (v.y==0){
      ret_vector.y=0;
    }
    ret_vector.z=z/v.z;
    if (v.z==0){
      ret_vector.z=0;
    }
    return ret_vector;
  }
  Vector3 operator-(const Vector3& v) {
    Vector3 ret_vector;
    ret_vector.x=x-v.x;
    ret_vector.y=y-v.y;
    ret_vector.z=z-v.z;
    return ret_vector;
  }
  bool operator==(const Vector3& v){
    bool ret_bool = true;
    if (x != v.x){
      ret_bool=false;
    }
    if (y != v.y){
      ret_bool=false;
    }
    if (z != v.z){
      ret_bool=false;
    }
    return ret_bool;
  }
  bool operator!=(const Vector3& v){
    Vector3 self_vec;
    self_vec.x=x;
    self_vec.y=y;
    self_vec.z=z;
    return !(self_vec==v);
  }
};
