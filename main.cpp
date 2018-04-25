#include <iostream>
using namespace std;

class Complex
{
private:

	float re;
	float im;

public:

	friend class Vector;
	Complex();
	Complex(float, float);
	Complex(const Complex &);
	Complex& operator=(const Complex &);
	~Complex();
	friend istream& operator>>(istream&, Complex&);
	friend ostream& operator<<(ostream&, Complex&);
	float get();

};

float Complex::get()
{
	return this->im; ///getter pentru partea reala a unui obiect complex
}

Complex::Complex(const Complex &a)
{
	this->re = a.re;
	this->im = a.im; ///constructor de copiere
}

Complex::Complex()
{
	re = im = 0; ///constructor fara parametrii
}

Complex::Complex(float x, float y)
{
	re = x;
	im = y; ///constructor cu parametrii
}

Complex& Complex::operator=(const Complex &a) ///supraincarcarea operatorului de atribuire
{
	if (&a != this) ///daca nu este acelasi obiect
	{
		this->re = a.re;
		this->im = a.im;
	}
	return *this; ///returnez obiectul copiat
}

Complex::~Complex()
{
	im = 0;
	re = 0; ///destructor
}

istream&operator>>(istream&in, Complex&x) ///supraincarcarea citirii
{
	cout << "re=?,im=?" << endl;
	in >> x.re >> x.im;
	return in; ///returnez fluxul de citire
}

ostream&operator<<(ostream&out, Complex&x) ///supraincarcarea afisarii
{
	if (x.im == 0)
		out << x.re << endl;
	if (x.im>0)
		out << x.re << "+i*" << x.im << endl;
	if (x.im<0)
		out << x.re << "-i*" << -x.im << endl;
	return out; ///returnez fluxul
}


class Vector
{
protected:
	int dim;
	Complex *v;

public:
	Vector(int);
	Vector(int, float);
	Vector(const Vector &);
	~Vector();
	Vector& operator=(const Vector &);
	void push(Complex &, int poz);
	friend istream&operator>>(istream&, Vector&);
	friend ostream&operator<<(ostream&, Vector&);
	virtual bool adauga(Complex &, int);
	Complex* get();
	virtual void citire();
	virtual void afisare();
	virtual bool scoate(int);
};

void Vector::afisare()
{
	cout << *this; ///afisare functie virtuala
}

void Vector::citire()
{
	cin >> *this;///functie virtuala
}

Complex* Vector::get()
{
	return (this->v); ///getter pentru vectorul de complexi
}

void Vector::push(Complex &a, int poz)
{
	v[poz] = a; ///metoda pentru a adauga un element
}

Vector::Vector(const Vector &a)
{
	this->dim = a.dim;
	this->v = a.v; /// constructor de copiere
}

Vector::Vector(int n = 0)
{
	dim = n;
	v = new Complex[n]; ///constructor
}

bool Vector::adauga(Complex &a, int nr)
{
	if (nr>dim)
	{
		cout << "Nu se poate" << endl;
		return false; ///daca dimensiunea a fost depasita nu se mai poate adauga
	}
	else
	{
		this->v[nr] = a;
		cout << "Adaugare cu succes!" << endl;
		return true; ///adaugarea a fost facuta
	}

}

bool Vector::scoate(int nr)
{
	if (nr>dim)
	{
		cout << "Nu se poate. Ati depasit dimensiunea vectorului!" << endl;
		return false; ///elementul care a fost cerut sa fie scos nu se afla in vector
	}
	else
	{
		for (int i = nr; i<dim; i++)
			v[i] = v[i + 1];
		dim--;
		cout << "Eliminare cu succes!" << endl;
		return true;
	}
}

Vector::Vector(int n, float x)
{
	dim = n;
	v = new Complex[n];
	for (int i = 0; i<n; i++)
		v[i].re = v[i].im = x;
}

Vector& Vector::operator=(const Vector &a)
{
	if (this != &a)
	{
		this->dim = a.dim;
		this->v = new Complex[a.dim];
		for (int i = 0; i<a.dim; i++)
		{
			v[i].re = a.v[i].re;
			v[i].im = a.v[i].im;
		}
	}
	return *this;
}

istream&operator>>(istream&in, Vector&a) ///supraincarcarea citirii
{
	int n;
	cout << "dimensiune=?" << endl;
	in >> n;
	a.dim = n;
	a.v = new Complex[n];
	for (int i = 0; i<n; i++)
	{
		cin >> a.v[i]; ///citirea complexilor
	}
	return in;
}

ostream&operator<<(ostream&out, Vector&a) ///supraincarcarea afisarii
{
	for (int i = 0; i<a.dim; i++)
		cout << a.v[i];
	return out;
}

Vector::~Vector() ///destructor
{
	dim = 0;
	if (v)
		delete[]v;
}


class Stiva :public Vector
{

	static int Top;

public:
	Stiva(int);
	~Stiva();
	bool adauga(Complex &, int);
	bool scoate(int);
	friend ostream&operator<<(ostream&, Stiva&);
	friend istream& operator>>(istream&, Stiva&);
	bool verifica();
	Stiva&operator=(const Stiva &);
	Stiva(const Stiva &a) :Vector(a) {}
	static int get_Top() { return Top; }
	void citire();
	void afisare();
};
int Stiva::Top; ///initializare Top

void Stiva::afisare()
{
	Vector::afisare(); ///apel metoda virtuala de afisare din Vector
}

void Stiva::citire()
{
	Vector::citire(); ///apel metoda virtuala de citire din Vector
	Top++;
}

Stiva::Stiva(int n = 0) :Vector(n)
{
	Top = n; ///constructor apel din Vector + initializare Top cu nr de elemente
}

Stiva::~Stiva() ///destructor
{
	this->dim = 0;
	if (v) ///daca nu a fost eliminat de catre destructorul clasei Vector
	{
		delete this->v;
		Top--;
	}
}

bool Stiva::verifica() ///metoda care verifica daca exista cel putin un elem cu partea imaginara nula
{
	for (int i = 0; i<this->dim; i++)
		if (this->v[i].get() == 0)
		{
			cout << "Stiva are cel putin un element cu partea imaginara nula" << endl;
			return 0;
		}
	cout << "Stiva nu are niciun element cu partea imaginara nenula" << endl;
	return 1;
}

bool Stiva::adauga(Complex &a, int poz) //nu mergea cu const
{
	if (poz >= this->dim)
	{
		cout << "Stiva este plina. Nu se poate adauga!" << endl;
		return false; ///nu se poata adauga
	}

	else
	{
		this->v[poz] = a;
		cout << "Adaugare cu succes!" << endl;
		Top++;
		return true; ///s-a adaugat
	}
}

bool Stiva::scoate(int poz)
{
	if (poz<0 || poz>this->dim)
	{
		cout << "Nu se poate elimina!" << endl;
		return false; ///pozitia nu a fost ocupata in vector
	}
	else
	{
		for (int i = poz; i<this->dim - 1; i++)
			this->v[i] = this->v[i + 1];
		Top--;
		return true; ///eliminare cu succes
	}
}

ostream&operator<<(ostream&out, Stiva&s) ///supraincarcarea afisarii
{
	for (int i = 0; i<s.dim; i++)
		out << s.v[i];
	return out;
}

istream&operator>>(istream&in, Stiva&s) ///supraincarcarea citirii
{
	cout << "Cate elemente adaugi in stiva?" << endl;
	in >> s.dim;
	Complex aux;
	s.v = new Complex[s.dim];
	for (int i = 0; i<s.dim; i++)
	{
		in >> s.v[i];
		s.Top++;
	}
	return in;
}

Stiva& Stiva:: operator=(const Stiva &a) ///supraincarcarea operatorului de atribuire
{
    if(this!=&a){
	(*this).Vector::operator=(a);///apel supraincarcarea din Vector
    }
	return *this;
}


class Coada :public Vector
{
	static int Top;

public:
	Coada(int);
	bool adauga(Complex &, int);
	bool scoate(int);
	bool verifica();
	friend ostream&operator<<(ostream&, Coada&);
	friend istream& operator>>(istream&, Coada&);
	Coada& operator=(const Coada &);
	Coada(const  Coada &a) :Vector(a) {}
	~Coada();
	static int get_top (){ return Top;}
	void citire();
	void afisare();


};

int Coada::Top; ///initializare Top

void Coada::afisare()
{
	Vector::afisare(); ///apel pentru afisare din Vector
}

void Coada::citire()
{
	Vector::citire(); ///apel citire din Vector
}

Coada::Coada(int n = 0) :Vector(n) ///constructor +apel din Vector al sau
{
	Top = n;
}

bool Coada::adauga(Complex&a, int poz)
{
	if (poz >= this->dim)
	{
		cout << "Nu se poate adauga!" << endl;
		return false; ///nu se poate adauga acest element
	}
	else {
		cout << "Adaugare cu succes!" << endl;
		this->v[poz] = a;
		Top++;
		return true; ///adaugare cu succes
	}
}

bool Coada::scoate(int poz)
{
	if (poz<0 || poz>this->dim)
	{
		cout << "Nu se poate elimina!" << endl;
		return false; ///pozitia nu se afla in Coada
	}
	else {

		for (int i = poz; i<this->dim - 1; i++)
			this->v[i] = this->v[i + 1];
		this->dim--;
		Top--;
		return true; ///eliminare cu succes
	}

}

istream&operator>>(istream&in, Coada&a) ///supraincarcarea citirii
{
	cout << "Dimensiune Coada:" << endl;
	in >> a.dim;
	a.v = new Complex[a.dim];
	for (int i = 0; i<a.dim; i++)
		in >> a.v[i];
	return in;
}

ostream&operator<<(ostream&out, Coada&a) ///supraincarcarea afisarii
{
	for (int i = 0; i<a.dim; i++)
		out << a.v[i];
	return out;
}

Coada::~Coada() ///destructor
{
	this->dim = 0;
	if (v) ///daca nu a fost eliminat de Vector
	{
		delete this->v;
		Top--;
	}
}

bool Coada::verifica() ///metoda care verifica daca exista macar un element in coada cu partea imaginara nula
{
	for (int i = 0; i<this->dim; i++)
		if (this->v[i].get() == 0)
		{
			return false;
			cout << "Coada contine cel putin un element cu partea imaginara nula" << endl;
		}
	cout << "Coada nu contine niciun element cu partea imaginara nula" << endl;
	return true;
}

Coada& Coada::operator=(const Coada &a) ///supraincarcarea op =
{
    if(this!=&a){
	(*this).Vector::operator=(a);
	}
	return *this;
}


int main()
{
	Vector **v;
	Stiva *s;
	Coada *c;
	cout << "Cate elemente doriti sa stocati?" << endl;
	int n, op;
	cin >> n;

	v = new Vector*[n];
	cout << "Buna ziua!" << endl << "Meniul proiectului incepe:" << endl;

	for (int i = 0; i<n; i++)
	{
		cout << "Aveti la dispozitie 2 optiuni." << endl;
		cout << "1.Declarare si citire a unui obiect din clasa Stiva." << endl;
		cout << "2.Declarare si citire a unui obiect din clasa Coada" << endl;
		cin >> op;
		switch (op)
		{
		case 1: {v[i] = new Stiva(); break; }
		case 2: {v[i] = new Coada(); break; }

		}
		v[i]->citire();
	}

	cout << "Meniul s-a inchis. Urmeaza afisarea celor " << n << " obiecte citite." << endl << endl << endl;

	for (int i = 0; i<n; i++)
	{
		cout << "La pasul " << i + 1 << " avem stocate informatiile:" << endl;
		if (Stiva *s = dynamic_cast<Stiva*>(v[i]))
			cout << "In stiva avem obiectele:" << endl;
		if (Coada *c = dynamic_cast<Coada*>(v[i]))
			cout << "In coada avem obiectele:" << endl;
		v[i]->afisare();
	}

	cout << "Finalul programului! Va multumesc!" << endl;
	delete[]v;

	return 0;
}

