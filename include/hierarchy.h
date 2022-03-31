// #include <vector>
// #define x first
// #define y second
//
// typedef std::pair<double, double> point;
//
// struct triangle {
//   int a;
//   int b;
//   int c;
//   triangle * ab;
//   triangle * bc;
//   triangle * ca;
//   std::vector<triangle*> children;
// };
//
// class hierarchy {
//   triangle* root;
//   std::vector<double> points;
//   int P_0;
// public:
//
//   void init_root(const int p_0);
//   void add_points(std::vector<double> & pts);
//
//   triangle* locate_point(int p, triangle* root);
//   bool is_inside_triangle(const int a, const int b, const int c, const int p);
//   bool is_above(const int a, const int b, const int p);
//   bool is_lexico_larger(const int p, const int q);
//
//   double cross(const int a, const int b, const int c, const int d);
//   double orient(const int a, const int b, const int c);
//   triangle* add_point(const int p);
//
//   void get_triangles(triangle * root, std::vector<triangle*> &results);
//   point mid_point(const int a, const int b) const;
//   std::vector<triangle*> get_triangles();
//
//   double cross(const double vx, const double vy, const  int c) const;
//   double cross_d(const double vx, const double vy, const  double cx, const double cy) const;
//   point circumcircle(const int a, const int b, const int c) const;
//   bool is_legal(const int a, const int b, const int c, const int d) const;
// };
