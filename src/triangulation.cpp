#include "triangulation.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

void triangulation::find_p_zero() {
    p_0 = 0;
    for (int i = 2; i < points.size(); i += 2) {
      if (points[i] > points[p_0]) {
        p_0 = i;
      } else if (points[i] == points[p_0]) {
        if (points[i + 1] > points[p_0 + 1]) p_0 = i;
      }
    }
}

void triangulation::read_file(const std::string & file_name) {
  std::ifstream in(file_name);

  if (!in.good()) {
        std::cout << "#1. Opening file" << file_name << " failed - "
                     "one of the error flags is true\n";
        return;
  }

  std::cout << "Reading File" << std::endl;
  int n;
  in >> n;
  int x;
  for (int i = 0; i < dimension * n; ++i) {
    in >> x;
    std::cout << x << '\n';
    points.push_back(x);
  }
  in.close();
  std::cout << "Done Reading File" << std::endl;
}

triangulation::triangulation(const std::string& file_name) {
  read_file(file_name);
  find_p_zero();
}

triangulation::triangulation(const std::vector<double> & pts): h() {
  points = pts;
  for (size_t i = 0; i < points.size(); ++i) {
    std::cout << points[i] << std::endl;
  }
  find_p_zero();
  std::cout << "P_0 = (" << points[p_0] << ", " << points[p_0 + 1] << ")\n";
  h.init_root(p_0);
  h.add_points(points);
}
