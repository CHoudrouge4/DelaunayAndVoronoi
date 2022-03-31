#include "triangulation.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <limits>
#include <cassert>
#include <set>

int cw(const int i) {
  return (i + 2) % 3;
}

int ccw(const int i) {
  return (i + 1) % 3;
}

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
  root = new triangle();
  root->vtx[0] = -2;
  root->vtx[ccw(0)] = -1;
  root->vtx[cw(0)] = p_0;
  root->op[0] = nullptr;
  root->op[ccw(0)] = nullptr;
  root->op[cw(0)] = nullptr;

  std::cout << "Root: " << root->vtx[0] << " " << root->vtx[ccw(0)] << " " << root->vtx[cw(0)] << '\n';
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
  //std::cout << "Test lexicographic order" << std::endl;
  //std::cout << points[q] << "?"  << points[p] << std::endl;
  if(points[q] > points[p]) return true;
  else if (std::abs(points[q] - points[p]) < std::numeric_limits<double>::epsilon() && std::abs(points[p] - points[q]) < std::numeric_limits<double>::epsilon() ) {
    //std::cout << "test equality!!" << std::endl;
    return (points[q + 1] > points[p + 1]);
  } else return false;
}

// equation of the line ab
bool triangulation::is_above(const int a, const int b, const int p) const {
  return orient(a, b, p) > 0;
}

bool triangulation::is_inside_triangle(const int a, const int b, const int c, const int p) {
  //tested
  if (a == -2 && b == -1 && c == p_0) return true; // here the points might be outside the triangle and this will return true. It is not an issue for now
  // tested

  // I think it is correct
  if (a == -2 && b == -1) {
     if (is_lexico_larger(p, c)) return true;
     else return false;
  }

  // tested but it needs more testing
  if (a == -2 && c == p_0) {
    if (is_lexico_larger(b , p) && is_above(b, p_0, p)) return true;
    else return false;
  }

  // testing, What I am doing here
  if (b == -1 && c == p_0) {
    std::cout << "left of AP? " << is_above(p_0, a, p) << std::endl;
    if (is_lexico_larger(a, p) && is_above(p_0, a, p) && is_lexico_larger(p, p_0)) return true;
    else return false;
  }
  // tested
  std::cout << "Standard triangule test" << std::endl;
  if(is_above(a, b, p) && is_above(b, c, p) && is_above(c, a, p)) return true;
  return false;
}

triangle* triangulation::locate_point(const int p, triangle* root) {
  // root.a root.b root.c
  // is inside triangle
  std::cout << "P: " << p << std::endl;
  std::cout << "Locate Point: " << root->vtx[0] << " " << root->vtx[ccw(0)] << " " << root->vtx[cw(0)] << std::endl;
  if (is_inside_triangle(root->vtx[0], root->vtx[ccw(0)], root->vtx[cw(0)], p)) {

    std::cout << "root size: " << root->children.size() << std::endl;
    if (root->children.size() == 0) return root;


    for(int i = 0; i < root->children.size(); ++i) {
      std::cout << "first triangle" << root->children[i]->vtx[0] << " " << root->children[i]->vtx[ccw(i)] << " " << root->children[i]->vtx[cw(0)] << std::endl;
      if(is_inside_triangle(root->children[i]->vtx[0], root->children[i]->vtx[ccw(0)], root->children[i]->vtx[cw(0)], p)) {
          return locate_point(p, root->children[i]);
      }
    }
    /*
    * I need to fix this one urgently
    * because now not all the vertices has degree two outgoing edges
    */


    // std::cout << "second triangle" << root->children[1]->vtx[0] << " " << root->children[1]->vtx[ccw(0)] << " " << root->children[1]->vtx[cw(0)] << std::endl;
    // if(is_inside_triangle(root->children[1]->vtx[0], root->children[1]->vtx[ccw(0)], root->children[1]->vtx[cw(0)], p)) {
    //     return locate_point(p, root->children[1]);
    // }
    //
    // std::cout << "third triangle" << root->children[2]->vtx[0] << " " << root->children[2]->vtx[ccw(0)] << " " << root->children[2]->vtx[cw(0)] << std::endl;
    // if(is_inside_triangle(root->children[2]->vtx[0], root->children[2]->vtx[ccw(0)], root->children[2]->vtx[cw(0)], p)) {
    //   return locate_point(p, root->children[2]);
    // }
  }
  return nullptr;
}

triangle* triangulation::add_point(const int p) {
  if (p == p_0) return root;
  triangle* t = locate_point(p, root);
  if (t == nullptr) std::cout << "nullptr" << std::endl;
  else std::cout << "we found t" << t->vtx[0] << ' ' << t->vtx[ccw(0)] << ' ' << t->vtx[cw(0)] << std::endl;
  // first triangle
  triangle* first = new triangle();
  first->vtx[0] = t->vtx[0];
  first->vtx[ccw(0)] = t->vtx[ccw(0)];
  first->vtx[cw(0)] = p;

  t->children.push_back(first);

  // second triangle
  triangle* second =  new triangle();
  second->vtx[0] = p;
  second->vtx[ccw(0)] = t->vtx[ccw(0)];
  second->vtx[cw(0)] = t->vtx[cw(0)];

  t->children.push_back(second);

  // third triangle
  triangle* third = new triangle();
  third->vtx[0] = t->vtx[0];
  third->vtx[ccw(0)] = p;
  third->vtx[cw(0)] = t->vtx[cw(0)];
  t->children.push_back(third);


  first->op[0] = second;
  first->op[ccw(0)] = third;
  first->op[cw(0)] = root->op[cw(0)];

  second->op[0] = root->op[0];
  second->op[ccw(0)] = third;
  second->op[cw(0)] = first;

  third->op[0] = second;
  third->op[ccw(0)] = root->op[ccw(0)];
  third->op[cw(0)] = first;

  return t;
}

//we will optimise in the future
void triangulation::get_triangles(triangle * root, std::vector<triangle*> &results) {
  if (root == nullptr) return;
  if (root->children.size() == 0) {
    results.push_back(root);
  } else {
    for(int i = 0; i < root->children.size(); ++i) {
        get_triangles(root->children[i], results);
    }
  }
}

//
std::vector<triangle*> triangulation::get_triangles() {
  std::vector<triangle*> results;
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
// a is oposite to d
bool triangulation::is_legal(const int a, const int b, const int c, const int d) const {
  auto center = circumcircle(a, b, c);
  double dx = (points[a] - center.x);
  double dy = (points[a + 1] - center.y);
  double raduis = dx*dx + dy * dy;

  double dxc = (points[d] - center.x);
  double dyc = (points[d + 1] - center.y);
  double distance = dxc * dxc + dyc * dyc;

  return (raduis < distance);
}

void triangulation::compute_delaunay() {
  for (size_t i = 0; i < points.size(); i += 2) {
     if (i == p_0) continue;
     triangle * t = add_point(i);
     triangle * first = t->children[0];

     if (first->op[cw(0)] != nullptr)
      legalize(cw(0), first, first->op[cw(0)]);

     triangle * second = t->children[1];
     if (second->op[0] != nullptr)
       legalize(0, second, second->op[0]);

    triangle * third = t->children[2];
    if(third->op[ccw(0)] != nullptr)
      legalize(ccw(0), third, third->op[ccw(0)]);
   }
 }

int get_oposite(triangle * t, int a, int b) {
  int op = 0;
  while (a == t->vtx[op] || b ==  t->vtx[op]) op = cw(0);
  return op;
}

// p_index in s, it is 0 (second), cw(0) first, or ccw(0) third
void triangulation::legalize(int p_index, triangle* s, triangle* t) {
  int op_index = get_oposite(t, s->vtx[cw(p_index)], s->vtx[ccw(p_index)]);
  if(!is_legal(s->vtx[p_index], s->vtx[cw(p_index)], s->vtx[ccw(p_index)], t->vtx[op_index])) {
    // create two new triangles
    triangle * r1 = new triangle();
    r1->vtx[0] = s->vtx[p_index];
    r1->vtx[cw(0)] = s->vtx[cw(p_index)];
    r1->vtx[ccw(0)] = t->vtx[op_index];

    triangle * r2 = new triangle();
    r2->vtx[0] = s->vtx[p_index];
    r2->vtx[cw(0)] = t->vtx[op_index];
    r2->vtx[ccw(0)] = s->vtx[ccw(p_index)];

    r1->op[0] = t->op[cw(op_index)];
    r1->op[cw(0)] = r2;
    r1->op[ccw(0)] = s->op[ccw(p_index)];

    r2->op[0] = t->op[ccw(op_index)];
    r2->op[cw(0)] = s->op[cw(p_index)];
    r2->op[ccw(0)] = r1;

    s->children.push_back(r1);
    s->children.push_back(r2);
    t->children.push_back(r1);
    t->children.push_back(r2);
    legalize(0, r1, r1->op[0]);
    legalize(0, r2, r2->op[0]);
  }
}
