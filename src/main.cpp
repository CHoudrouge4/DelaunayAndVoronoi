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

  auto pts = generate_points(10, -100.0, 100.0);
  //std::vector<double> pts = {0.0, 0.0, 100, -100, 0, 100, 25, 50, 50, 200, 30, 30, 200, -100};
  //std::vector<double> pts = {-37.0752, 38.7575, 78.0524,30.3614, 4.0041,-30.7034, -8.72772, -28.2299};
  triangulation h(pts);
  for (int i = 0; i < pts.size(); i += 2) {
     if(i == h.get_p_0()) continue;
     h.add_point(i);
  }

  //h.compute_delaunay();

  auto triangles = h.get_triangles();
  for (int i = 0; i < triangles.size(); ++i) {
    std::cout << "T: " << pts[triangles[i]->vtx[0]] << ' '<< pts[triangles[i]->vtx[0] + 1]
    << ' ' << pts[triangles[i]->vtx[1]] << ' ' << pts[triangles[i]->vtx[1] + 1]
    << ' ' << pts[triangles[i]->vtx[2]] << ' ' << pts[triangles[i]->vtx[2] + 1] << std::endl;
    auto p = h.circumcircle(triangles[i]->vtx[0], triangles[i]->vtx[1], triangles[i]->vtx[2]);
    std::cout << "center of T: " << p.x << ' ' << p.y << std::endl;
    // for (int j = 0; j < 3; ++j)
    //   if (triangles[i]->op[j] != nullptr) {
    //     std::cout << "ad T: " << triangles[i]->op[j]->vtx[0] << ' ' << triangles[i]->op[j]->vtx[1] << ' ' << triangles[i]->op[j]->vtx[2] << std::endl;
    //   }
  }

  std::cout << "how many triangles? " << triangles.size() << '\n';
  // std::cout << "is in triangle: " << h.is_inside_triangle(4, -1, 2, 6) << std::endl;
  // std::cout << "is in triangle: " << h.is_inside_triangle(4, -1, 2, 8) << std::endl;
  //std::cout << "is above test: " << h.is_above(0, 4, 6) << std::endl;
  //triangulation t(pts);

   sf::RenderWindow window(sf::VideoMode(1000, 1000), "Delaunay Triangulation");
   sf::CircleShape shape(100.f);
   shape.setFillColor(sf::Color::Transparent);


   // set a 10-pixel wide orange outline
  shape.setOutlineThickness(1);
  shape.setOutlineColor(sf::Color(250, 150, 100));
   while (window.isOpen()) {
     sf::Event event;
     while (window.pollEvent(event)) {
       if (event.type == sf::Event::Closed)
         window.close();
       }

       window.clear();
       window.draw(shape);
       //
       int scale = 1;
       int translate = 200;
       for (int i = 0; i < triangles.size(); ++i) {
         int a = triangles[i]->vtx[0];
         int b = triangles[i]->vtx[ccw(0)];
         int c = triangles[i]->vtx[cw(0)];
         auto p = h.circumcircle(a, b, c);
         double dx = (p.x * scale - pts[b] * scale);
         double dy = (p.y * scale - pts[b + 1] * scale);
         double radius = std::sqrt(dx*dx + dy * dy);
         shape.setRadius(radius);
         shape.setPosition(p.x * scale + translate - radius, p.y*scale + translate - radius);
         window.draw(shape);
         if(a == -2) continue;
         if(b == -1) continue;
           sf::Vertex line1[] =
            {
                sf::Vertex(sf::Vector2f(pts[a]*scale + translate, pts[a + 1]*scale + translate)),
                sf::Vertex(sf::Vector2f(pts[b]*scale + translate, pts[b + 1]*scale + translate))
            };

            window.draw(line1, 2, sf::Lines);
            sf::Vertex line2[] =
             {
                 sf::Vertex(sf::Vector2f(pts[b]*scale + translate, pts[b + 1]*scale + translate)),
                 sf::Vertex(sf::Vector2f(pts[c]*scale + translate, pts[c + 1]*scale + translate))
             };

             window.draw(line2, 2, sf::Lines);
             sf::Vertex line3[] =
              {
                  sf::Vertex(sf::Vector2f(pts[c]*scale + translate, pts[c + 1]*scale + translate)),
                  sf::Vertex(sf::Vector2f(pts[a]*scale + translate, pts[a + 1]*scale + translate))
              };

            window.draw(line3, 2, sf::Lines);
       }
       // draw the triangles

       window.display();
   }

  return 0;
}
