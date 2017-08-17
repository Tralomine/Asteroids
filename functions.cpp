#include "functions.hpp"

void drawBounds(const sf::Text& drawable, sf::RenderTarget& target){
  sf::RectangleShape rectangle(sf::Vector2f(drawable.getGlobalBounds().width, drawable.getGlobalBounds().height));
  rectangle.setPosition(drawable.getGlobalBounds().left, drawable.getGlobalBounds().top);
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(1);
  target.draw(rectangle);
}

float dist(sf::Vector2f pos1, sf::Vector2f pos2)
{
  return std::sqrt((pos2.x-pos1.x)*(pos2.x-pos1.x) + (pos2.y-pos1.y)*(pos2.y-pos1.y));
}

sf::Vector2f normalize(sf::Vector2f v)
{
  return v / dist(sf::Vector2f(0, 0), v);
}

void resetView(sf::RenderTarget& target, sf::Sprite& backgroundTexture)
{
  //définition des vues, une première qui fait tout pour mettre un fond si le jeux prend pas tout l'écran
  sf::View background(sf::FloatRect(0, 0, target.getSize().x, target.getSize().y));
  background.setViewport(sf::FloatRect(0, 0, 1, 1));
  target.setView(background);
  target.clear();
  backgroundTexture.setTextureRect(sf::IntRect(0, 0, target.getSize().x, target.getSize().y));
  target.draw(backgroundTexture);

  //et un deuxieme qui permet de faire en sorte que l'écran reste bien aux mêmes proportions, même si la taille de la fenêtre est changé
  sf::View view(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
  if(target.getSize().x/4.0 > target.getSize().y/3.0){
    view.setViewport(sf::FloatRect((target.getSize().x-((4/3.0)*target.getSize().y))/(2*target.getSize().x), 0, ((4/3.0)*target.getSize().y)/target.getSize().x, 1));
  }else{
    view.setViewport(sf::FloatRect(0, (target.getSize().y-(0.75*target.getSize().x))/(2*target.getSize().y), 1, (0.75*target.getSize().x)/target.getSize().y));
  }
  target.setView(view);

  //redessine un fond noir
  sf::RectangleShape black;
  black.setSize(sf::Vector2f(800, 600));
  black.setFillColor(sf::Color::Black);
  target.draw(black);
}
