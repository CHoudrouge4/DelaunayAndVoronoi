#include "triangulation.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <limits>
#include <cassert>
#include <set>
#include <memory>

void print_triangle(std::shared_ptr<triangle> t, std::string s) {
  std::cout << s << ' ' << t->vtx[0] << ' ' << t->vtx[ccw(0)] << ' ' << t->vtx[cw(0)] << std::endl;
}

int cw(const int i) { return (i + 2) % 3; }

int ccw(const int i) { return (i + 1) % 3; }

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

void triangulation::init_root() {
  root = std::make_shared<triangle>();
  root->vtx[0] = -2;
  root->vtx[ccw(0)] = -1;
  root->vtx[cw(0)] = p_0;
  root->op[0] = nullptr;
  root->op[ccw(0)] = nullptr;
  root->op[cw(0)] = nullptr;
}

triangulation::triangulation(const std::vector<double> & pts) {
  points = pts;
  for (size_t i = 0; i < points.size(); ++i) {
    std::cout << points[i] << std::endl;
  }
  find_p_zero();
  std::cout << "p_0 = (" << points[p_0] << ", " << points[p_0 + 1] << ")\n";
  init_root();
}

double triangulation::cross(const int a, const int b, const  int c, const int d) const {
  double x_ab = points[b] - points[a];
  double y_ab = points[b + 1] - points[a + 1];

  double x_cd = points[d] - points[c];
  double y_cd = points[d + 1] - points[c + 1];

  return x_ab * y_cd - y_ab * x_cd;
}

double triangulation::orient(const int a, const int b, const int c) const {
  return cross(a, b, a, c);
}

// is q > p
bool triangulation::is_lexico_larger(const int p, const int q) const {
  if(points[q] > points[p]) return true;
  else if (std::abs(points[q] - points[p]) < std::numeric_limits<double>::epsilon() && std::abs(points[p] - points[q]) < std::numeric_limits<double>::epsilon() ) {
    return (points[q + 1] > points[p + 1]);
  } else return false;
}

// equation of the line ab
bool triangulation::is_above(const int a, const int b, const int p) const {
  return orient(a, b, p) > 0;
}

bool triangulation::is_inside_triangle(const int a, const int b, const int c, const int p) {

  if (a == -2 && b == -1 && c == p_0) return true;

  if (a == -2 && b == -1) {
     if (is_lexico_larger(c , p)) return false;
     else return true;
  }

  if (a == -2) {
    if (is_lexico_larger(c, p)) return false;
    else if (is_above(c, b, p)) return false;
    else return true;
  }

  if (b == -1) {
    if (is_above(a, c, p)) return false;
    else if (is_lexico_larger(c, p)) return false;
    else if (is_lexico_larger(a ,p)) return true;
    else return false;
  }

  if(is_above(a, b, p) && is_above(b, c, p) && is_above(c, a, p)) return true;
  return false;
}

void swap(int &a, int &b) {
  int tmp = a;
  a = b;
  b = tmp;
}

std::shared_ptr<triangle> triangulation::locate_point(const int p, std::shared_ptr<triangle> root) {

  int a = root->vtx[0];
  int b = root->vtx[ccw(0)];
  int c = root->vtx[cw(0)];
  if (b == -2) {
    swap(a, b);
    swap(b, c);
  } else if (c == -2) {
    std::cout << "yes: c = -2 " <<std::endl;
    swap(a, c);
    swap(b, c);
  }

  if (a == -1) {
    swap(a, b);
    swap(a, c);
  } else if (c == -1) {
    swap(b, c);
    swap(c, a);
  }
  std::cout << "a " << a << " b: " << b << " c: " << c << std::endl;
  if (is_inside_triangle(a, b, c, p)) {

    if (root->children.size() == 0) return root;

    for(int i = 0; i < root->children.size(); ++i) {
      print_triangle(root->children[i], "testing triangle " + std::to_string(i));
      int a = root->children[i]->vtx[0];
      int b = root->children[i]->vtx[ccw(0)];
      int c = root->children[i]->vtx[cw(0)];
      if (b == -2) {
        swap(a, b);
        swap(b, c);
      } else if (c == -2) {
        std::cout << "yes: c = -2 " <<std::endl;
        swap(a, c);
        swap(b, c);
      }

      if (a == -1) {
        swap(a, b);
        swap(a, c);
      } else if (c == -1) {
        swap(b, c);
        swap(c, a);
      }
      std::cout << "a: " << a << " b: " << b << " c: " << c << std::endl;
      if(is_inside_triangle(a, b, c, p)) {
          return locate_point(p, root->children[i]);
      }
    }
  }
  return nullptr;
}

int get_oposite(std::shared_ptr<triangle> t, int a, int b) {
  std::cout << "a: " << a << " b: " << b << std::endl;
  int op = 0;
  while (a == t->vtx[op] || b ==  t->vtx[op]) op = cw(op);
  return op;
}

// it needs more checking
std::shared_ptr<triangle> triangulation::add_point(const int p) {
  std::cout << "adding " << p << std::endl;

  std::shared_ptr<triangle> t = locate_point(p, root);
  if (t == nullptr) std::cout << "nullptr" << std::endl;
  else std::cout << "we found t" << t->vtx[0] << ' ' << t->vtx[ccw(0)] << ' ' << t->vtx[cw(0)] << std::endl;

  // first triangle
  std::shared_ptr<triangle> first = std::make_shared<triangle>();
  first->vtx[0] = t->vtx[0];
  first->vtx[ccw(0)] = t->vtx[ccw(0)];
  first->vtx[cw(0)] = p;

  print_triangle(first, "first");
  t->children.push_back(first);

  // second triangle
  std::shared_ptr<triangle> second =  std::make_shared<triangle>();
  second->vtx[0] = p;
  second->vtx[ccw(0)] = t->vtx[ccw(0)];
  second->vtx[cw(0)] = t->vtx[cw(0)];

  print_triangle(second, "second");
  t->children.push_back(second);

  // third triangle
  std::shared_ptr<triangle> third = std::make_shared<triangle>();
  third->vtx[0] = t->vtx[0];
  third->vtx[ccw(0)] = p;
  third->vtx[cw(0)] = t->vtx[cw(0)];
  t->children.push_back(third);
  print_triangle(third, "third");

  first->op[0] = second;
  first->op[ccw(0)] = third;
  first->op[cw(0)] = t->op[cw(0)];
  if (t->op[cw(0)] != nullptr) {
   int op_index = get_oposite(t->op[cw(0)], first->vtx[0], first->vtx[ccw(0)]);
   t->op[cw(0)]->op[op_index] = first;
  }

  second->op[0] = t->op[0];
  second->op[ccw(0)] = third;
  second->op[cw(0)] = first;
  if (t->op[0] != nullptr) {
    int op_index = get_oposite(t->op[0], second->vtx[ccw(0)], second->vtx[cw(0)]);
    t->op[0]->op[op_index] = second;
  }

  third->op[0] = second;
  third->op[ccw(0)] = t->op[ccw(0)];
  third->op[cw(0)] = first;
  if (t->op[ccw(0)] != nullptr) {
    int op_index = get_oposite(t->op[ccw(0)], third->vtx[cw(0)], third->vtx[0]);
    t->op[ccw(0)]->op[op_index] = third;
  }
  return t;
}

int triangle_type(const int a, const int b, const int c) {
  int count = 0;
  if (a < 0) count++;
  if (b < 0) count++;
  if (c < 0) count++;
  return count;
}

bool is_boundary_triangle(const int a, const int b, const int c) {
  int t = triangle_type(a, b, c);
  return (t == 2);
}

//we will optimise in the future
void triangulation::get_triangles(std::shared_ptr<triangle> root, std::vector<std::shared_ptr<triangle>> &results) {
  if (root == nullptr) return;
  if (root->children.size() == 0 && triangle_type(root->vtx[0], root->vtx[1], root->vtx[2]) == 0) {
    results.push_back(root);
  } else {
    for(int i = 0; i < root->children.size(); ++i) {
        get_triangles(root->children[i], results);
    }
  }
}

//
std::vector<std::shared_ptr<triangle>> triangulation::get_triangles() {
  std::vector<std::shared_ptr<triangle>> results;
  get_triangles(root, results);
  return results;
}

point triangulation::mid_point(const int a, const int b) const {
  double mx = (points[a] + points[b])/2;
  double my = (points[a + 1] + points[b + 1])/2;
  return {mx, my};
}

double triangulation::cross(const double vx, const double vy, const  int c) const {
  return vx * points[c + 1] - vy * points[c];
}

double triangulation::cross_d(const double vx, const double vy, const  double cx, const double cy) const {
  return vx * cy - vy * cx;
}

point triangulation::circumcircle(const int a, const int b, const int c) const {
  auto mid_ab = mid_point(a, b);
  auto mid_ac = mid_point(a, c);

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
// a is oposite to d
bool triangulation::is_legal(const int a, const int b, const int c, const int d) const {

  int t = triangle_type(a, b, c);
  int s = triangle_type(b, c, d);
  if (t == 1 || s == 1) {
      return std::min(a, d) < std::min(b, c);
  }

  auto center = circumcircle(a, b, c);
  double dx = (points[a] - center.x);
  double dy = (points[a + 1] - center.y);
  double raduis = dx*dx + dy * dy;

  double dxc = (points[d] - center.x);
  double dyc = (points[d + 1] - center.y);
  double distance = dxc * dxc + dyc * dyc;
  std::cout << "raduis: " << raduis << ' ' << distance << std::endl;
  return (raduis < distance);
}

bool triangulation::legalizable(int p, std::shared_ptr<triangle> s, std::shared_ptr<triangle> t) {
  if (s == nullptr || t == nullptr) return false;
  if (!is_convex(p, s, t)) return false;

  int s_type = triangle_type(s->vtx[0], s->vtx[1], s->vtx[2]);
  int t_type = triangle_type(t->vtx[0], t->vtx[1], t->vtx[2]);

  if ((s_type == 1 && t_type == 0) || (s_type == 0 && t_type == 1)) return true;
  if (s_type == 1 && t_type == 1) {
      int neg = 0;
      for (int i = 0; i < 3; ++i) {
        if (s->vtx[i] < 0) neg = s->vtx[i];
      }
      for (int i = 0; i < 3; ++i) {
        if (t->vtx[i] == neg) return true;
      }
      return false;
  }
  return (s_type == 0) && (t_type == 0);
}

void triangulation::compute_delaunay() {
  for (size_t i = 0; i < points.size(); i += 2) {
     if (i == p_0) continue;
     std::shared_ptr<triangle> t = add_point(i);
     std::shared_ptr<triangle> first = t->children[0];
     if (first->op[cw(0)] != nullptr && legalizable(cw(0), first, first->op[cw(0)])) {
      legalize(cw(0), first, first->op[cw(0)]);
     }
    std::shared_ptr<triangle> second = t->children[1];
    // print_triangle(second->op[0], "op of second ");
     if (second->op[0] != nullptr && legalizable(0, second, second->op[0]))
       legalize(0, second, second->op[0]);

    std::shared_ptr<triangle> third = t->children[2];
    if(third->op[ccw(0)] != nullptr && legalizable(ccw(0), third, third->op[ccw(0)]))
      legalize(ccw(0), third, third->op[ccw(0)]);
   }
 }



// p_index in s, it is 0 (second), cw(0) first, or ccw(0) third
void triangulation::legalize(int p_index, std::shared_ptr<triangle> s, std::shared_ptr<triangle> t) {
  std::cout << "legalizing" << std::endl;
  print_triangle(s, "left legal");
  print_triangle(t, "right legal");
  int op_index = get_oposite(t, s->vtx[cw(p_index)], s->vtx[ccw(p_index)]);
  std::cout << "p_ index is " << p_index << ' ' << s->vtx[p_index] << std::endl;
  std::cout << "we get the opposite index " << op_index << ' ' << t->vtx[op_index] << std::endl;
  if(!is_legal(s->vtx[p_index], s->vtx[cw(p_index)], s->vtx[ccw(p_index)], t->vtx[op_index])) {
    // create two new triangles
    std::shared_ptr<triangle> r1 = std::make_shared<triangle>();
    r1->vtx[0] = s->vtx[p_index];
    r1->vtx[cw(0)] = s->vtx[cw(p_index)];
    r1->vtx[ccw(0)] = t->vtx[op_index];

    std::shared_ptr<triangle> r2 = std::make_shared<triangle>();
    r2->vtx[0] = s->vtx[p_index];
    r2->vtx[cw(0)] = t->vtx[op_index];
    r2->vtx[ccw(0)] = s->vtx[ccw(p_index)];

    r1->op[0] = t->op[cw(op_index)];
    r1->op[cw(0)] = r2;
    r1->op[ccw(0)] = s->op[ccw(p_index)];
    if (r1->op[0] != nullptr) {
       int op_index = get_oposite(r1->op[0], r1->vtx[cw(0)], r1->vtx[ccw(0)]);
       r1->op[0]->op[op_index] = r1;
    }
    if(r1->op[ccw(0)] != nullptr) {
       int op_index = get_oposite(r1->op[ccw(0)], r1->vtx[0], r1->vtx[cw(0)]);
       r1->op[ccw(0)]->op[op_index] = r1;
    }


    r2->op[0] = t->op[ccw(op_index)];
    r2->op[cw(0)] = s->op[cw(p_index)];
    r2->op[ccw(0)] = r1;
    if(r2->op[0] != nullptr) {
      int op_index = get_oposite(r2->op[0], r2->vtx[cw(0)], r2->vtx[ccw(0)]);
      r2->op[0]->op[op_index] = r2;
    }
    if(r2->op[cw(0)] != nullptr) {
      int op_index = get_oposite(r2->op[cw(0)], r2->vtx[0], r2->vtx[ccw(0)]);
      r2->op[cw(0)]->op[op_index] = r2;
    }


    s->children.push_back(r1);
    s->children.push_back(r2);
    t->children.push_back(r1);
    t->children.push_back(r2);

    print_triangle(r1, "r1");
//  print_triangle(t->op[ccw(op_index)], "r1 0 op");
    print_triangle(r2, "r2");

    if(legalizable(0, r1, r1->op[0]))
      legalize(0, r1, r1->op[0]);
    if (legalizable(0, r2, r2->op[0]))
      legalize(0, r2, r2->op[0]);
  }
}

int triangulation::get_p_0() {
  return p_0;
}

// quadrilateur abcd
bool triangulation::is_convex(int a, int b, int c, int d) {
  return is_above(a, b, c) && is_above(a, b, d) && is_above(b, c, d) && is_above(b, c, d) && is_above(c, d, a) && is_above(c, d, b) && is_above(d, a, b) && is_above(d, a, c);
}

bool triangulation::is_convex(int p, std::shared_ptr<triangle>  s, std::shared_ptr<triangle> t) {

  int s_type = triangle_type(s->vtx[0], s->vtx[1], s->vtx[2]);
  int t_type = triangle_type(t->vtx[0], t->vtx[1], t->vtx[2]);

  if (s_type == 1 || t_type == 1) {
    if (s->vtx[p] < 0) return true;
    int op_index = get_oposite(t, s->vtx[cw(p)], s->vtx[ccw(p)]);
    if(t->vtx[op_index] < 0) return true;
    if (s->vtx[ccw(p)] == -1) {
      return is_above(s->vtx[p], t->vtx[op_index], s->vtx[cw(p)]);
    } else if (s->vtx[cw(p)] == -1) {
      return !is_above(s->vtx[p], t->vtx[op_index], s->vtx[ccw(p)]);
    } else if (s->vtx[ccw(p)] == -2) {
      return is_above(s->vtx[p], t->vtx[op_index], s->vtx[cw(p)]);
    } else {
      return !is_above(s->vtx[p], t->vtx[op_index], s->vtx[ccw(p)]);
    }
  }

  if (s_type == 0 && t_type == 0) {
    int op_index = get_oposite(t, s->vtx[cw(p)], s->vtx[ccw(p)]);
    return is_convex(s->vtx[p], s->vtx[ccw(p)], t->vtx[op_index], s->vtx[cw(p)]);
  }
  return false;
}

// void triangulation::clean (triangle * root, triangle* t1, triangle * t2, triangle *t3) {
//   //
//   // if (root->children.size() == 0) {
//   //   delete root->op[0];
//   //   delete root->op[1];
//   //   delete root->op[2];
//   //   for (int i = 0; i < root->children.size(); ++i) {
//   //     delete root->children[i];
//   //   }
//   // }
//
//   if (t1.children.size() == 0) {
//     free(t1)
//   }
//
//   for (int i = 0; i < root->children.size(); ++i) {
//     clean(root->children[i]);
//   }
//
//   if (root->children.size() == 0) {
//     free(root);
//   }
//
// }
//
// triangulation::~triangulation() {
//   clean(root);
// }
