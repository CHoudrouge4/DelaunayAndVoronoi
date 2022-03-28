/*
* Author: Nicolas Bourbaki
*
*/
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
  std::vector<double> pts = {0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.25, 0.5, 0.5, 0.2, 0.3, 0.3, 0.6, 0.2};
  hierarchy h;
  h.init_root(2);
  h.add_points(pts);

  h.add_point(0);
  h.add_point(4);
  h.add_point(6);
  h.add_point(8);
  h.add_point(10);
  h.add_point(12);


  auto triangles = h.get_triangles();

  // std::cout << "is in triangle: " << h.is_inside_triangle(4, -1, 2, 6) << std::endl;
  // std::cout << "is in triangle: " << h.is_inside_triangle(4, -1, 2, 8) << std::endl;

  //std::cout << "is above test: " << h.is_above(0, 4, 6) << std::endl;
  //triangulation t(pts);

   sf::RenderWindow window(sf::VideoMode(1000, 1000), "Delaunay Triangulation");
   sf::CircleShape shape(100.f);
   //shape.setFillColor(sf::Color::Green);
   while (window.isOpen()) {
     sf::Event event;
     while (window.pollEvent(event)) {
       if (event.type == sf::Event::Closed)
         window.close();
       }

       window.clear();
      // window.draw(shape);
       //
       int scale = 100;
       for (int i = 0; i < triangles.size(); ++i) {
         int a = triangles[i]->a;
         int b = triangles[i]->b;
         int c = triangles[i]->c;
         if(a == -2) continue;
         if(b == -1) continue;
           sf::Vertex line1[] =
            {
                sf::Vertex(sf::Vector2f(pts[a]*scale + 100, pts[a + 1]*scale + 100)),
                sf::Vertex(sf::Vector2f(pts[b]*scale + 100, pts[b + 1]*scale + 100))
            };

            window.draw(line1, 2, sf::Lines);
            sf::Vertex line2[] =
             {
                 sf::Vertex(sf::Vector2f(pts[b]*scale + 100, pts[b + 1]*scale + 100)),
                 sf::Vertex(sf::Vector2f(pts[c]*scale + 100, pts[c + 1]*scale + 100))
             };

             window.draw(line2, 2, sf::Lines);
             sf::Vertex line3[] =
              {
                  sf::Vertex(sf::Vector2f(pts[c]*scale + 100, pts[c + 1]*scale + 100)),
                  sf::Vertex(sf::Vector2f(pts[a]*scale + 100, pts[a + 1]*scale + 100))
              };

            window.draw(line3, 2, sf::Lines);
       }
       // draw the triangles

       window.display();
   }

  return 0;
}
