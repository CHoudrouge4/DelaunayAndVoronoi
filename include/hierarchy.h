#include <vector>

struct triangle {
  int a;
  int b;
  int c;

  std::vector<triangle*> children;

};

class hierarchy {
  triangle root;
  std::vector<double> points;
public:

  void init_root(const int p_0);
  void get_points(std::vector<double> & pts);
};
