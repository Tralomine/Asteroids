#include "objects.hpp"
#include <cmath>

Asteroid createAsteroid(sf::Vector2f pos, sf::Vector2f v, int size, bool golden)
{
  Asteroid temp;
  temp.pos = pos;
  temp.size = size;
  temp.v = v;
  temp.vr = rand()%256/128.0 - 1;
  temp.r = 0;
  switch (size) {
    case 0:
    temp.sprite = rand()%8;
    temp.life = 1;
    break;
    case 1:
    temp.sprite = rand()%4;
    temp.life = 4;
    break;
    case 2:
    temp.life = 8;
  }
  temp.golden = golden;
  return temp;
}

Shot::Shot(sf::Vector2f pos, float r, int type):
m_pos(pos),
m_rotation(r-90),
m_type(type),
m_lasthit(0)
{}

void Shot::move(const sf::Vector2f& offset)
{
  m_pos += offset;
}

void Shot::move(const float& offsetX, const float& offsetY)
{
  m_pos += sf::Vector2f(offsetX, offsetY);
}

void Shot::moveForward(const float& offset)
{
  this->move(offset * cosf(m_rotation*pi/180), offset * sinf(m_rotation*pi/180));
}

void Shot::moveToAst(std::vector<Asteroid> asteroids, float offset)
{
  int min = 2147483647;
  int indexMin = -1;
  for (size_t i = 0; i < asteroids.size(); i++) {
    if(dist(asteroids[i].pos, m_pos) - asteroids[i].size * 32 < min){
      min = dist(asteroids[i].pos, m_pos) - asteroids[i].size * 32;
      indexMin = i;
    }
  }
  if(indexMin >= 0){
    sf::Vector2f temp({0, 0});
    temp += sf::Vector2f(cosf(m_rotation*pi/180), sinf(m_rotation*pi/180));
    temp += (10.f/(float)dist(asteroids[indexMin].pos, m_pos)) * normalize(asteroids[indexMin].pos - m_pos);
    temp = normalize(temp);
    if(asinf(temp.y) > 0)
      m_rotation = acosf(temp.x) / pi * 180;
    else
      m_rotation = 180-(acosf(temp.x) / pi * 180) + 180;
    temp *= offset;
    this->move(temp);
  }else{
    this->moveForward(offset);
  }
}

sf::Vector2f Shot::getPosition()
{
  return m_pos;
}

int Shot::getType()
{
  return m_type;
}

bool Shot::canTouch()
{
  return m_lasthit == 0;
}

void Shot::setLastHit()
{
  m_lasthit = 10;
}

void Shot::update()
{
  if(m_lasthit>0)
    m_lasthit --;
}


void Shot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  sf::RectangleShape tx;
  tx.setRotation(m_rotation);
  tx.setPosition(m_pos);
  sf::Sprite sp(*m_texture, m_missileRect);
  sp.setRotation(m_rotation + 90);
  sp.setPosition(m_pos);
  sp.setOrigin(2, 4);
  sp.scale(2, 2);
  switch(m_type){
    case 0:
      tx.setFillColor(sf::Color(255, 255, 0));
      tx.setSize(sf::Vector2f(2, 2));
      tx.setOrigin(1, 1);
      break;
    case 1:
      tx.setFillColor(sf::Color(192, 192, 255));
      tx.setSize(sf::Vector2f(8, 2));
      tx.setOrigin(4, 1);
      break;
    case 2:
      target.draw(sp);
      break;
  }
  target.draw(tx);
}

void Shot::erase(size_t index)
{
  if(index < shots.size())
    shots.erase(shots.cbegin()+index);
}

int Shot::colide(Asteroid astr)
{
  int hitbox(0);
  if(astr.size < 2) hitbox = (astr.size+1) *16;
  if(astr.size == 2) hitbox = 64;
  for (size_t i = 0; i < shots.size(); i++) {
    if(dist(astr.pos, shots[i].getPosition()) < hitbox){
      return i;
    }
  }
  return -1;
}

void Shot::loadMissileTexture(sf::Texture *text, sf::IntRect rect)
{
	m_texture = text;
	m_missileRect = rect;
}

std::vector<Shot> Shot::shots({});
sf::Texture *Shot::m_texture(0);
sf::IntRect Shot::m_missileRect({});
