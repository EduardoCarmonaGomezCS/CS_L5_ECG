// Eduardo Carmona Gomez
// Lab #5
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

// ===== Constants ===== //
const int MAX_CREATURES = 10;
const int MAX_NAME_LEN = 20;
const int MIN_STAT = 50;
const int MAX_STAT = 100;
const int DEFAULT_STAT = 100;
const char* const CREATURE_TYPES[] = { "Macara", "Ceffyl", "Nuggle", "Bahamut" };
const int TYPE_COUNT = 4;

const char* const NAMES[] = {
    "Zara", "Talon", "Lyra", "Kane", "Rhea", "Jinx", "Orin", "Nova", "Axel", "Mira",
    "Dax", "Vira", "Leo", "Nyx", "Zeke", "Echo", "Iris", "Jett", "Luna", "Thor"
};
const int NAME_COUNT = 20;

// ===== Creature Class ===== //
class Creature {
private:
    char name[MAX_NAME_LEN];
    char type[MAX_NAME_LEN];
    int strength;
    int health;

    void setCreature(const char* n, const char* t, int s, int h) {
        strncpy(name, n, MAX_NAME_LEN);
        name[MAX_NAME_LEN - 1] = '\0';

        strncpy(type, t, MAX_NAME_LEN);
        type[MAX_NAME_LEN - 1] = '\0';

        strength = (s >= MIN_STAT && s <= MAX_STAT) ? s : DEFAULT_STAT;
        health = (h >= MIN_STAT && h <= MAX_STAT) ? h : DEFAULT_STAT;
    }

public:
    Creature() {
        setCreature("n/a", CREATURE_TYPES[0], DEFAULT_STAT, DEFAULT_STAT);
    }

    Creature(const char* n, const char* t) {
        int s = MIN_STAT + rand() % (MAX_STAT - MIN_STAT + 1);
        int h = MIN_STAT + rand() % (MAX_STAT - MIN_STAT + 1);
        setCreature(n, t, s, h);
    }

    int getDamage() {
        int damage = rand() % (strength + 1);

        if (strcmp(type, "Ceffyl") == 0 && rand() % 100 < 15) {
            damage += 25;
        }
        else if (strcmp(type, "Nuggle") == 0 && rand() % 15 == 0) {
            damage *= 2;
        }

        return damage;
    }

    void takeDamage(int d) {
        health -= d;
        if (health < 0) health = 0;
    }

    bool isAlive() const {
        return health > 0;
    }

    int getHealth() const { return health; }
    int getStrength() const { return strength; }
    const char* getName() const { return name; }
    const char* getType() const { return type; }

    void print() const {
        cout << name << " (" << type << ") HP: " << health << " STR: " << strength << endl;
    }
};

// ===== Army Class ===== //
class Army {
private:
    char name[MAX_NAME_LEN];
    int size;
    Creature* creatures;

public:
    Army(const char* n, int s, int startNameIndex) {
        strncpy(name, n, MAX_NAME_LEN);
        name[MAX_NAME_LEN - 1] = '\0';

        size = s;
        creatures = new Creature[size];

        for (int i = 0; i < size; ++i) {
            const char* cname = NAMES[(startNameIndex + i) % NAME_COUNT];
            const char* ctype = CREATURE_TYPES[rand() % TYPE_COUNT];
            creatures[i] = Creature(cname, ctype);
        }
    }

    ~Army() {
        delete[] creatures;
    }

    Creature& getCreature(int index) {
        return creatures[index];
    }

    int getSize() const {
        return size;
    }

    int totalHealth() const {
        int total = 0;
        for (int i = 0; i < size; ++i)
            total += creatures[i].getHealth();
        return total;
    }

    void print() const {
        cout << "\nArmy: " << name << endl;
        for (int i = 0; i < size; ++i)
            creatures[i].print();
    }

    const char* getName() const {
        return name;
    }
};

// ===== Game Class ===== //
class Game {
private:
    Army* army1;
    Army* army2;
    int size;

public:
    Game(const char* name1, const char* name2, int s) {
        size = s;
        army1 = new Army(name1, s, 0);
        army2 = new Army(name2, s, s);  // Different name index
    }

    ~Game() {
        delete army1;
        delete army2;
    }

    void play() {
        cout << "\n--- Before Battle ---\n";
        army1->print();
        army2->print();

        cout << "\n--- Battle Begins ---\n";

        for (int i = 0; i < size; ++i) {
            Creature& c1 = army1->getCreature(i);
            Creature& c2 = army2->getCreature(i);

            cout << "\n?? Battle between " << c1.getName() << " and " << c2.getName() << endl;

            bool turn = rand() % 2; // 0 = c1, 1 = c2

            while (c1.isAlive() && c2.isAlive()) {
                if (turn == 0) {
                    int dmg = c1.getDamage();
                    cout << c1.getName() << " attacks " << c2.getName()
                        << " for " << dmg << " damage." << endl;
                    c2.takeDamage(dmg);
                }
                else {
                    int dmg = c2.getDamage();
                    cout << c2.getName() << " attacks " << c1.getName()
                        << " for " << dmg << " damage." << endl;
                    c1.takeDamage(dmg);
                }

                if (strcmp(c1.getType(), "Bahamut") == 0 && turn == 0) {
                    int dmg = c1.getDamage();
                    cout << c1.getName() << " (Bahamut) attacks again for " << dmg << " damage!" << endl;
                    c2.takeDamage(dmg);
                }
                if (strcmp(c2.getType(), "Bahamut") == 0 && turn == 1) {
                    int dmg = c2.getDamage();
                    cout << c2.getName() << " (Bahamut) attacks again for " << dmg << " damage!" << endl;
                    c1.takeDamage(dmg);
                }

                turn = 1 - turn;
            }

            if (c1.isAlive()) {
                cout << c1.getName() << " wins this round!\n";
            }
            else {
                cout << c2.getName() << " wins this round!\n";
            }
        }

        cout << "\n--- After Battle ---\n";
        army1->print();
        army2->print();

        int h1 = army1->totalHealth();
        int h2 = army2->totalHealth();

        cout << "\n?? Final Result:\n";
        cout << army1->getName() << " total health: " << h1 << endl;
        cout << army2->getName() << " total health: " << h2 << endl;

        if (h1 > h2)
            cout << army1->getName() << " wins the war!\n";
        else if (h2 > h1)
            cout << army2->getName() << " wins the war!\n";
        else
            cout << "It's a draw!\n";
    }
};

// ===== Main Function ===== //
int main() {
    srand(time(0));
    int choice;

    do {
        cout << "\n=== Creature Battle Game ===\n";
        cout << "1. Play\n";
        cout << "2. Quit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            char army1name[20], army2name[20];
            int size;

            cout << "Enter name for Army 1: ";
            cin >> army1name;

            cout << "Enter name for Army 2: ";
            cin >> army2name;

            cout << "Enter number of creatures in each army (max " << MAX_CREATURES << "): ";
            cin >> size;
            if (size <= 0 || size > MAX_CREATURES) {
                cout << "Invalid size. Exiting...\n";
                break;
            }

            Game g(army1name, army2name, size);
            g.play();
        }
    } while (choice != 2);

    return 0;
}
