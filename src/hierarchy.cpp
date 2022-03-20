#include "hierarchy.h"
#include <iostream>
#include <vector>

void hierarchy::init_root(const int p_0) {
  root.a = -2;
  root.b = -1;
  root.c = p_0;

  std::cout << "Root: " << root.a << " " << root.b << " " << root.c << '\n';
}

// might need optimisation
void hierarchy::get_points(std::vector<double> &pts) {
  points = pts;
  std::cout << "printing hierarchy points" << std::endl;
  for (size_t i = 0; i < points.size(); ++i) {
    std::cout << points[i] << '\n';
  }
}
