#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "functions.hpp"
#include "consts.hpp"

struct Triangle{
  sf::Vector2f points[3];
};

class Clickable{
  public:
    virtual void click() = 0;
  private:
    std::vector<Triangle> triangles;
};

class Colidable{
  public:
    virtual void colide(Colidable);
  private:
};

struct Asteroid{
  sf::Vector2f pos;
  sf::Vector2f v;
  float r;
  float vr;
  int size;
  int sprite;
  int life;
  bool golden;
};

Asteroid createAsteroid(sf::Vector2f pos, sf::Vector2f v, int size, bool golden);

class Shot : public sf::Drawable{
  public:
    Shot(sf::Vector2f pos, float r, int type);

    void move(const sf::Vector2f& offset);
    void move(const float& offsetX, const float& offsetY);
    void moveForward(const float& offset);
    void moveToAst(std::vector<Asteroid> asteroids, float offset);

    sf::Vector2f getPosition();
    int getType();
    bool canTouch();
    void setLastHit();
    void update();

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Vector2f m_pos;
    float m_rotation;
    int m_type;
    int m_lasthit;

  public:
    static std::vector<Shot> shots;

    static void erase(size_t index);

    static int colide(Asteroid astr);

  	static sf::Texture *m_texture;
  	static sf::IntRect m_missileRect;

  	static void loadMissileTexture(sf::Texture *text, sf::IntRect rect);
};

struct Coin{
  sf::Vector2f pos;
  int animLevel;
  int lifespan;
  int valueLevel;
};
