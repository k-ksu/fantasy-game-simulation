#include <iostream>
#include <utility>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Weapon {
public:
    int damage;
    string name;

    Weapon() {}
    Weapon(int damage, string& name) {
        this->damage = damage;
        this->name = name;
    }

    friend ostream& operator<<(ostream &os, Weapon& w) {
        os << w.name << ":" << w.damage;
        return os;
    }
};

class Potion {
public:
    int healValue;
    string name;

    Potion() {}
    Potion(int healValue, string& name) {
        this->healValue = healValue;
        this->name = name;
    }

    friend ostream& operator<<(ostream &os, Potion& p) {
        os << p.name << ":" << p.healValue;
        return os;
    }
};

class Spell {
public:
    vector<string> allowedTargets;
    string name;

    Spell() {}
    Spell(vector<string>& allowedTargets, string& name) {
        this->allowedTargets = allowedTargets;
        this->name = name;
    }

    friend ostream& operator<<(ostream &os, Spell& s) {
        os << s.name << ":" << s.allowedTargets.size();
        return os;
    }
};

class Character {
public:
    vector<Weapon> arsenal;
    vector<Potion> medicalBag;
    vector<Spell> spellBook;

    int hp;
    string name;

    int maxArsenal;
    int maxMedicalBag;
    int maxSpellBook;

    string type;

    Character() {}
    Character(int healthPoints, string& type, string& name) {
        this->hp = healthPoints;
        this->name = name;
        this->type = type;

        if (type == "fighter") {
            maxArsenal = 3;
            maxMedicalBag = 5;
            maxSpellBook = 0;
        } else if (type == "wizard") {
            maxArsenal = 0;
            maxMedicalBag = 10;
            maxSpellBook = 10;
        } else if (type == "archer") {
            maxArsenal = 2;
            maxMedicalBag = 3;
            maxSpellBook = 2;
        }
    }

    // addWeapon returns true if weapon added, false if capacity is full
    bool addWeapon(Weapon& weapon) {
        if (arsenal.size() < maxArsenal) {
            arsenal.push_back(weapon);
            sort(
                    arsenal.begin(),
                    arsenal.end(),
                    [](Weapon& w1, Weapon& w2) {
                        return w1.name < w2.name;
                    }
            );
            return true;
        }

        return false;
    }

    // addPotion returns true if potion added, false if capacity is full
    bool addPotion(Potion& potion) {
        if (medicalBag.size() < maxMedicalBag) {
            medicalBag.push_back(potion);
            sort(
                    medicalBag.begin(),
                    medicalBag.end(),
                    [](Potion& p1, Potion& p2) {
                        return p1.name < p2.name;
                    }
            );
            return true;
        }

        return false;
    }

    // addSpell returns true if spell added, false if capacity is full
    bool addSpell(Spell& spell) {

        if (this->spellBook.size() < maxSpellBook) {
            this->spellBook.push_back(spell);
            sort(
                    this->spellBook.begin(),
                    this->spellBook.end(),
                    [](Spell& s1, Spell& s2) {
                        return s1.name < s2.name;
                    }
            );
            return true;
        }

        return false;
    }

    bool doesWeaponExist(string& weaponName) {
        for (auto& w : arsenal) {
            if (w.name == weaponName) {
                return true;
            }
        }

        return false;
    }

    bool doesPotionExist(string& potionName) {
        for (auto& p : medicalBag) {
            if (p.name == potionName) {
                return true;
            }
        }

        return false;
    }

    bool doesSpellExist(string& spellName) {
        for (auto& s : this->spellBook) {
            if (s.name == spellName) {
                return true;
            }
        }

        return false;
    }

    Weapon getWeapon(string& weaponName) {
        for (auto w : arsenal) {
            if (w.name == weaponName) {
                return w;
            }
        }

        return Weapon();
    }

    Potion getPotion(string& potionName) {
        for (auto p : medicalBag) {
            if (p.name == potionName) {
                return p;
            }
        }

        return Potion();
    }

    Spell getSpell(string& spellName) {
        for (auto s : spellBook) {
            if (s.name == spellName) {
                return s;
            }
        }

        return Spell();
    }

    friend ostream& operator<<(ostream &os, Character& ch) {
        os << ch.name << ":" << ch.type << ":" << ch.hp;
        return os;
    }
};



class Game {
private:
    int stepsNumber;
    vector<Character> characters;

    void sayDialogue(string& characterName, string& sentence) {
        cout << characterName << ": " << sentence << endl;
    }

    bool doesCharacterExist(string& name) {
        for (auto ch : characters) {
            if (ch.name == name) {
                return true;
            }
        }

        return false;
    }

    Character* findCharacterByName(string& name) {
        for (int i = 0; i < characters.size(); i++) {
            if (characters[i].name == name) {
                return &characters[i];
            }
        }

        return nullptr;
    }

public:
    void startAdventure() {
        cin >> stepsNumber;

        string command;
        getline(cin, command);
        while (stepsNumber--) {
            getline(cin, command);

            stringstream ss(command);
            vector<string> words;
            string word;
            while (ss >> word) {
                words.push_back(word);
            }

            if (words[0] == "Dialogue") {
                string character = words[1];

                string sentence;
                for (int i = 3; i < words.size(); i++) {
                    sentence += words[i];
                    if (i != words.size() - 1) {
                        sentence += " ";
                    }
                }

                if (character == "Narrator") {
                    sayDialogue(character, sentence);
                } else {
                    bool wasFound = false;
                    for (auto ch : characters) {
                        if (ch.name == character) {
                            sayDialogue(character, sentence);
                            wasFound = true;
                            break;
                        }
                    }

                    if (!wasFound) {
                        cout << "Error caught" << endl;
                    }
                }
            } else if (words[0] == "Create") {
                if (words[1] == "character") {
                    string type = words[2];
                    string name = words[3];
                    int initHP = stoi(words[4]);

                    Character newCharacter(initHP, type, name);
                    characters.push_back(newCharacter);
                    sort(
                            characters.begin(),
                            characters.end(),
                            [](Character& c1, Character& c2) {
                                return c1.name < c2.name;
                            }
                    );

                    cout << "A new " << type << " came to town, " << name << "." << endl;
                } else if (words[1] == "item") {
                    if (words[2] == "weapon") {
                        string character = words[3];
                        string weaponName = words[4];
                        int weaponDamage = stoi(words[5]);

                        bool doesExist = doesCharacterExist(character);
                        if (!doesExist) {
                            cout << "Error caught" << endl;
                            continue;
                        }

                        if (weaponDamage <= 0) {
                            cout << "Error caught" << endl;
                            continue;
                        }

                        Weapon newWeapon = Weapon(weaponDamage, weaponName);
                        Character* ch = findCharacterByName(character);
                        bool wasAdded = ch->addWeapon(newWeapon);
                        if (!wasAdded) {
                            cout << "Error caught" << endl;
                            continue;
                        }

                        cout << character << " just obtained a new weapon called " << weaponName << "." << endl;
                    } else if (words[2] == "potion") {
                        string character = words[3];
                        string potionName = words[4];
                        int potionEffect = stoi(words[5]);

                        bool doesExist = doesCharacterExist(character);
                        if (!doesExist) {
                            cout << "Error caught" << endl;
                            continue;
                        }

                        if (potionEffect <= 0) {
                            cout << "Error caught" << endl;
                            continue;
                        }

                        Potion newPotion = Potion(potionEffect, potionName);
                        Character* ch = findCharacterByName(character);
                        bool wasAdded = ch->addPotion(newPotion);
                        if (!wasAdded) {
                            cout << "Error caught" << endl;
                            continue;
                        }

                        cout << character << " just obtained a new potion called " << potionName << "." << endl;
                    } else if (words[2] == "spell") {
                        string character = words[3];
                        string spellName = words[4];
                        vector<string> canBeAppliedTo;

                        bool isErr = false;
                        for (int i = 6; i < words.size(); i++) {
                            canBeAppliedTo.push_back(words[i]);
                            bool isCh = false;
                            for (int j = 0; j < characters.size(); j++) {
                                if (characters[j].name == words[i]) {
                                    isCh = true;
                                    break;
                                }
                            }

                            if (!isCh) {
                                isErr = true;
                                break;
                            }
                        }

                        if (isErr) {
                            cout << "Error caught" << endl;
                            continue;
                        }
                        // TODO: handle spell creation

                        bool doesExist = doesCharacterExist(character);
                        if (!doesExist) {
                            cout << "Error caught" << endl;
                            continue;
                        }

                        Spell newSpell = Spell(canBeAppliedTo, spellName);
                        Character* ch = findCharacterByName(character);
                        bool wasAdded = ch->addSpell(newSpell);
                        if (!wasAdded) {
                            cout << "Error caught" << endl;
                            continue;
                        }

                        cout << character << " just obtained a new spell called " << spellName << "." << endl;
                    }
                }
            } else if (words[0] == "Attack") {
                string attackerName = words[1];
                string targetName = words[2];
                string weaponName = words[3];
                // TODO: handle attack action

                bool doesExist = doesCharacterExist(attackerName);
                if (!doesExist) {
                    cout << "Error caught" << endl;
                    continue;
                }

                doesExist = doesCharacterExist(targetName);
                if (!doesExist) {
                    cout << "Error caught" << endl;
                    continue;
                }

                Character* attackerCh = findCharacterByName(attackerName);
                doesExist = attackerCh->doesWeaponExist(weaponName);
                if (!doesExist) {
                    cout << "Error caught" << endl;
                    continue;
                }

                Weapon weapon = attackerCh->getWeapon(weaponName);

                cout << attackerName << " attacks " << targetName << " with their " << weaponName << "!" << endl;

                Character* targetCh = findCharacterByName(targetName);
                targetCh->hp -= weapon.damage;
                if (targetCh->hp <= 0) {
                    cout << targetCh->name << " has died..." << endl;
                    for (int i = 0; i < characters.size(); i++) {
                        if (characters[i].name == targetCh->name) {
                            characters.erase(characters.begin() + i);
                            break;
                        }
                    }
                }


            } else if (words[0] == "Cast") {
                string casterName = words[1];
                string targetName = words[2];
                string spellName = words[3];

                bool doesExist = doesCharacterExist(casterName);
                if (!doesExist) {
                    cout << "Error caught" << endl;
                    continue;
                }

                doesExist = doesCharacterExist(targetName);
                if (!doesExist) {
                    cout << "Error caught" << endl;
                    continue;
                }

                Character* casterCh = findCharacterByName(casterName);
                doesExist = casterCh->doesSpellExist(spellName);
                if (!doesExist) {
                    cout << "Error caught" << endl;
                    continue;
                }

                Spell spell = casterCh->getSpell(spellName);
                bool isAllowed = false;
                for (auto& s : spell.allowedTargets) {
                    if (s == targetName) {
                        isAllowed = true;
                        break;
                    }
                }

                if (!isAllowed) {
                    cout << "Error caught" << endl;
                    continue;
                }

                cout << casterName << " casts " << spellName << " on " << targetName << "!" << endl;
                for (int i = 0; i < casterCh->spellBook.size(); i++) {
                    if (casterCh->spellBook[i].name == spellName) {
                        casterCh->spellBook.erase(casterCh->spellBook.begin() + i);
                        break;
                    }
                }

                cout << targetName << " has died..." << endl;
                for (int i = 0; i < characters.size(); i++) {
                    if (characters[i].name == targetName) {
                        characters.erase(characters.begin() + i);
                        break;
                    }
                }

                // TODO: handle cast action

            } else if (words[0] == "Drink") {
                string supplierName = words[1];
                string drinkerName = words[2];
                string potionName = words[3];
                // TODO: handle cast action

                bool doesExist = doesCharacterExist(supplierName);
                if (!doesExist) {
                    cout << "Error caught" << endl;
                    continue;
                }

                doesExist = doesCharacterExist(drinkerName);
                if (!doesExist) {
                    cout << "Error caught" << endl;
                    continue;
                }

                Character* supplierCh = findCharacterByName(supplierName);
                doesExist = supplierCh->doesPotionExist(potionName);
                if (!doesExist) {
                    cout << "Error caught" << endl;
                    continue;
                }

                Potion potion = supplierCh->getPotion(potionName);

                cout << drinkerName << " drinks " << potionName << " from " << supplierName << "." << endl;
                Character* drinkerCh = findCharacterByName(drinkerName);
                drinkerCh->hp += potion.healValue;
                for (int i = 0; i < supplierCh->medicalBag.size(); i++) {
                    if (supplierCh->medicalBag[i].name == potion.name) {
                        supplierCh->medicalBag.erase(supplierCh->medicalBag.begin() + i);
                        break;
                    }
                }

            } else if (words[0] == "Show") {
                if (words[1] == "characters") {
                    if (characters.empty())
                        cout << endl;
                    for (int i = 0; i < characters.size(); i++) {
                        cout << characters[i];
                        if (i != characters.size() - 1) {
                            cout << " ";
                        } else {
                            cout << " " << endl;
                        }
                    }

                } else if (words[1] == "weapons") {
                    string character = words[2];

                    bool doesExist = doesCharacterExist(character);
                    if (!doesExist) {
                        cout << "Error caught" << endl;
                        continue;
                    }

                    Character* ch = findCharacterByName(character);
                    if (ch->maxArsenal == 0) {
                        cout << "Error caught" << endl;
                        continue;
                    }
                    for (int i = 0; i < ch->arsenal.size(); i++) {
                        cout << ch->arsenal[i];
                        if (i != ch->arsenal.size() - 1) {
                            cout << " ";
                        } else {
                            cout << " " << endl;
                        }
                    }

                    if (ch->arsenal.empty())
                        cout << endl;
                    // TODO: alphabetical order check errors

                } else if (words[1] == "potions") {
                    string character = words[2];

                    bool doesExist = doesCharacterExist(character);
                    if (!doesExist) {
                        cout << "Error caught" << endl;
                        continue;
                    }

                    Character* ch = findCharacterByName(character);
                    if (ch->maxMedicalBag == 0) {
                        cout << "Error caught" << endl;
                        continue;
                    }
                    for (int i = 0; i < ch->medicalBag.size(); i++) {
                        cout << ch->medicalBag[i];
                        if (i != ch->medicalBag.size() - 1) {
                            cout << " ";
                        } else {
                            cout << " " << endl;
                        }
                    }

                    if (ch->medicalBag.empty())
                        cout << endl;
                    // TODO: handle show potions action

                } else if (words[1] == "spells") {
                    string character = words[2];

                    bool doesExist = doesCharacterExist(character);
                    if (!doesExist) {
                        cout << "Error caught" << endl;
                        continue;
                    }

                    Character* ch = findCharacterByName(character);
                    if (ch->maxSpellBook == 0) {
                        cout << "Error caught" << endl;
                        continue;
                    }
                    for (int i = 0; i < ch->spellBook.size(); i++) {
                        cout << ch->spellBook[i];
                        if (i != ch->spellBook.size() - 1) {
                            cout << " ";
                        } else {
                            cout << " " << endl;
                        }
                    }

                    if (ch->spellBook.empty())
                        cout << endl;
                    // TODO: handle show spells action

                }
            }
        }
    }
};

int main() {
    ifstream fin("input.txt");
    cin.rdbuf(fin.rdbuf());

    ofstream fout("output.txt");
    cout.rdbuf(fout.rdbuf());

    Game game;
    game.startAdventure();
    return 0;
}