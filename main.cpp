#include<iostream>
#include<fstream>
#include<string>
#include<limits>
#include<exception>
#include<assert.h>

using namespace std;

class Bazowe_Cechy {
public:
	static const bool _jest_liczba = false;
	static const bool _nalezy_do_przedzialu = false;
	static const bool _jest_liczba_calkowita = false;
	static const int _dolna_granica_przedzialu = 0;
	static const int _gorna_granica_przedzialu = 0;
	static const bool _jest_parzysta = false;
};

template<typename T>
class Cechy: public Bazowe_Cechy {
public:
	static const double dolna_granica_przedzialu() { return 0; };
	static const double gorna_granica_przedzialu() { return 0; };
};

class temperatura_wody {
	double n;
public:
	temperatura_wody(double temp = 50): n(temp) {};
	double operator()() { return n; };
	temperatura_wody& operator=(double temp) { n = temp; return *this; };
	friend ostream& operator<<(ostream& out,temperatura_wody& arg){
		out<<arg.n;
			return out;
		}
};

template<>
class Cechy<temperatura_wody>: public Bazowe_Cechy {
public:
	static const bool _jest_liczba = true;
	static const bool _nalezy_do_przedzialu = true;
	static const double dolna_granica_przedzialu() { return 0; };
	static const double gorna_granica_przedzialu() { return 100; };
};

class cel {
	double n;
public:
	cel(double temp = 50): n(temp) {};
	double operator()() { return n; };
	cel& operator=(double temp) { n = temp; return *this; };
	friend ostream& operator<<(ostream& out,cel& arg){
		out<<arg.n;
			return out;
		}
};

template<>
class Cechy<cel>: public Bazowe_Cechy {
public:
	static const bool _jest_liczba = true;
	static const bool _nalezy_do_przedzialu = true;
	static const double dolna_granica_przedzialu() { return -273.17; };
	static const double gorna_granica_przedzialu() { return DBL_MAX; };
};

class kostka_do_gry {
	int n;
public:
	kostka_do_gry(int num = 1): n(num) {};
	int operator()() { return n; };
	kostka_do_gry& operator=(int num) { n = num; return *this; };
	friend ostream& operator<<(ostream& out,kostka_do_gry& arg){
		out<<arg.n;
			return out;
		}
};

template<>
class Cechy<kostka_do_gry>: public Bazowe_Cechy  {
public:
	static const bool _jest_liczba = true;
	static const bool _nalezy_do_przedzialu = true;
	static const bool _jest_liczba_calkowita = true;
	static const int _dolna_granica_przedzialu = 1;
	static const int _gorna_granica_przedzialu = 6;

};

class nat {
	int n;
public:
	nat(int num = 1): n(num) {};
	int operator()() { return n; };
	nat& operator=(int num) { n = num; return *this; };
	friend ostream& operator<<(ostream& out,nat& arg){
		out<<arg.n;
			return out;
		}
};

template<>
class Cechy<nat>: public Bazowe_Cechy  {
public:
	static const bool _jest_liczba = true;
	static const bool _nalezy_do_przedzialu = true;
	static const bool _jest_liczba_calkowita = true;
	static const int _dolna_granica_przedzialu = 0;
	static const int _gorna_granica_przedzialu = INT_MAX;
};

class parz {
	int n;
public:
	parz():n(2){};
	parz(int num):n(num){};
	int operator()() { return n; };
	parz& operator=(int num) { n = num; return *this; };
	friend ostream& operator<<(ostream& out,parz& arg){
		out<<arg.n;
			return out;
		}
};

template<>
class Cechy<parz>: public Bazowe_Cechy  {
public:
	static const bool _jest_liczba = true;
	static const bool _nalezy_do_przedzialu = true;
	static const bool _jest_liczba_calkowita = true;
	static const int _dolna_granica_przedzialu = 2;
	static const int _gorna_granica_przedzialu = INT_MAX-1;
	static const bool _jest_parzysta = true;
};

class Przepelnienie: public exception {
	char opis[100];
public:
	Przepelnienie(const char* o) { strcpy_s(opis,o); }
	const char* what() const throw() { return opis;	};
};
class BrakDanych: public exception {
	char opis[100];
public:
	BrakDanych(const char* o) { strcpy_s(opis,o); }
	const char* what() const throw() { return opis;	};
};


template<typename T, int rozmiar, class _Cechy = Cechy<T>>
class SzablonStosu{ 
	public:
	T stos[rozmiar];
	int top;
	int zajetosc() { return top; };
	SzablonStosu() : top(0) {}
	void push(const T& i) {
		if(top==rozmiar)
			throw Przepelnienie(typeid(i).name());
		stos[top++] = i;
	}
	void push(int i) {
		if(top==rozmiar)
			throw Przepelnienie(typeid(i).name());
		if(Cechy<T>::_jest_liczba && Cechy<T>::_jest_liczba_calkowita) {
			if(Cechy<T>::_nalezy_do_przedzialu) {
				if((Cechy<T>::_dolna_granica_przedzialu <= i) && (i <= Cechy<T>::_gorna_granica_przedzialu)){
					if(Cechy<T>::_jest_parzysta){
						if(i%2==0)
							stos[top++] = i;
					}else
						stos[top++] = i;
				}
			}else
				stos[top++] = i;
		}
	}
	void push(double i) {
		if(top==rozmiar)
			throw Przepelnienie(typeid(i).name());
		if(Cechy<T>::_jest_liczba && !Cechy<T>::_jest_liczba_calkowita) {
			if(Cechy<T>::_nalezy_do_przedzialu) {
				if((Cechy<T>::dolna_granica_przedzialu() <= i) && (i <= Cechy<T>::gorna_granica_przedzialu()))
					stos[top++] = i;
			} else
				stos[top++] = i;
		}
	}
	T pop() {
		if(top==0)
			throw BrakDanych(typeid(stos[0]).name());
		return stos[--top];
	}
	class iterator {
		public:	
		SzablonStosu& s;
		int index;
		
		iterator(SzablonStosu& is) : s(is), index(0) {}
		iterator(SzablonStosu& is, bool) : s(is), index(is.top) {}
		T operator++() { // Prefix
			assert(index < s.top); 
			return s.stos[++index];
		}
		T operator++(int) { // Postfix
			assert(index < s.top);
			return s.stos[index++];
		}
		bool operator==(const iterator& rv) const {
			return index == rv.index;
		}
		bool operator!=(const iterator& rv) const {
			return index != rv.index;
		} 
		T operator*() const { return s.stos[index];}
	}; 
	iterator begin() { return iterator(*this); }
	iterator end() { return iterator(*this, true);}
	friend class iterator;

};

template<typename T, int rozmiar>
void wypisz(ostream& out,SzablonStosu<T,rozmiar>& arg) {
	SzablonStosu<T,rozmiar>::iterator it(arg);
	while (it!=arg.end()) {
		out<<*it<<' ';
		it++;
	}
}
int main() {
	SzablonStosu<string,10> K1;
	SzablonStosu<temperatura_wody,10> K2;
	SzablonStosu<kostka_do_gry,10> K3;
	SzablonStosu<parz,10> K4;
	SzablonStosu<nat,10> K5;
	SzablonStosu<cel,10> K6;
	ifstream fi("qv.txt");
	string s;
	try{
		K1.push("Henryk");
		K1.push("Sienkiewicz");
		/*while (fi) {
			fi >> s; 
			K1.push(s);
			fi.seekg(ios::beg);
			fi.clear();
		};*/
	}
	catch(Przepelnienie& e){
		cout << "K1 gotowy: " << e.what() << endl;
	};
	cout << "Danych na stosie K1: " << K1.zajetosc() << endl;

	K2.push(temperatura_wody());
	K2.push(temperatura_wody(36.6));
	K2.push(40.0);
	K2.push(71.2);
	cout << "Danych na stosie K2: " << K2.zajetosc() << endl;

	K3.push(kostka_do_gry(3));
	K3.push(kostka_do_gry());
	K3.push(4);
	K3.push(6);
	K3.push(10);
	cout << "Danych na stosie K3: " << K3.zajetosc() << endl;

	K4.push(parz(3));
	K4.push(parz());
	K4.push(3);
	K4.push(6);
	K4.push(10);
	cout << "Danych na stosie K4: " << K4.zajetosc() << endl;

	K5.push(nat(3));
	K5.push(nat());
	K5.push(-3);
	K5.push(6);
	K5.push(10);
	cout << "Danych na stosie K5: " << K5.zajetosc() << endl;

	K6.push(cel(3));
	K6.push(cel());
	K6.push(-334.2);
	K6.push(6.5);
	K6.push(10.1);
	cout << "Danych na stosie K6: " << K6.zajetosc() << endl;


	wypisz<string,10>(cout,K1); cout<<'\n';
	wypisz<temperatura_wody,10>(cout,K2); cout<<'\n';
	wypisz<kostka_do_gry,10>(cout,K3); cout<<'\n';
	wypisz<parz,10>(cout,K4); cout<<'\n';
	wypisz<nat,10>(cout,K5); cout<<'\n';
	wypisz<cel,10>(cout,K6); cout<<'\n';
	// opró¿nianie stosu
	try{
		while (true)
			K1.pop();
	}
	catch(BrakDanych& e) {
		cout << "K1 pusty: " << e.what() << endl;
	}
	try{
		while (true)
			K2.pop();
	}
	catch(BrakDanych& e) {
		cout << "K2 pusty: " << e.what() << endl;
	}
	try{
		while (true)
			K3.pop();
	}
	catch(BrakDanych& e) {
		cout << "K3 pusty: " << e.what() << endl;
	}

	return 0;
}