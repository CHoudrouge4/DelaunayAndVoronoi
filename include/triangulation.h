#include <vector>
#include <string>
#define x first
#define y second

int cw(const int i);
int ccw(const int i);

// #include "hierarchy.h"
typedef std::pair<double, double> edge;
typedef std::pair<double, double> point;

struct triangle {
  std::vector<int> vtx;
  std::vector<triangle*> op;
  std::vector<triangle*> children;
  triangle() {
    vtx = std::vector<int>(3);
    op = std::vector<triangle*>(3);
  }

};


class triangulation {

private:
  int n;
  int dimension = 2;
  int p_0;
  std::vector<double> points;
  std::vector<edge> edges;
//  hierarchy h;
  void read_file(const std::string & file_name);
  void find_p_zero();
  triangle* root;
  // Boundary edges

public:
  triangulation(const std::string &file_name);
  triangulation(const std::vector<double> & pts);


  std::vector<double> get_points() const;
  std::vector<edge> get_edges() const;
  void init_root();
  double cross(const int a, const int b, const  int c, const int d) const;
  double orient(const int a, const int b, const int c) const;
  bool is_lexico_larger(const int p, const int q) const;
  bool is_above(const int a, const int b, const int p) const;
  bool is_inside_triangle(const int a, const int b, const int c, const int p);
  triangle* locate_point(const int p, triangle* root);
  triangle* add_point(const int p);
  void get_triangles(triangle * root, std::vector<triangle*> &results);
  std::vector<triangle*> get_triangles();
  point mid_point(const int a, const int b) const;
  double cross(const double vx, const double vy, const  int c) const;
  double cross_d(const double vx, const double vy, const  double cx, const double cy) const;
  point circumcircle(const int a, const int b, const int c) const;
  bool is_legal(const int a, const int b, const int c, const int d) const;
  void compute_delaunay();
  void legalize(int p_index, triangle* s, triangle* t);
  bool legalizable(int p, triangle * s, triangle *t);
  int get_p_0();
  bool is_convex(int a, int b, int c, int d);
  bool is_convex(int p, triangle*  s, triangle * t);
};
