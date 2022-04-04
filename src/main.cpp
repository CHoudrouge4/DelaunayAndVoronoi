/*
* Author: Nicolas Bourbaki
*
*/
#include <iostream>
#include <vector>
#include "triangulation.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <fstream>

std::vector<double> generate_points(const int n, double lower, double upper) {
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(lower, upper);
  std::vector<double> points(2 * n);
  for (size_t i = 0; i < points.size(); ++i)
    points[i] = dis(gen);
  return points;
}

void experiment( ) {
  std::ofstream out("results.out", std::ios_base::app);

  for (int i = 10000; i < 100000; i += 100) {
    auto pts = generate_points(i, -100000, 100000);
    triangulation t(pts);


    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    t.compute_delaunay();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    out << i << ' ' << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
    std::cout << i << ' ' << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
  }


  out.close();
  std::cout << "Done Writing File" << std::endl;
}


void display_exp() {
  auto pts = generate_points(4, -300.0, 300.0);
  // std::vector<double> pts = {3.0, +2.0, 1, 1, 1.5, 1.5};
  triangulation h(pts);

  // for (int i = 0; i < pts.size(); i += 2) {
  //  if(i == h.get_p_0()) continue;
  //  h.add_point(i);
  //  }
  //

  h.compute_delaunay();

  auto triangles = h.get_triangles();
  for (int i = 0; i < triangles.size(); ++i) {
    std::cout << "T: " << triangles[i]->vtx[0] << ' ' << triangles[i]->vtx[1] << ' ' << triangles[i]->vtx[2] << std::endl;
    auto p = h.circumcircle(triangles[i]->vtx[0], triangles[i]->vtx[1], triangles[i]->vtx[2]);
  }

  sf::RenderWindow window(sf::VideoMode(1000, 1000), "Delaunay Triangulation");
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Transparent);

   // set a 10-pixel wide orange outline
  shape.setOutlineThickness(1);
  shape.setOutlineColor(sf::Color(250, 150, 100));
  while (window.isOpen()) {
     sf::Event event;
     while (window.pollEvent(event)) {
       if (event.type == sf::Event::Closed) {
         window.close();
       }

       window.clear();
       window.draw(shape);
       //
       int scale = 1;
       int translate = 400;
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

       window.display();
     }
   }
}

int main () {

  experiment();
  //display_exp();
  return 0;
}
