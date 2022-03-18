#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <typeindex>
#include <algorithm>

using namespace std;
enum class State { normal };
enum class Race { human };

class PPS 
{
private:	
	const unsigned int ID;
	State state = State::normal;
	Race race = Race::human;
	bool can_move = 1;

	int get_next_ID() {
		static int s{ 0 };
		return s++;
	}

	void Erase() {
		delete[] name;
	}

	void Clone(const PPS& p) {
		name = new char[strlen(p.name) + 1];
		strcpy(name, p.name);
		state = p.state;
		can_move = p.can_move; 
		race = p.race;
	}

protected:
	char* name;

public:
	PPS(const char* _name) : ID(get_next_ID()) { set_name(_name); }
	
	PPS(const PPS& p) : ID(get_next_ID()) {
		Clone(p);
	}	 
	
	virtual ~PPS() {
		Erase();
	}
	
	unsigned int get_ID() const {
		return ID;
	}
	
	void set_name(const char* _name) {
		if (_name == NULL)
			throw exception("Invalid parameter name (NULL)");
		if (_name[0] == '\0')
			throw exception("Invalid parameter name (empty)");
		name = new char[strlen(_name) + 1];
		strcpy(name, _name);
	}

	const char* get_name() const {
		return name;
	}

	virtual string get_info() const {
		string name = get_name();
		return "I'm a PPS. My name is " + name + ".";
	}

	bool operator<(const PPS& p) {
		return strcmp(name, p.name) == -1;
	}

	PPS& operator =(const PPS& p) {
		if (this != &p) {
			Erase(); Clone(p);
		}
		return *this;
	}	
};


//ostream& operator<<(ostream& out, const PPS& p) 
//{
//	string name = p.name;
//	out << "I'm a PPS. My name is " + name;
//	return out;
//}



class WizardPPS : public PPS 
{
private:
	unsigned int max_mana;
	unsigned int mana;
public:
	WizardPPS(const char* _name, unsigned int _max_mana) : PPS(_name), max_mana(_max_mana) , mana(_max_mana) {};
	WizardPPS(const WizardPPS& p) : PPS(p), max_mana(p.max_mana), mana(p.mana) {}
	
	
	//~WizardPPS() override {
	//	cout << "W destructed" << endl;
	//}

	int get_mana() const {
		return mana;
	}

	void set_mana(unsigned int n) {
		if (n <= max_mana) {
			mana = n;
		} else throw exception("Invalid mana value"); 
	}

	string get_info() const override {
		string name = get_name();
		return "I'm a Wizard. My name is " + name + ". My mana is " + to_string(get_mana());
	}		
};

struct cmp {
	bool operator() (const PPS* a, const PPS* b) const {
		return !a || !b ? 0 : strcmp(a->get_name(), b->get_name()) == -1;
	}
};

int main()
{
	unsigned int count_wizard = 0;
	unsigned int count_pps = 0;
	vector<PPS*> persons;
	persons.push_back(new PPS("kl"));
	persons.push_back(new PPS("abc"));
	persons.push_back(new PPS("aab"));
	persons.push_back(new WizardPPS("d", 134));
	persons.push_back(new WizardPPS("abbc", 333));
	//persons.push_back(nullptr);

	PPS w1("kbh");
	PPS w2(w1);
	
	persons.push_back(&w2);

	std::sort(persons.begin(), persons.end(), cmp());

	for (unsigned int i = 0; i < persons.size(); i++) {
		if (persons[i]) {
			cout << persons[i]->get_ID() << ":" << persons[i]->get_info() << endl;
			type_index ti(typeid(*persons[i]));
			if (ti == type_index(typeid(WizardPPS))) {
				count_wizard++;
			}
			else	if (ti == type_index(typeid(PPS))) {
				count_pps++;
			}
		}
	}
	cout << count_pps << "  " << count_wizard;
	
}