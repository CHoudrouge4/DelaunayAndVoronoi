#include <vector>
#include <string>
#include "hierarchy.h"

typedef std::pair<double, double> edge;

class triangulation {

private:
  int n;
  int dimension = 2;
  int p_0;
  std::vector<double> points;
  std::vector<edge> edges;
  hierarchy h;
  void read_file(const std::string & file_name);
  void find_p_zero();

  // Boundary edges

public:
  triangulation(const std::string &file_name);
  triangulation(const std::vector<double> & pts);
  void compute_convex_hull(); // compute the Boundary edges // I do not need this, I can just add three extra points
  void triangulate();
  void compute_delaunay();


  std::vector<double> get_points() const;
  std::vector<edge> get_edges() const;
};
