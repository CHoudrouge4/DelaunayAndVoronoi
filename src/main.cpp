#include <iostream>
#include <vector>
#include "triangulation.h"
#include <SFML/Graphics.hpp>
#include <random>

std::vector<double> generate_points(const int n, double lower, double upper) {
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(lower, upper);
  std::vector<double> points(2 * n);
  for (size_t i = 0; i < points.size(); ++i)
    points[i] = dis(gen);
  return points;
}

int main () {
  // std::string file_name = "test.in";
  // triangulation t(file_name);

//  auto pts = generate_points(10, -10.0, 10.0);
  std::vector<double> pts = {0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.25, 0.5, 0.5, 0.7};
  hierarchy h;
  h.init_root(2);
  h.get_points(pts);

  std::cout << "is in triangle: " << h.is_inside_triangle(4, -1, 2, 6) << std::endl;
  std::cout << "is in triangle: " << h.is_inside_triangle(4, -1, 2, 8) << std::endl;

  std::cout << "is above test: " << h.is_above(0, 4, 6) << std::endl;
  //triangulation t(pts);

   sf::RenderWindow window(sf::VideoMode(1000, 1000), "Delaunay Triangulation");
   sf::CircleShape shape(100.f);
   shape.setFillColor(sf::Color::Green);
   while (window.isOpen()) {
     sf::Event event;
     while (window.pollEvent(event)) {
       if (event.type == sf::Event::Closed)
         window.close();
       }

       window.clear();
       window.draw(shape);
       // draw the triangles

       window.display();
   }

  return 0;
}
