#include <vector>
#include <string>

using namespace std;

class Mesh{
public:
  string name;
  vector<vector<double>> vertices;
  vector<vector<double>> normal_vectors;
  vector<vector<vector<int>>> faces;
};
