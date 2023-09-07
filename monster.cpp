#include <algorithm> // std::shuffle
#include <array>
#include <cassert>
#include <ctime> // std::time
#include <iostream>
#include <random> // std::mt19937
#include <assert.h>
#include <string>
#include "Random.h"

class Creature
{
protected:
    std::string m_name;
    char m_sym;
    int m_health;
    int m_damage;
    int m_gold;

public:
    Creature(std::string name, char sym, int health, int damage, int gold)
        : m_name{name}, m_sym{sym}, m_health{health}, m_damage{damage}, m_gold{gold}
    {
    }
    std::string getName() { return m_name; };
    char getSym() { return m_sym; };
    int getHealth() { return m_health; };
    int getDamage() { return m_damage; };
    int getGold() { return m_gold; };
    void reduceHealth(int n) { m_health -= n; };
    bool isDead() { return m_health <= 0; };
    void addGold(int n) { m_gold += n; };
};

class Player : public Creature
{
private:
    int m_level = 1;

public:
    Player(std::string name) : Creature(name, '@', 10, 1, 0) {}
    void levelUp()
    {
        m_level += 1;
        m_damage += 1;
    }
    bool hasWon() const { return m_level >= 20; }
    int getLevel() { return m_level; }
};

class Monster : public Creature
{
public:
    enum Type
    {
        DRAGON,
        ORC,
        SLIME,
        max_types
    };
    Monster(Type type)
        : Creature{getDefaultCreature(type)}
    {
    }
    static Monster getRandomMonster()
    {
        return Monster(static_cast<Type>(Random::get(0, static_cast<int>(Type::max_types) - 1)));
    }

private:
    static const Creature &getDefaultCreature(Type type)
    {
        static const std::array<Creature, static_cast<std::size_t>(Type::max_types)> monsterData{
            {{"dragon", 'D', 20, 4, 100},
             {"orc", 'o', 4, 2, 25},
             {"slime", 's', 1, 1, 10}}};

        return monsterData.at(static_cast<std::size_t>(type));
    }
};

char readPlayerMove()
{
    std::cout << "(R)un or (F)ight:";
    char rtn;
    std::cin >> rtn;
    while (rtn != 'f' && rtn != 'r')
    {
        std::cin.clear();
        std::cout << "Invalid input\n (R)un or (F)ight:";
        std::cin >> rtn;
    }
    return rtn;
};

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // set initial seed value to system clock
    std::rand();                                               // get rid of first result
    std::cout << "Enter Your Name: ";
    std::string player_name;
    std::cin >> player_name;
    std::cout << "Welcome, " << player_name << ". \n";
    Player player = Player(player_name);
    while (!player.isDead() && !player.hasWon())
    {

        Monster m = Monster::getRandomMonster();
        std::cout << "You have encountered a " << m.getName() << " " << m.getSym() << '\n';
        while (true)
        {
            char player_input = readPlayerMove();
            if (player_input == 'r')
            {
                int chance = Random::get(0, 1);
                if (chance)
                {
                    std::cout << "You successfully fled.\n";
                    break;
                }
                else
                {
                    std::cout << "You failed to flee.\n";
                }
            }
            else
            {
                m.reduceHealth(player.getDamage());
                std::cout << "You hit the " << m.getName() << " for " << player.getDamage() << " damage.\n";
            }
            if (m.isDead())
            {
                std::cout << "You killed the " << m.getName() << ". \n";
                player.levelUp();
                std::cout << "You are now level " << player.getLevel() << ". \n";
                player.addGold(m.getGold());
                std::cout << "You found " << m.getGold() << " gold.\n";
                break;
            }
            else
            {
                player.reduceHealth(m.getDamage());
                std::cout << "The " << m.getName() << " hit you for " << m.getDamage() << " damage.\n";
            }

            if (player.isDead())
            {
                std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " gold.\nToo bad you can’t take it with you!";
                break;
            }
        }
        if (player.hasWon())
        {
            std::cout << "You won !! you have " << player.getGold() << " gold.\n";
        }
    }
    return 0;
}
