#include "world-objects.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

vector<Object> parse_objects(string f_path){
  ifstream input;
  input.open(f_path);
  if (input.is_open()){
    char char_buf;
    while (!input.eof()){
      input >> char_buf;
      cout << char_buf;
    }
  }
  input.close();
  vector<Object> ret_vec;
  return ret_vec;
}
