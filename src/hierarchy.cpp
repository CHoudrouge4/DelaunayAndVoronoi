#include "hierarchy.h"
#include <iostream>
#include <vector>

void hierarchy::init_root(const int p_0) {
  root.a = -2;
  root.b = -1;
  root.c = p_0;
  P_0 = p_0;
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

// is p < q
bool hierarchy::is_lexico_larger(const int p, const int q) {
  if(points[q] > points[p]) return true;
  else if (points[q] == points[p]) return (points[q + 1] > points[p + 1]);
  else false;
}

// equation of the line ab
bool hierarchy::is_above(const int a, const int b, const int p) {
  // add the case when one is horizontal and one is vertical
  if (points[a] == 0 && points[b] == 0) return (points[a] < points[p]);
  if (points[a + 1] == 0 && points[b + 1] == 0) return (points[a + 1] < points[p + 1]);

  double slope = (points[b + 1] - points[a + 1])/(points[b] - points[a]);
  double c = points[b + 1] - slope * points[b];
  return (slope * points[p] + c) > 0;
}

bool hierarchy::is_inside_triangle(const int a, const int b, const int c, const int p) {
  if (a == -2 && b == -1 && c == P_0) return true;
  if (a == -2 && b == -1) {
    if (is_lexico_larger(c, p)) return false;
    else return true;
  }

  if (a == -2 && c == P_0) {
    if (is_lexico_larger(b , p) && is_above(b, P_0, p)) return true;
    else return false;
  }

  if (b == -1 && c == P_0) {
    if (is_lexico_larger(a, p) && !is_above(a, P_0, p)) return true;
    else false;
  }
  std::cout << "Standard triangule test" << std::endl;
  if(is_above(a, b, p) && is_above(b, c, p) && !is_above(c, a, p)) return true;
  return false;
}

triangle* hierarchy::locate_point(int p, triangle* root) {
  // root.a root.b root.c
  // is inside triangle
}
