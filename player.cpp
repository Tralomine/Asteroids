#include "player.hpp"


Player::Player(const sf::Texture& texture, const sf::IntRect& textureRect):
m_position(400, 300),
m_spaceShipComp(texture, textureRect),
m_spaceshipUpdate(0),
m_maxLife(10),
m_life(10),
m_lastHit(0),
m_lastshot(0),
m_shotType(0),
m_shotSpeed(30),
m_fireType(0),
m_speed(1.0),
m_lifespan(0),
m_rotateSpeed(1.0),
m_magnetRange(32)
{
  m_sprite.setOrigin(16, 16);
  m_sprite.scale(2, 2);

  m_spaceShipTexture.create(32, 32);

  for(size_t i(0); i<8; i++){
    m_reactorComp[i].setTexture(texture);
    m_reactorComp[i].setTextureRect(sf::IntRect(32+ (i/4)*8, 64+ (i%4)*8, 8, 8));
  }
}

void Player::init(int life, int maxLife, int shotType, double shotSpeed, int fireType, double speed, double rotationSpeed, int magnetRange)
{
  m_life = life;
  m_maxLife = maxLife;
  m_shotType = shotType;
  m_shotSpeed = shotSpeed;
  m_fireType = fireType;
  m_speed = speed;
  m_rotateSpeed = rotationSpeed;
  m_magnetRange = magnetRange;
}

void Player::resetEffects()
{
  m_lastHit = 0;
  m_lastshot = 0;
  m_position = sf::Vector2f(400, 300);
}

void Player::move(const float& x, const float& y)
{
  m_position += sf::Vector2f(x, y);
}

void Player::move(const sf::Vector2f& pos)
{
  m_position += pos;
}

void Player::moveForward()
{
  this->move(m_speed*cos((m_rotation-90)*pi/180), m_speed*sin((m_rotation-90)*pi/180));
}
void Player::moveBackward()
{
  this->move(-0.5*m_speed*cos((m_rotation-90)*pi/180), -0.5*m_speed*sin((m_rotation-90)*pi/180));
}

void Player::setPosition(const sf::Vector2f& pos)
{
  m_position = pos;
}

sf::Vector2f Player::getPosition() const
{
  return m_position;
}

void Player::rotateLeft()
{
  m_rotation -= m_rotateSpeed;
}

void Player::rotateRight()
{
  m_rotation += m_rotateSpeed;
}

void Player::update()
{
  //rest in bounds
  if(m_position.x > 800) m_position += sf::Vector2f(-800, 0);
  if(m_position.x < 0) m_position += sf::Vector2f(800, 0);
  if(m_position.y > 600) m_position += sf::Vector2f(0, -600);
  if(m_position.y < 0) m_position += sf::Vector2f(0, 600);

  if(m_lastshot > 0) m_lastshot--;
  if(m_lastHit > 0) m_lastHit--;
  m_lifespan++;

  m_sprite.setColor(sf::Color(255, 255-(255/30.0*m_lastHit), 255-(255/30.0*m_lastHit)));
  m_sprite.setRotation(m_rotation);

  if(m_spaceshipUpdate-- == 0){
    m_spaceShipTexture.clear(sf::Color::Transparent);
    m_spaceShipTexture.draw(m_spaceShipComp);
    int i(rand()%8);
    m_reactorComp[i].setPosition(7, 24);
    m_spaceShipTexture.draw(m_reactorComp[i]);
    i = rand()%8;
    m_reactorComp[i].setPosition(17, 24);
    m_spaceShipTexture.draw(m_reactorComp[i]);
    m_spaceShipTexture.display();

    m_spaceshipUpdate = 4;
  }

  m_sprite.setTexture(m_spaceShipTexture.getTexture());
}

bool Player::canShot() const
{return m_lastshot == 0;}

bool Player::shot()
{
  if(this->canShot()){
    double shotMultiplier(1);
    if(m_shotType == 2) shotMultiplier *= 4;
    if(m_fireType == 1) shotMultiplier *= 2;
    if(m_fireType == 2) shotMultiplier /= 3;
    switch(m_fireType){
      case 0:
        Shot::shots.push_back({m_position, m_rotation, m_shotType});
        m_lastshot = m_shotSpeed * shotMultiplier;
        break;
      case 1:
        Shot::shots.push_back({m_position, m_rotation-20, m_shotType});
        Shot::shots.push_back({m_position, m_rotation, m_shotType});
        Shot::shots.push_back({m_position, m_rotation+20, m_shotType});
        m_lastshot = m_shotSpeed * shotMultiplier;
        break;
      case 2:
        Shot::shots.push_back({m_position, m_rotation+rand()%512/50.f -5.f, m_shotType});
        m_lastshot = m_shotSpeed * shotMultiplier;
      default:
        break;
    }
    return true;
  }
  return false;
}

void Player::setShotType(int shotType)
{
  m_shotType = shotType;
}

void Player::setFireType(int fireType)
{
  m_fireType = fireType;
}

bool Player::canBeHit() const
{
  return m_lastHit == 0;
}

bool Player::isHit(const Asteroid& astr)
{
  if(dist(m_position, astr.pos) < (astr.size+1) * 16 + 16){
    m_life--;
    m_lastHit = 30;
    return true;
  }else return false;
}

bool Player::isAlive() const
{
  return m_life > 0;
}

void Player::setLife(int life)
{
  m_life = life;
}

int Player::getLife() const
{
  return m_life;
}

int Player::getMaxLife() const
{
  return m_maxLife;
}

void Player::setMaxLife(int maxLife)
{
  m_maxLife = maxLife;
}

void Player::setShotSpeed(double newSpeed)
{
  m_shotSpeed = newSpeed;
}

double Player::getShotSpeed() const
{
  return m_shotSpeed;
}

void Player::setSpeed(float newSpeed)
{
  m_speed = newSpeed;
}

double Player::getSpeed() const
{
  return (double)m_speed;
}

void Player::setRotationSpeed(float newSpeed)
{
	m_rotateSpeed = newSpeed;
}

double Player::getRotationSpeed() const
{
  return (double)m_rotateSpeed;
}

int Player::getMagnetRange() const
{
  return m_magnetRange;
}

void Player::setMagnetRange(int newRange)
{
  m_magnetRange = newRange;
}

int Player::getShotType() const
{
  return m_shotType;
}

int Player::getFireType() const
{
  return m_fireType;
}


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  sf::Sprite temp(m_sprite);
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      temp.setPosition(m_position.x + i*800, m_position.y + j*600);
      target.draw(temp);
    }
  }
}
