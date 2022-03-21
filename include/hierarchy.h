#include <vector>

struct triangle {
  int a;
  int b;
  int c;

  std::vector<triangle*> children;

};

class hierarchy {
  triangle* root;
  std::vector<double> points;
  int P_0;
public:

  void init_root(const int p_0);
  void get_points(std::vector<double> & pts);

  triangle* locate_point(int p, triangle* root);
  bool is_inside_triangle(const int a, const int b, const int c, const int p);
  bool is_above(const int a, const int b, const int p);
  bool is_lexico_larger(const int p, const int q);

  double cross(const int a, const int b, const int c, const int d);
  double orient(const int a, const int b, const int c);
  void add_point(const int p);

  void get_triangles(triangle * root, std::vector<triangle*> &results);
  std::vector<triangle*> get_triangles();
};
