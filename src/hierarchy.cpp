#include "hierarchy.h"
#include <iostream>
#include <vector>
#include <limits>
#include <cassert>

void hierarchy::init_root(const int p_0) {
  root = new triangle();
  root->a = -2;
  root->b = -1;
  root->c = p_0;
  P_0 = p_0;
  std::cout << "Root: " << root->a << " " << root->b << " " << root->c << '\n';
}

double hierarchy::cross(const int a, const int b, const  int c, const int d) {
  double x_ab = points[b] - points[a];
  double y_ab = points[b + 1] - points[a + 1];

  double x_cd = points[d] - points[c];
  double y_cd = points[d + 1] - points[c + 1];

  return x_ab * y_cd - y_ab * x_cd;
}

double hierarchy::orient(const int a, const int b, const int c) {
  return cross(a, b, a, c);
}

// might need optimisation
void hierarchy::add_points(std::vector<double> &pts) {
  points = pts;
  std::cout << "printing hierarchy points" << std::endl;
  for (size_t i = 0; i < points.size(); ++i) {
    std::cout << points[i] << '\n';
  }
}

// is q > p
bool hierarchy::is_lexico_larger(const int p, const int q) {
  //std::cout << "Test lexicographic order" << std::endl;
  //std::cout << points[q] << "?"  << points[p] << std::endl;
  if(points[q] > points[p]) return true;
  else if (std::abs(points[q] - points[p]) < std::numeric_limits<double>::epsilon() && std::abs(points[p] - points[q]) < std::numeric_limits<double>::epsilon() ) {
    //std::cout << "test equality!!" << std::endl;
    return (points[q + 1] > points[p + 1]);
  } else return false;
}

// equation of the line ab
bool hierarchy::is_above(const int a, const int b, const int p) {
  // add the case when one is horizontal and one is vertical
  // if (points[a] == 0 && points[b] == 0) return (points[a] < points[p]);
  // if (points[a + 1] == 0 && points[b + 1] == 0) return (points[a + 1] < points[p + 1]);
  //
  // double slope = (points[b + 1] - points[a + 1])/(points[b] - points[a]);
  // double c = points[b + 1] - slope * points[b];
  // return (slope * points[p] + c) > 0;
  return orient(a, b, p) > 0;
}

bool hierarchy::is_inside_triangle(const int a, const int b, const int c, const int p) {
  //tested
  if (a == -2 && b == -1 && c == P_0) return true; // here the points might be outside the triangle and this will return true. It is not an issue for now
  // tested
  if (a == -2 && b == -1) {
    if (is_lexico_larger(p, c)) return true;
    else return false;
  }

  // tested but it needs more testing
  if (a == -2 && c == P_0) {
    if (is_lexico_larger(b , p) && is_above(b, P_0, p)) return true;
    else return false;
  }

  // testing, What I am doing here
  if (b == -1 && c == P_0) {
    if (!is_lexico_larger(a, p) && is_above(P_0, a, p) && is_lexico_larger(p, P_0)) return true;
    else return false;
  }
  // tested
  std::cout << "Standard triangule test" << std::endl;
  if(is_above(a, b, p) && is_above(b, c, p) && is_above(c, a, p)) return true;
  return false;
}

triangle* hierarchy::locate_point(int p, triangle* root) {
  // root.a root.b root.c
  // is inside triangle
  std::cout << "P: " << p << std::endl;
  std::cout << "Locate Point: " << root->a << " " << root->b << " " << root->c << std::endl;
  if (is_inside_triangle(root->a, root->b, root->c, p)) {

    std::cout << "root size: " << root->children.size() << std::endl;
    if (root->children.size() == 0) return root;
    std::cout << "first triangle" << root->children[0]->a << " " << root->children[0]->b << " " << root->children[0]->c << std::endl;
    if(is_inside_triangle(root->children[0]->a, root->children[0]->b, root->children[0]->c, p)) {

        return locate_point(p, root->children[0]);
    }

    std::cout << "second triangle" << root->children[1]->a << " " << root->children[1]->b << " " << root->children[1]->c << std::endl;
    if(is_inside_triangle(root->children[1]->a, root->children[1]->b, root->children[1]->c, p)) {
        return locate_point(p, root->children[1]);
    }

    std::cout << "third triangle" << root->children[2]->a << " " << root->children[2]->b << " " << root->children[2]->c << std::endl;
    if(is_inside_triangle(root->children[2]->a, root->children[2]->b, root->children[2]->c, p)) {
      return locate_point(p, root->children[2]);
    }
  }
  return nullptr;
}

void hierarchy::add_point(const int p) {
  triangle* t = locate_point(p, root);
  if (t == nullptr) std::cout << "nullptr" << std::endl;
  else std::cout << "we found t" << t->a << ' ' << t->b << ' ' << t->c << std::endl;
  triangle* first = new triangle();
  first->a = t->a;
  first->b = t->b;
  first->c = p;

  t->children.push_back(first);

  triangle* second =  new triangle();
  second->a = p;
  second->b = t->b;
  second->c = t->c;

  t->children.push_back(second);

  triangle* third = new triangle();
  third->a = t->a;
  third->b = p;
  third->c = t->c;

  t->children.push_back(third);
}

//we will optimise in the future
void hierarchy::get_triangles(triangle * root, std::vector<triangle*> &results) {
  if (root == nullptr) return;
  if (root->children.size() == 0) results.push_back(root);
  else {
    for(int i = 0; i < root->children.size(); ++i) {
        get_triangles(root->children[i], results);
    }
  }
}

//
std::vector<triangle*> hierarchy::get_triangles() {
  std::vector<triangle*> results;
  get_triangles(root, results);
  return results;
}

point hierarchy::mid_point(const int a, const int b) const {
  double mx = (points[a] + points[b])/2;
  double my = (points[a + 1] + points[b + 1])/2;
  return {mx, my};
}

double hierarchy::cross(const double vx, const double vy, const  int c) const {
  return vx * points[c + 1] - vy * points[c];
}

double hierarchy::cross_d(const double vx, const double vy, const  double cx, const double cy) const {
  return vx * cy - vy * cx;
}

point hierarchy::circumcircle(const int a, const int b, const int c) const {
  auto mid_ab = mid_point(a, b);
  auto mid_ac = mid_point(a, c);

  // double slope_mid_ab = (-1) * (points[b] - points[a])/(points[b + 1] - points[a + 1]);
  // double b_ab = mid_ab.y - slope_mid_ab * mid_ab.x;
  //
  // double slope_mid_ac = (-1) * (points[c] - points[a])/(points[c + 1] - points[a + 1]);
  // double b_ac = mid_ac.y - slope_mid_ac * mid_ac.x;
  // line ab
  double vx = points[b] - points[a];
  double vy = points[b + 1] - points[a + 1];
  double cab = cross(vx, vy, a);

  // line perp to ab
  double pvx = -vy;
  double pvy = vx;
  double cpv = cross_d(pvx, pvy, mid_ab.x, mid_ab.y);

  // line by ac
  double wx = points[c] - points[a];
  double wy = points[c + 1] - points[a + 1];
  double cac = cross(wx, wy, a);

  // line perp to ac
  double pwx = -wy;
  double pwy = wx;
  double cpw = cross_d(pwx, pwy, mid_ac.x, mid_ac.y);

  double d = cross_d(pvx, pvy, pwx, pwy);

  double inter_x = (cpv * pwx - cpw * pvx) / d;
  double inter_y = (cpv * pwy - cpw * pvy) / d;
  return {inter_x,  inter_y};
}

// we have triangle abc with circumcircle computed we want to check if the point d is legal
bool hierarchy::is_legal(const int a, const int b, const int c, const int d) const {
  auto center = circumcircle(a, b, c);
  double dx = (points[a] - center.x);
  double dy = (points[a + 1] - center.y);
  double raduis = dx*dx + dy * dy;

  double dxc = (points[d] - center.x);
  double dyc = (points[d + 1] - center.y);
  double distance = dxc * dxc + dyc * dyc;

  return (raduis < distance);
}
