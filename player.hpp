#pragma once

#include <SFML/Graphics.hpp>
#include "functions.hpp"
#include "objects.hpp"


class Player : public sf::Drawable
{
  public:
    Player(const sf::Texture& texture, const sf::IntRect& textureRect);

    void init(int life, int maxLife, int shotType, double shotSpeed, int fireType, double speed, double rotationSpeed, int magnetRange, int shieldMax, int shield);

    void move(const float& x, const float& y);
    void move(const sf::Vector2f& pos);
    void moveForward();
    void moveBackward();
    void rotateLeft();
    void rotateRight();

    void update();
    bool shot();
    void resetEffects();

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;
    bool canShot() const;
    void setShotType(int shotType);
    void setFireType(int fireType);
    bool canBeHit() const;
    bool isHit(const Asteroid& astr);
    bool takeDamage();
    bool isAlive() const;
    void setLife(int life);
    int getLife() const;
    int getMaxLife() const;
    void setMaxLife(int maxLife);
    void setShotSpeed(double newSpeed);
    double getShotSpeed() const;
    void setSpeed(float newSpeed);
    double getSpeed() const;
    void setRotationSpeed(float newSpeed);
    double getRotationSpeed() const;
    int getMagnetRange() const;
    void setMagnetRange(int newRange);
    int getShotType() const;
    int getFireType() const;
    void setShieldMax(int newShield);
    int getShieldMax() const;
    // void setShield(int newShield);
    int getShield() const;

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Vector2f m_position;
    float m_rotation;
    sf::Sprite m_sprite;
    sf::Sprite m_spaceShipComp, m_reactorComp[8];
    sf::RenderTexture m_spaceShipTexture;
    int m_reactorUpdate;
    int m_maxLife, m_life;
    int m_lastHit;
    int m_lastshot;
    int m_shotType;
    double m_shotSpeed;
    int m_fireType;
    double m_speed, m_rotateSpeed;
    int m_lifespan;
    int m_magnetRange;
    int m_shield, m_shieldMax;
    int m_shieldCooldown;
    int m_react1, m_react2;
};
