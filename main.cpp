#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>

#include "json/json.hpp"

#include "shop.hpp"
#include "objects.hpp"
#include "functions.hpp"
#include "player.hpp"


using namespace std;


int main()
{
  /*======================INIT START=========================*/

  srand(time(NULL));

  sf::ContextSettings settings(24, 8, 0, 3, 0);
  sf::RenderWindow app(sf::VideoMode(800, 600), "Asteroids", sf::Style::Default, settings);
  app.setFramerateLimit(60);
  //app.setVerticalSyncEnabled(true);

  sf::Font fira;
  fira.loadFromFile("assets/FiraSans-Regular.otf");

  sf::Texture all;
  all.loadFromFile("assets/asteroids.png");

  //définition du joueur
  Player player(all, sf::IntRect(0, 64, 32, 32));

  sf::Texture backgroundText;
  backgroundText.loadFromFile("assets/background.png");
  backgroundText.setRepeated(true);
  sf::Sprite backgroundSpr(backgroundText);

  sf::FloatRect windowsBounds(-100, -100, 1000, 800);

  //def asteroids
  sf::Sprite asteroidLarge(all, sf::IntRect(0, 0, 64, 64));
  asteroidLarge.setOrigin(32, 32);
  asteroidLarge.scale(2, 2);

  sf::Sprite asteroidMedium;
  asteroidMedium.setTexture(all);
  asteroidMedium.setOrigin(16, 16);
  asteroidMedium.scale(2, 2);

  sf::IntRect astMedRect[4];
  for(size_t i(0); i<4; i++){
    astMedRect[i] = sf::IntRect(64+ (i/2)*32, (i%2)*32, 32, 32);
  }

  sf::Sprite asteroidSmall;
  asteroidSmall.setTexture(all);
  asteroidSmall.setOrigin(8, 8);
  asteroidSmall.scale(2, 2);

  sf::IntRect astSmallRect[8];
  for(size_t i(0); i<8; i++){
    astSmallRect[i] = sf::IntRect(128+ (i/4)*16, (i%4)*16, 16, 16);
  }

  vector<Asteroid> asteroids(0);

  int astLevel(0);
  int goldRate(1);

  int score(0);

  //def pièces
  sf::Sprite coinText;
  coinText.setTexture(all);
  coinText.setOrigin(4, 4);
  coinText.scale(2, 2);

  sf::IntRect coinAnim[8];
  for (size_t i = 0; i < 8; i++) {
    coinAnim[i] = sf::IntRect(160, i*8, 8, 8);
  }

  sf::Color coinColors[8] = {{220, 70, 0},
                             {210, 210, 220},
                             {255, 230, 20},
                             {0, 128, 0},
                             {64, 128, 255},
                             {64, 0, 192},
                             {32, 32, 32},
                             {0, 0, 0}};

  vector<Coin> coins(0);

  unsigned int money(0), totalMoney(0);
  unsigned int coinValues[8] = {10, 50, 200, 1000, 5000, 20000, 100000, 1000000};
  int moneyDrop(1);
  int coinLevel(0);

  int coinLevelDropChance[8][8] = {{100, 0, 0, 0, 0, 0, 0, 0},
                                  {100, 25, 0, 0, 0, 0, 0, 0},
                                  {100, 75, 25, 0, 0, 0, 0, 0},
                                  {100, 85, 60, 20, 0, 0, 0, 0},
                                  {100, 90, 70, 45, 15, 0, 0, 0},
                                  {100, 95, 80, 60, 40, 10, 0, 0},
                                  {100, 95, 85, 70, 55, 35, 10, 0},
                                  {100, 95, 85, 75, 60, 45, 20, 5}};

  sf::Text moneyText("", fira, 36);
  moneyText.setScale(0.5f, 0.5f);
  moneyText.setPosition(10, 40);
  moneyText.setFillColor(sf::Color::White);

  Shot::loadMissileTexture(&all, sf::IntRect(168, 0, 4, 8));

  int t(0);

  //boutons du menu
  Button play({300, 220, 200, 40}, {0, 192, 0}, {0, 64, 0}, "Play", 18, fira);
  Button option({300, 280, 200, 40}, {0, 0, 255}, {0, 0, 128}, "Options", 18, fira);
  Button quit({300, 340, 200, 40}, {192, 0, 0}, {64, 0, 0}, "Quit", 18, fira);

  Button goShop({690, 10, 100, 40}, {255, 255, 0}, {128, 128, 0}, "Shop ->", 18, fira);
  //boutons du shop
  Button shopBack({10, 10, 100, 40}, {0, 192, 0}, {0, 64, 0}, "<- Back", 18, fira);
  Button backMenu({690, 10, 100, 40}, {0, 192, 0}, {0, 64, 0}, "Menu ->", 18, fira);

  vector<ShopUpgrade> shopUpgrades({});
  shopUpgrades.push_back(ShopUpgrade(20, 99, {60, 100}, all, {0, 96, 16, 16}, 1.8f, "Increase shot speed rate", fira));
  shopUpgrades.push_back(ShopUpgrade(10, 100, {60, 150}, all, {0, 112, 16, 16}, 2.75f, "Increase movement speed", fira));
  shopUpgrades.push_back(ShopUpgrade(10, 100, {60, 200}, all, {0, 128, 16, 16}, 2.75f, "Increase maniability", fira));
  shopUpgrades.push_back(ShopUpgrade(15, 32, {60, 250}, all, {16, 96, 16, 16}, 2.2222f, "Increase max life", fira));
  shopUpgrades.push_back(ShopUpgrade(10, 500, {60, 300}, all, {16, 112, 16, 16}, 3.f, "Increase coin drop", fira));
  shopUpgrades.push_back(ShopUpgrade(7, 2342, {60, 350}, all, {32, 96, 16, 16}, 4.f, "Increase coin value", fira));
  shopUpgrades.push_back(ShopUpgrade(12, 350, {60, 400}, all, {32, 112, 16, 16}, 2.14159f, "Increase coin magnet range", fira));
  shopUpgrades.push_back(ShopUpgrade(11, 123, {60, 450}, all, {32, 128, 16, 16}, 2.71828f, "Increase asteroids spawn rate", fira));
  shopUpgrades.push_back(ShopUpgrade(14, 255, {60, 500}, all, {32, 128, 16, 16}, 1.99f, "Increase golden asteroids rate", fira));

  vector<SwitchableShopUpgrade> weaponType({});
  weaponType.push_back(SwitchableShopUpgrade(0, {450, 100}, all, {48, 96, 16, 16}, "base shots", fira));
  weaponType.push_back(SwitchableShopUpgrade(5000, {450, 140}, all, {48, 112, 16, 16}, "laser shots", fira));
  weaponType.push_back(SwitchableShopUpgrade(25000, {450, 180}, all, {48, 128, 16, 16}, "guided missiles", fira));

  vector<SwitchableShopUpgrade> weaponFire({});
  weaponFire.push_back(SwitchableShopUpgrade(0, {450, 260}, all, {64, 96, 16, 16}, "one shot, base fire rate", fira));
  weaponFire.push_back(SwitchableShopUpgrade(25000, {450, 300}, all, {64, 112, 16, 16}, "3 shots", fira));
  weaponFire.push_back(SwitchableShopUpgrade(500000, {450, 340}, all, {64, 128, 16, 16}, "rapid fire", fira));

  SwitchableShopUpgrade heal(0, {450, 420}, all, {80, 96, 16, 16}, "Retrieve all your health", fira);

  /*======================INIT END=========================*/


  //gameloop
  while (app.isOpen()){

    //load save//
    {
      ifstream saveFile("save", ios::in | ios::binary);
      if(saveFile.is_open()){

        string saveStr( (std::istreambuf_iterator<char>(saveFile)),
        (std::istreambuf_iterator<char>()        ) );

        size_t i(0);
        json::Object save(json::parse(saveStr, i));

        player.init((int)save["player"]["life"],
                    (int)save["player"]["max life"],
                    (int)save["player"]["shot type"],
                    (double)save["player"]["shot speed"],
                    (int)save["player"]["fire type"],
                    (double)save["player"]["speed"],
                    (double)save["player"]["rotation speed"],
                    (int)save["player"]["magnet range"]);

        money = (int)save["money"]["money"];
        totalMoney = (int)save["money"]["total"];
        moneyDrop  = (int)save["money"]["drop chance"];
        coinLevel = (int)save["money"]["level"];

        score = (int)save["score"];

        astLevel = (int)save["asteroids"]["spawn rate"];
        goldRate = (int)save["asteroids"]["golden chance"];

        for (size_t i = 0; i < shopUpgrades.size(); i++){
          shopUpgrades[i].unlock((int)save["shop upgrade"][i]);
        }
        for (size_t i = 0; i < weaponType.size(); i++){
          if((bool)save["weapons"]["shot type"][i] == true)
          weaponType[i].unlock();
        }
        for (size_t i = 0; i < weaponFire.size(); i++){
          if((bool)save["weapons"]["firing pattern"][i] == true)
          weaponFire[i].unlock();
        }
      }
      saveFile.close();
    }
    weaponType[0].unlock();
    weaponType[0].setActive(true);
    weaponFire[0].unlock();
    weaponFire[0].setActive(true);

    sf::Event event;

    bool next(false);
    // menu
    while(!next){
      while (app.pollEvent(event)){
        if (event.type == sf::Event::Closed || quit.isClicked(event, app)){
          app.close();
          next = true;
        }
        if(play.isClicked(event, app)){
          next = true;
        }
      }
      play.updateHover(app);
      option.updateHover(app);
      quit.updateHover(app);

      resetView(app, backgroundSpr);
      app.draw(play);
      app.draw(option);
      app.draw(quit);
      app.display();
    }

    player.resetEffects();
    coins.clear();
    asteroids.clear();

    bool gameIsPlaying(true);
    while(app.isOpen() && gameIsPlaying)
    {
      t++;

      while (app.pollEvent(event)){
        if (event.type == sf::Event::Closed){
          app.close();
          break;
        }
        if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || (event.type == sf::Event::MouseButtonPressed && goShop.isClicked(event, app))){
          /*======================SHOP=========================*/
          next = false;
          while(!next){
            while(app.pollEvent(event)){
              if (event.type == sf::Event::Closed){
                app.close();
                next = true;
              }
              if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || (event.type == sf::Event::MouseButtonPressed && shopBack.isClicked(event, app))){
                next = true;
              }
              if((event.type == sf::Event::MouseButtonPressed && backMenu.isClicked(event, app))){
                next = true;
                gameIsPlaying = false;
              }
              if(event.type == sf::Event::MouseButtonPressed){
                if(shopUpgrades[0].canBuy(money) && shopUpgrades[0].tryBuy(event, app)){
                  shopUpgrades[0].buy(money);
                  player.setShotSpeed(player.getShotSpeed()* 0.95f);
                }
                if(shopUpgrades[1].canBuy(money) && shopUpgrades[1].tryBuy(event, app)){
                  shopUpgrades[1].buy(money);
                  player.setSpeed(player.getSpeed()*1.15f);
                }
                if(shopUpgrades[2].canBuy(money) && shopUpgrades[2].tryBuy(event, app)){
                  shopUpgrades[2].buy(money);
                  player.setRotationSpeed(player.getRotationSpeed()*1.2);
                }
                if(shopUpgrades[3].canBuy(money) && shopUpgrades[3].tryBuy(event, app)){
                  shopUpgrades[3].buy(money);
                  player.setMaxLife(player.getMaxLife()+1);
                  player.setLife(player.getLife()+1);
                }
                if(shopUpgrades[4].canBuy(money) && shopUpgrades[4].tryBuy(event, app)){
                  shopUpgrades[4].buy(money);
                  moneyDrop++;
                }
                if(shopUpgrades[5].canBuy(money) && shopUpgrades[5].tryBuy(event, app)){
                  shopUpgrades[5].buy(money);
                  coinLevel++;
                }
                if(shopUpgrades[6].canBuy(money) && shopUpgrades[6].tryBuy(event, app)){
                  shopUpgrades[6].buy(money);
                  player.setMagnetRange(player.getMagnetRange() + 16);
                }
                if(shopUpgrades[7].canBuy(money) && shopUpgrades[7].tryBuy(event, app)){
                  shopUpgrades[7].buy(money);
                  astLevel++;
                }
                if(shopUpgrades[8].canBuy(money) && shopUpgrades[8].tryBuy(event, app)){
                  shopUpgrades[8].buy(money);
                  goldRate++;
                }
                //switchable upgrade
                  //arme
                if(weaponType[0].tryBuy(event, app)){
                  weaponType[0].setActive(true);
                  weaponType[1].setActive(false);
                  weaponType[2].setActive(false);
                  player.setShotType(0);
                }
                if(weaponType[1].tryBuy(event, app)){
                  if(!weaponType[1].isBuyed()){
                    if(weaponType[1].canBuy(money)){
                      weaponType[1].buy(money);
                    }
                  }
                  if(weaponType[1].isBuyed()){
                    weaponType[0].setActive(false);
                    weaponType[1].setActive(true);
                    weaponType[2].setActive(false);
                    player.setShotType(1);
                  }
                }
                if(weaponType[2].tryBuy(event, app)){
                  if(!weaponType[2].isBuyed()){
                    if(weaponType[2].canBuy(money)){
                      weaponType[2].buy(money);
                    }
                  }
                  if(weaponType[2].isBuyed()){
                    weaponType[0].setActive(false);
                    weaponType[1].setActive(false);
                    weaponType[2].setActive(true);
                    player.setShotType(2);
                  }
                }
                  //manière de tirer
                if(weaponFire[0].tryBuy(event, app)){
                  weaponFire[0].setActive(true);
                  weaponFire[1].setActive(false);
                  weaponFire[2].setActive(false);
                  player.setFireType(0);
                }
                if(weaponFire[1].tryBuy(event, app)){
                  if(!weaponFire[1].isBuyed()){
                    if(weaponFire[1].canBuy(money)){
                      weaponFire[1].buy(money);
                    }
                  }
                  if(weaponFire[1].isBuyed()){
                    weaponFire[0].setActive(false);
                    weaponFire[1].setActive(true);
                    weaponFire[2].setActive(false);
                    player.setFireType(1);
                  }
                }
                if(weaponFire[2].tryBuy(event, app)){
                  if(!weaponFire[2].isBuyed()){
                    if(weaponFire[2].canBuy(money)){
                      weaponFire[2].buy(money);
                    }
                  }
                  if(weaponFire[2].isBuyed()){
                    weaponFire[0].setActive(false);
                    weaponFire[1].setActive(false);
                    weaponFire[2].setActive(true);
                    player.setFireType(2);
                  }
                }

                if(heal.tryBuy(event, app)){
                  if(heal.canBuy(money)){
                    heal.buy(money);
                    heal.unBuy();
                    player.setLife(player.getMaxLife());
                  }
                }
              }
            }

            resetView(app, backgroundSpr);

            shopBack.updateHover(app);
            app.draw(shopBack);
            backMenu.updateHover(app);
            app.draw(backMenu);

            for (size_t i = 0; i < shopUpgrades.size(); i++) {
              shopUpgrades[i].update(money, app);
              app.draw(shopUpgrades[i]);
            }

            for (size_t i = 0; i < weaponType.size(); i++) {
              weaponType[i].update(money, app);
              app.draw(weaponType[i]);
            }

            for (size_t i = 0; i < weaponFire.size(); i++) {
              weaponFire[i].update(money, app);
              app.draw(weaponFire[i]);
            }

            heal.update(money, app);
            heal.setPrice(0.05 * totalMoney);
            app.draw(heal);

            moneyText.setString(std::to_string(money));
            moneyText.setPosition(97 - moneyText.getGlobalBounds().width , 60);
            app.draw(moneyText);
            coinText.setTextureRect(coinAnim[0]);
            coinText.setPosition(107, 71);
            coinText.setColor(coinColors[2]);
            app.draw(coinText);

            app.display();
          }
            /*======================SHOP=========================*/
        }
      }

	  if(!gameIsPlaying) break;

      player.update();

      //gestion des touches
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)){
        player.moveForward();
      }

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
        player.moveBackward();
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){
        player.shot();
      }

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
        player.rotateLeft();
      }

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        player.rotateRight();
      }

      //génération des astéroides
      if(rand()%(120 - 10 * astLevel) == 0){
        int dir(rand()%4);
        sf::Vector2f temppos;
        float precdir(rand()%90*pi/180);
        switch(dir){
          case 0:
            temppos = sf::Vector2f{(float)(rand()%600 + 100),-50};
            precdir += pi/4;
            break;
          case 1:
            temppos = sf::Vector2f{(float)(rand()%600 + 100), 650};
            precdir += 5*pi/4;
            break;
          case 2:
            temppos = sf::Vector2f{-50, (float)(rand()%400 + 100)};
            precdir += 7*pi/4;
            break;
          case 3:
            temppos = sf::Vector2f{850, (float)(rand()%400 + 100)};
            precdir += 3*pi/4;
            break;
        }
        float v(rand()%256 / 128.0 + 0.5);
        int size = rand()%20;
        if(size > 12) size = 0;
        if(size > 2) size %= 2;
        asteroids.push_back(createAsteroid(temppos, sf::Vector2f{(float)(v * cos(precdir)), (float)(v * sin(precdir))}, size, rand()%100 < goldRate));
      }

      //gestion des mouvements
        //pièces
      if(t%12 == 0){
        for (size_t i = 0; i < coins.size(); i++) {
          coins[i].animLevel++;
          if (coins[i].animLevel >= 8) {
            coins[i].animLevel = 0;
          }
        }
      }
      for (size_t i = 0; i < coins.size(); i++) {
        coins[i].lifespan++;
        if(coins[i].lifespan > 900){
          coins.erase(coins.cbegin()+i);
          i--;
        }
      }
        //tirs
      for(size_t i(0); i<Shot::shots.size(); i++){
        Shot::shots[i].update();
        if(Shot::shots[i].getType() == 2){
          Shot::shots[i].moveToAst(asteroids, 16.f);
        }else Shot::shots[i].moveForward(16.f);
        if(!windowsBounds.contains(Shot::shots[i].getPosition())){
          Shot::erase(i);
          i--;
        }
      }
        //astéroides
      for(size_t i(0); i<asteroids.size(); i++){
        asteroids[i].pos += asteroids[i].v;
        asteroids[i].r += asteroids[i].vr;
        if(!windowsBounds.contains(asteroids[i].pos)){
          asteroids.erase(asteroids.cbegin()+i);
          i--;
        }
      }

      //gestion des collisions
        //astéroides - tirs
      for (size_t j = 0; j < asteroids.size(); j++) {
        int colide(Shot::colide(asteroids[j]));
        if(colide > -1 && Shot::shots[colide].canTouch()){
          if(Shot::shots[colide].getType() == 2){
            asteroids[j].life -= 4;
          }
          else {
            asteroids[j].life--;
          }
          if(Shot::shots[colide].getType() != 1){
            Shot::erase(colide);
          }else{
            Shot::shots[colide].setLastHit();
          }
          if(asteroids[j].life <= 0){
            if(asteroids[j].size > 0){
              unsigned int nbrChild = rand()%3 +1;
              for (size_t x = 0; x < nbrChild; x++) {
                float precdir(rand()%360*pi/180);
                float v(rand()%256 / 128.0 + 0.5);
                asteroids.push_back(createAsteroid(asteroids[j].pos, sf::Vector2f{(v * cosf(precdir)), (v * sinf(precdir))}, asteroids[j].size -1, asteroids[j].golden));
              }
            }else{
              unsigned int nbr(rand()%moneyDrop + moneyDrop/2);
              if(asteroids[j].golden){
                nbr = (nbr+1)*5;
              }
              for (size_t i = 0; i <= nbr; i++) {
                int c_coinLevel = rand()%100;
                int c_coin(0);
                for (size_t n = 0; n < 8; n++) {
                  if(c_coinLevel < coinLevelDropChance[coinLevel][n]){
                    c_coin = n;
                  }else break;
                }
                coins.push_back({sf::Vector2f(asteroids[j].pos.x + rand()%32, asteroids[j].pos.y + rand()%32),rand()%8, rand()%60, c_coin});
              }
            }
            int s_score(0);
            switch(asteroids[j].size){
              case 0:
                s_score += 25;
                break;
              case 1:
                s_score += 100;
                break;
              case 2:
                s_score += 500;
                break;
              default:
                break;
            }
            if (asteroids[j].golden) {
              s_score *= 5;
            }
            score += s_score;
            asteroids.erase(asteroids.cbegin()+j);
            j--;
          }
        }
      }

        //astéroides - player
      if(player.canBeHit()){
        for (size_t i = 0; i < asteroids.size(); i++) {
          if(player.isHit(asteroids[i])){
            score -= 500;
            asteroids.erase(asteroids.cbegin()+i);
            i--;
            if(!player.isAlive()){
              gameIsPlaying = false;
            }
          }
        }
      }

        //coins - player
      for (size_t i = 0; i < coins.size(); i++) {
        if(dist(player.getPosition(), coins[i].pos) < player.getMagnetRange()){
          coins[i].pos += 24.f * (10.f/(float)dist(player.getPosition(), coins[i].pos)) * normalize(player.getPosition() - coins[i].pos);
        }
        if(dist(player.getPosition(), coins[i].pos) < 8){
          money += coinValues[coins[i].valueLevel];
          totalMoney += coinValues[coins[i].valueLevel];
          score += 10;
          coins.erase(coins.cbegin()+i);
          i--;
        }
      }

      if(t%30 == 0){
        score++;
      }

      //affichage
      resetView(app, backgroundSpr);

      //definition de la couleur des pièces rainbow
      {
        if((t*16)%1536 < 256)
          coinColors[7] = sf::Color((t*16)%256, 0, 255);
        else if((t*16)%1536 < 512)
          coinColors[7] = sf::Color(255, 0, 255-(t*16)%256);
        else if((t*16)%1536 < 768)
          coinColors[7] = sf::Color(255, (t*16)%256, 0);
        else if((t*16)%1536 < 1024)
          coinColors[7] = sf::Color(255-(t*16)%256, 255, 0);
        else if((t*16)%1536 < 1280)
          coinColors[7] = sf::Color(0, 255, (t*16)%256);
        else
          coinColors[7] = sf::Color(0, 255-(t*16)%256, 255);
      }

      //pièces
      for(size_t i(0); i<coins.size(); i++){
        if(coins[i].lifespan < 720 || coins[i].lifespan%16/8 == 0){
          coinText.setPosition(coins[i].pos);
          coinText.setColor(coinColors[coins[i].valueLevel]);
          coinText.setTextureRect(coinAnim[coins[i].animLevel]);
          app.draw(coinText);
        }
      }

      //tirs
      for(size_t i(0); i<Shot::shots.size(); i++){
        app.draw(Shot::shots[i]);
      }

      app.draw(player);

      sf::CircleShape magnetRange(player.getMagnetRange());

      //astéroides
      for(size_t i(0); i<asteroids.size(); i++){
        sf::Color gold(255, 255, 255);
        if(asteroids[i].golden){
          gold = sf::Color(255, 192, 0);
        }
        switch(asteroids[i].size){
          case 0:
            asteroidSmall.setPosition(asteroids[i].pos);
            asteroidSmall.setRotation(asteroids[i].r);
            asteroidSmall.setColor(gold);
            asteroidSmall.setTextureRect(astSmallRect[asteroids[i].sprite]);
            app.draw(asteroidSmall);
            break;
          case 1:
            asteroidMedium.setPosition(asteroids[i].pos);
            asteroidMedium.setRotation(asteroids[i].r);
            asteroidMedium.setColor(gold);
            asteroidMedium.setTextureRect(astMedRect[asteroids[i].sprite]);
            app.draw(asteroidMedium);
            break;
          case 2:
            asteroidLarge.setPosition(asteroids[i].pos);
            asteroidLarge.setRotation(asteroids[i].r);
            asteroidLarge.setColor(gold);
            app.draw(asteroidLarge);
            break;
          default:
            break;
        }
      }
      sf::RectangleShape lifeDispExt;
      lifeDispExt.setFillColor(sf::Color(128, 128, 128));
      lifeDispExt.setOutlineColor(sf::Color(0, 128, 0));
      lifeDispExt.setOutlineThickness(4);
      lifeDispExt.setPosition(10, 10);
      lifeDispExt.setSize(sf::Vector2f(player.getMaxLife()*10, 20));
      app.draw(lifeDispExt);

      sf::RectangleShape lifeDisp;
      lifeDisp.setPosition(10, 10);
      lifeDisp.setSize(sf::Vector2f(10*player.getLife(), 20));
      lifeDisp.setFillColor(sf::Color(255-player.getLife()*(255.0/player.getMaxLife()), player.getLife()*(255.0/player.getMaxLife()), 0));
      app.draw(lifeDisp);

      sf::Text lifeText(to_string(player.getLife()) + " / " + to_string(player.getMaxLife()), fira, 36);
      lifeText.setOrigin(sf::Vector2f(lifeText.getGlobalBounds().width/2, 0));
      lifeText.setScale(0.5f, 0.5f);
      lifeText.setFillColor(sf::Color::Black);
      lifeText.setPosition(10+player.getMaxLife()*5, 10);
      app.draw(lifeText);

      moneyText.setString(std::to_string(money));
      moneyText.setPosition(97 - moneyText.getGlobalBounds().width , 40);
      app.draw(moneyText);
      coinText.setTextureRect(coinAnim[0]);
      coinText.setPosition(107, 51);
      coinText.setColor(coinColors[2]);
      app.draw(coinText);

      goShop.updateHover(app);
      app.draw(goShop);

      sf::Text scoreText("Score : " + to_string(score), fira, 36);
      scoreText.setScale(0.5, 0.5);
      scoreText.setFillColor(sf::Color::White);
      scoreText.setPosition(400-scoreText.getGlobalBounds().width/2, 580);
      app.draw(scoreText);

      app.display();
    }

    //SAVE
    {
      json::Object jsonplayer;
      json::Object jsonmoney;
      json::Object jsonasteroids;
      json::Array jsonShopUpgrades;
      json::Array jsonWeaponType;
      json::Array jsonWeaponFire;

      if(player.isAlive()){
        jsonplayer["life"] = player.getLife();
        jsonplayer["max life"] = player.getMaxLife();
        jsonplayer["shot type"] = player.getShotType();
        jsonplayer["shot speed"] = player.getShotSpeed();
        jsonplayer["fire type"] = player.getFireType();
        jsonplayer["speed"] = player.getSpeed();
        jsonplayer["rotation speed"] = player.getRotationSpeed();
        jsonplayer["magnet range"] = player.getMagnetRange();

        jsonmoney["money"] = (int)money;
        jsonmoney["total"] = (int)totalMoney;
        jsonmoney["drop chance"] = moneyDrop;
        jsonmoney["level"] = coinLevel;

        jsonasteroids["spawn rate"] = astLevel;
        jsonasteroids["golden chance"] = goldRate;

        for (size_t i = 0; i < shopUpgrades.size(); i++){
          jsonShopUpgrades.push_back((int)shopUpgrades[i].getLevel());
        }

        for (size_t i = 0; i < weaponType.size(); i++){
          if(weaponType[i].isBuyed())
            jsonWeaponType.push_back(true);
          else
            jsonWeaponType.push_back(false);
        }
        for (size_t i = 0; i < weaponFire.size(); i++){
          if(weaponFire[i].isBuyed())
            jsonWeaponFire.push_back(true);
          else
            jsonWeaponFire.push_back(false);
        }
      }else{
        jsonplayer["life"] = 10;
        jsonplayer["max life"] = 10;
        jsonplayer["shot type"] = 0;
        jsonplayer["shot speed"] = 30;
        jsonplayer["fire type"] = 0;
        jsonplayer["speed"] = 1.0;
        jsonplayer["rotation speed"] = 1.0;
        jsonplayer["magnet range"] = 32;

        jsonmoney["money"] = 0;
        jsonmoney["total"] = 0;
        jsonmoney["drop chance"] = 1;
        jsonmoney["level"] = 0;

        jsonasteroids["spawn rate"] = 0;
        jsonasteroids["golden chance"] = 1;

        for (size_t i = 0; i < shopUpgrades.size(); i++){
          jsonShopUpgrades.push_back(0);
        }
        jsonWeaponType.push_back(true);
        for (size_t i = 1; i < weaponType.size(); i++){
          jsonWeaponType.push_back(false);
        }
        jsonWeaponFire.push_back(true);
        for (size_t i = 1; i < weaponFire.size(); i++){
          jsonWeaponFire.push_back(false);
        }
      }
      json::Object jsonWeapons;
      jsonWeapons["shot type"] = jsonWeaponType;
      jsonWeapons["firing pattern"] = jsonWeaponFire;

      json::Object save;
      save["player"] = jsonplayer;
      save["score"] = 0;
      save["money"] = jsonmoney;
      save["asteroids"] = jsonasteroids;
      save["shop upgrade"] = jsonShopUpgrades;
      save["weapons"] = jsonWeapons;

      json::Value::minify = true;

      ofstream saveFile("save", ios::out | ios::binary);
      saveFile << save;
      saveFile.close();
    }
  }

  return EXIT_SUCCESS;
}
