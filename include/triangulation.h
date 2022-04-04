#include <vector>
#include <string>
#include <memory>
#define x first
#define y second

int cw(const int i);
int ccw(const int i);

// #include "hierarchy.h"
typedef std::pair<double, double> edge;
typedef std::pair<double, double> point;

struct triangle {

  std::vector<int> vtx;
  std::vector<std::shared_ptr<triangle>> op;
  std::vector<std::shared_ptr<triangle>> children;
  triangle() {
    vtx = std::vector<int>(3);
    op = std::vector<std::shared_ptr<triangle>>(3);
  }

  // ~triangle() {
  //
  //   // for(int i = 0; i < children.size(); i++) {
  //   //   if(children[i] != nullptr)
  //   //     free(children[i]);
  //   // }
  // }
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
  std::shared_ptr<triangle> root;
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
  std::shared_ptr<triangle> locate_point(const int p, std::shared_ptr<triangle> root);
  std::shared_ptr<triangle> add_point(const int p);
  void get_triangles(std::shared_ptr<triangle> root, std::vector<std::shared_ptr<triangle>> &results);
  std::vector<std::shared_ptr<triangle>> get_triangles();
  point mid_point(const int a, const int b) const;
  double cross(const double vx, const double vy, const  int c) const;
  double cross_d(const double vx, const double vy, const  double cx, const double cy) const;
  point circumcircle(const int a, const int b, const int c) const;
  bool is_legal(const int a, const int b, const int c, const int d) const;
  void compute_delaunay();
  void legalize(int p_index, std::shared_ptr<triangle> s, std::shared_ptr<triangle> t);
  bool legalizable(int p, std::shared_ptr<triangle> s, std::shared_ptr<triangle> t);
  int get_p_0();
  bool is_convex(int a, int b, int c, int d);
  bool is_convex(int p, std::shared_ptr<triangle> s, std::shared_ptr<triangle> t);
  //~triangulation();
  void clean(std::shared_ptr<triangle> root);
  std::string points_to_string(int p);
};
