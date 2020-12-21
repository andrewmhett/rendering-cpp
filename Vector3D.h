#include <vector>
#include <math.h>

using namespace std;

class Vector3{
public:
  double x;
  double y;
  double z;
  double dot(Vector3 comp_vec){
    return (comp_vec.x*x)
    +(comp_vec.y*y)
    +(comp_vec.z*z);
  }
  double dist(Vector3 comp_vec){
    Vector3 delta_vector;
    delta_vector.x=x-comp_vec.x;
    delta_vector.y=y-comp_vec.y;
    delta_vector.z=z-comp_vec.z;
    return pow(pow(delta_vector.x,2)+pow(delta_vector.y,2)+pow(delta_vector.z,2),0.5);
  }
  Vector3 operator+(const Vector3& v) {
    Vector3 ret_vector;
    ret_vector.x=v.x+x;
    ret_vector.y=v.y+y;
    ret_vector.z=v.z+z;
    return ret_vector;
  }
  Vector3 operator*(double d) {
    Vector3 ret_vector;
    ret_vector.x=d*x;
    ret_vector.y=d*y;
    ret_vector.z=d*z;
    return ret_vector;
  }
  Vector3 operator-(const Vector3& v) {
    Vector3 ret_vector;
    ret_vector.x=x-v.x;
    ret_vector.y=y-v.y;
    ret_vector.z=z-v.z;
    return ret_vector;
  }
};
