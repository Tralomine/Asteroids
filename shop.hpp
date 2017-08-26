#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "consts.hpp"

class Button : public sf::Drawable
{
  public:
    Button(sf::IntRect button, sf::Color fill, sf::Color outline, std::string text, int tSize, sf::Font &font);
    Button(sf::Vector2u pos, sf::Vector2u size);

    bool isClicked(const sf::Event& event, const sf::RenderTarget& target) const;
    void updateHover(sf::RenderWindow &window);
    void setColor(sf::Color fillColor, sf::Color outlineColor);

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::IntRect m_rectangle;
    sf::Text m_text;
    sf::Color m_fillColor, m_borderColor;
    bool m_isHovered;
};

class ShopUpgrade : public sf::Drawable{
  public:
    ShopUpgrade(int maxLevel, int basePrice, sf::Vector2u pos, sf::Texture& texture, sf::IntRect textureRect, float priceMultiplier, const std::string desc, sf::Font &font);

    void update(unsigned int& money, sf::RenderWindow& window);
    void buy(unsigned int& money);
    bool canBuy(const unsigned int& money) const;
    bool tryBuy(const sf::Event &event, const sf::RenderWindow& window) const;
    unsigned int getLevel() const;
    void unlock(unsigned int nbr);

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Vector2f m_pos;
    sf::Sprite m_texture;
    Button m_buy;
    unsigned int m_maxLevel;
    unsigned int m_basePrice;
    float m_priceMultiplier;
    unsigned int m_currentLevel;
    sf::Text m_description;
    sf::Color m_textColor;
    sf::Font &m_font;
};

class SwitchableShopUpgrade : public sf::Drawable{
  public:
    SwitchableShopUpgrade(int price, sf::Vector2u pos, sf::Texture& texture, sf::IntRect textureRect, const std::string desc, sf::Font &font);

    void update(unsigned int& money, sf::RenderWindow& window);
    void buy(unsigned int& money);
    bool canBuy(const unsigned int& money) const;
    bool tryBuy(const sf::Event &event, const sf::RenderWindow& window) const;
    void setActive(bool activity);
    bool isBuyed() const;
    void unlock(bool unlocked = true);
    void setPrice(int newPrice);
    void unBuy();

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Vector2f m_pos;
    sf::Sprite m_texture;
    Button m_button;
    unsigned int m_price;
    sf::Text m_description;
    sf::Color m_textColor;
    bool m_isBuyed;
    bool m_isActive;
    sf::Font &m_font;
};
