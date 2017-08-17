#include "shop.hpp"


Button::Button(sf::IntRect button, sf::Color fill, sf::Color outline, std::string text, int tSize, sf::Font &font):
m_rectangle(button),
m_text(text, font, tSize*2),
m_fillColor(fill),
m_borderColor(outline)
{
  m_text.scale(0.5, 0.5);
  m_text.setFillColor(outline);
  m_text.setOrigin(m_text.getGlobalBounds().width, 22);
  m_text.setPosition(m_rectangle.left + m_rectangle.width/2.0, m_rectangle.top + m_rectangle.height/2.0);
}

Button::Button(sf::Vector2u pos, sf::Vector2u size):
m_rectangle(sf::IntRect(pos.x, pos.y, size.x, size.y)),
m_fillColor(sf::Color(0, 192, 0)),
m_borderColor(sf::Color(0, 128, 0))
{}

bool Button::isClicked(const sf::Event& event, const sf::RenderTarget& target) const
{
  return m_rectangle.contains((sf::Vector2i)target.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
}

void Button::updateHover(sf::RenderWindow &window)
{
  m_isHovered = m_rectangle.contains((sf::Vector2i)window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}

void Button::setColor(sf::Color fillColor, sf::Color outlineColor)
{
  m_fillColor = fillColor;
  m_borderColor = outlineColor;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::RectangleShape rectangle(sf::Vector2f(m_rectangle.width -8, m_rectangle.height -8));
    rectangle.setPosition(m_rectangle.left +4, m_rectangle.top +4);
    rectangle.setFillColor(m_fillColor);
    rectangle.setOutlineColor(m_borderColor);
    if(m_isHovered){
      rectangle.setFillColor(m_fillColor + sf::Color(64, 64, 64));
      rectangle.setOutlineColor(m_borderColor + sf::Color(64, 64, 64));
    }
    rectangle.setOutlineThickness(4);
    target.draw(rectangle, states);
    target.draw(m_text, states);
}


ShopUpgrade::ShopUpgrade(int maxLevel, int basePrice, sf::Vector2u pos, sf::Texture& texture, sf::IntRect textureRect, float priceMultiplier, const std::string desc, sf::Font &font):
m_pos(pos),
m_texture(texture, textureRect),
m_buy(pos + sf::Vector2u(maxLevel*9 + 48, 0), {20, 20}),
m_maxLevel(maxLevel),
m_basePrice(basePrice),
m_priceMultiplier(priceMultiplier),
m_currentLevel(0),
m_description(desc, font, 24),
m_textColor(sf::Color::Red),
m_font(font)
{
  m_texture.scale(2.f, 2.f);
  m_texture.setPosition(m_pos + sf::Vector2f(0, 1));
  m_description.scale(0.5, 0.5);
  m_description.setPosition(m_pos + sf::Vector2f(40, 22));
  m_description.setStyle(sf::Text::Style::Italic);
  m_description.setFillColor(sf::Color(128, 128, 128));
}

void ShopUpgrade::update(unsigned int& money, sf::RenderWindow& window)
{
  m_buy.updateHover(window);

  if(this->canBuy(money)){
    m_textColor = sf::Color::White;
  }else{
    m_textColor = sf::Color::Red;
  }
}

void ShopUpgrade::buy(unsigned int& money)
{
  money -= (int)(m_basePrice * pow(m_priceMultiplier, m_currentLevel));
  m_currentLevel++;
}

bool ShopUpgrade::canBuy(const unsigned int& money) const
{
  return money >= (unsigned int)(m_basePrice * pow(m_priceMultiplier, m_currentLevel)) && m_currentLevel < m_maxLevel;
}

bool ShopUpgrade::tryBuy(const sf::Event &event, const sf::RenderWindow& window) const
{
  return m_buy.isClicked(event, window);
}

unsigned int ShopUpgrade::getLevel() const
{
  return m_currentLevel;
}

void ShopUpgrade::unlock(unsigned int nbr)
{
  m_currentLevel = std::min(nbr, m_maxLevel);
}

void ShopUpgrade::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  sf::RectangleShape upgradePoint(sf::Vector2f(8, 14));
  upgradePoint.setPosition(m_pos+sf::Vector2f(40, 3));
  upgradePoint.setOutlineColor(sf::Color(0, 192, 0));
  upgradePoint.setOutlineThickness(2);
  upgradePoint.setFillColor(sf::Color(0, 255, 0));
  for (size_t i = 0; i < m_maxLevel; i++) {
    if(i >= m_currentLevel){
      upgradePoint.setFillColor(sf::Color(0, 128, 0));
    }
    target.draw(upgradePoint);
    upgradePoint.move( 9, 0);
  }
  target.draw(m_texture, states);

  sf::Text text(std::to_string((int)(m_basePrice * pow(m_priceMultiplier, m_currentLevel))) + "$", m_font, 36);
  text.setPosition(m_pos + sf::Vector2f(m_maxLevel*9 + 73, -2));
  text.setScale(0.5f, 0.5f);

  if(m_currentLevel < m_maxLevel){
    target.draw(m_buy, states);
    text.setFillColor(m_textColor);
  }else{
    text.setFillColor(sf::Color::Yellow);
    text.setString("MAX");
    text.setStyle(sf::Text::Style::Bold);
  }
  target.draw(text, states);
  target.draw(m_description);
}


SwitchableShopUpgrade::SwitchableShopUpgrade(int price, sf::Vector2u pos, sf::Texture& texture, sf::IntRect textureRect, const std::string desc, sf::Font &font):
m_pos(pos),
m_texture(texture, textureRect),
m_button(pos, {40, 40}),
m_price(price),
m_description(desc, font, 24),
m_textColor(sf::Color::Red),
m_isBuyed(false),
m_isActive(false),
m_font(font)
{
  m_texture.scale(2.f, 2.f);
  m_texture.setPosition(m_pos + sf::Vector2f(4, 4));
  m_description.scale(0.5, 0.5);
  m_description.setPosition(m_pos + sf::Vector2f(44, 22));
  m_description.setStyle(sf::Text::Style::Italic);
  m_description.setFillColor({128, 128, 128});
}

void SwitchableShopUpgrade::update(unsigned int& money, sf::RenderWindow& window)
{
  m_button.updateHover(window);

  if(this->canBuy(money)){
    m_textColor = sf::Color::White;
  }else{
    m_textColor = sf::Color::Red;
  }
  if(!m_isBuyed){
    m_button.setColor(sf::Color(0, 192, 0), sf::Color(0, 64, 0));
  }else if(m_isActive){
    m_button.setColor(sf::Color(255, 255, 0), sf::Color(128, 128, 0));
  }else{
    m_button.setColor(sf::Color(0, 0, 255), sf::Color(0, 0, 128));
  }
}

void SwitchableShopUpgrade::buy(unsigned int& money)
{
  money -= m_price;
  m_isBuyed = true;
}

bool SwitchableShopUpgrade::canBuy(const unsigned int& money) const
{
  return money >= m_price && !m_isBuyed;
}

bool SwitchableShopUpgrade::tryBuy(const sf::Event &event, const sf::RenderWindow& window) const
{
  return m_button.isClicked(event, window);
}

void SwitchableShopUpgrade::setActive(bool activity)
{
  m_isActive = activity;
}

bool SwitchableShopUpgrade::isBuyed() const
{
  return m_isBuyed;
}

void SwitchableShopUpgrade::unlock()
{
  m_isBuyed = true;
}

void SwitchableShopUpgrade::setPrice(int newPrice)
{
  m_price = newPrice;
}

void SwitchableShopUpgrade::unBuy()
{
  m_isBuyed = false;
}

void SwitchableShopUpgrade::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  sf::Text text(std::to_string(m_price) + "$", m_font, 36);
  text.setPosition(m_pos + sf::Vector2f(44, -2));
  text.setScale(0.5f, 0.5f);

  if(!m_isBuyed){
    text.setFillColor(m_textColor);
  }else if(m_isActive){
    text.setFillColor(sf::Color::Green);
    text.setString("EQUIPED");
    text.setStyle(sf::Text::Style::Bold);
  }else{
    text.setFillColor(sf::Color::Yellow);
    text.setString("OWNED");
    text.setStyle(sf::Text::Style::Bold);
  }
  target.draw(m_button, states);
  target.draw(m_texture, states);
  target.draw(text, states);
  target.draw(m_description);
}
