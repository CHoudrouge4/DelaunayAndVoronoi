#include <vector>

typedef std::pair<double, double> edge;

class triangulation {

private:
  int n;
  std::vector<double> points;
  std::vector<edge> edges;
  // Boundary edges

public:

  void compute_convex_hull(); // compute the Boundary edges
  void triangulate();
  void compute_delaunay();
  

  std::vector<double> get_points() const;
  std::vector<edge> get_edges() const;
};
