#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
void afisaretimp(int t) {
	cout << '[';
	if (t / 60 % 24 < 10) cout << '0' << t / 60 % 24 << ':';
	else cout << t / 60 % 24 << ':';
	if (t % 60 < 10) cout << '0' << t % 60;
	else cout << t % 60;
	cout << "] ";
}

class pozitie {
	int x, y;
public:

	int getx() const {
		return x;
	}
	int gety() const {
		return y;
	}
	void setx(double x) {
		this->x = x;
	}
	void sety(double y) {
		this->y = y;
	}
	pozitie() :x(0), y(0) {};
	pozitie(int x, int  y) :x(x), y(y) {};
	double operator-(pozitie p) {
		return sqrt(1LL * (this->x - p.x) * (this->x - p.x) + 1LL * (this->y - p.y) * (this->y - p.y));
	}
	friend static istream& operator>>(istream& in, pozitie& p) {
		return in >> p.x >> p.y;
	}
	friend static ostream& operator<<(ostream& out, pozitie p) {
		return out << "x = " << p.x << "\ny = " << p.y << '\n';
	}
};
class dimensiune {
	float mas, vol;
public:
	float getmasa() const {
		return mas;
	}
	float getvolum() const {
		return vol;
	}
	dimensiune operator-(dimensiune d) {
		d.vol = this->vol - d.vol;
		d.mas = this->mas - d.mas;
		return d;
	}
	dimensiune operator-=(dimensiune d) {
		this->mas -= d.mas;
		this->vol -= d.vol;
		return *this;
	}
	dimensiune operator+(dimensiune d) {
		d.vol = this->vol + d.vol;
		d.mas = this->mas + d.mas;
		return d;
	}
	dimensiune operator+=(dimensiune d) {
		this->mas += d.mas;
		this->vol += d.vol;
		return *this;
	}
	friend istream& operator>>(istream& in, dimensiune& d) {
		return in >> d.mas >> d.vol;
	}
	dimensiune() :mas(0), vol(0) {};
	dimensiune(int x, int y) :mas(x), vol(y) {};

};
class pachet {
	pozitie pozr, pozl;
	dimensiune dim;
	short id;
	bool rece;
	int tr, tl, liv;
public:
	pachet(int i, int t) :pozl(), pozr(), liv(0), dim(), id(i), rece(0), tl(0), tr(t) {};
	pachet(pachet& p) {
		pozr = p.pozr, pozl = p.pozl, id = p.id, dim = p.dim, rece = p.rece, tr = p.tr, tl = p.tl, liv = p.liv;
	}
	friend istream& operator>>(istream& in, pachet& p) {
		in >> p.dim >> p.rece >> p.pozr >> p.pozl >> p.liv;
		if (p.liv != 3) return in >> p.tl;
		else return in;
	}
	bool isrece() const {
		return rece;
	}
	dimensiune getdim() const {
		return dim;
	}
	pozitie getrid() const {
		return pozr;
	}
	pozitie getliv() const {
		return pozl;
	}
	int getid() const {
		return id;
	}
	int getlim() const {
		return liv;
	}
	int gettmpr() const {
		return tr;
	}
	int getmas() const {
		return dim.getmasa();
	}
	float getvol() const {
		return dim.getvolum();
	}
	int gettmpl() const {
		if (liv == 3) return -1;
		return tl;
	}
};
class vehicul {
protected:
	static int const vit = 1;
	pozitie poz, pozdest;
	dimensiune dim;
	pachet* p[100];
	int id, s, pac, t[100];
	bool pedrum, lr;
public:
	vehicul(int id) : id(id), pozdest(0, 0), pac(0), pedrum(0), lr(0) {};
	vehicul(vehicul& v) : poz(v.poz), pozdest(v.pozdest), dim(v.dim), id(v.id), s(v.s), pac(v.pac), pedrum(v.pedrum), lr(v.lr) {
		for (int i = 0; i < v.pac; i++) {
			t[i] = v.t[i];
			p[i] = new pachet(*v.p[i]);
		}
	}
	virtual ~vehicul() {
		for (int i = 0; i < pac; i++)
			delete p[i];
	}
	vehicul& operator=(vehicul& v) {
		for (int i = 0; i < v.pac; i++) {
			delete p[i];
		}
		poz = v.poz;
		pozdest = v.pozdest;
		dim = v.dim;
		id = v.id;
		s = v.s;
		pac = v.pac;
		pedrum = v.pedrum;
		lr = v.lr;
		for (int i = 0; i < v.pac; i++) {
			t[i] = v.t[i];
			p[i] = new pachet(*v.p[i]);
		}
		return *this;
	}
	friend istream& operator>>(istream& in, vehicul& v) {
		return in >> v.dim >> v.poz >> v.s;
	}
	void sprrid() {
		lr = 0;
	}
	void sprliv() {
		lr = 1;
	}
	bool PeDrum() const {
		return pedrum;
	}
	int getid() const {
		return id;
	}
	int getnrpac() const {
		return pac;
	}
	void plecare() {
		pedrum = 1;
	}
	float getmas() const {
		return dim.getmasa();
	}
	float getvol() const {
		return dim.getvolum();
	}
	void setStrat(int s) {
		this->s = s;
	}
	void adagpac(pachet* p, int t, int poz) {
		if (poz > 99) throw poz;
		if (poz < pac) delete this->p[poz];
		this->p[poz] = new pachet(*p);
		this->t[poz] = t;
		pac++;
		this->ocuparespatiu(p->getdim());
	}
	pachet* getpac(int i) const {
		return p[i];
	}
	int gett(int i) const {
		return this->t[i];
	}
	pozitie getpoz() const {
		return this->poz;
	}
	pozitie getdest() const {
		return pozdest;
	}
	int getstrat() const {
		return s;
	}
	void ocuparespatiu(dimensiune d) {
		dim -= d;
	}
	void eliberarespatiu(dimensiune d) {
		dim += d;
	}
	void schimbdest(pozitie poz) {
		pozdest = poz;
	}
	void schpoz(pozitie poz) {
		this->poz = poz;
	}
	void recalctimp(int poz) {
		for (int i = poz + 1; i < pac; i++) {
			this->t[i] = this->calctimp(this->getpac(i)->getliv(), this->getpac(i - 1)->getrid()) + this->calctimp(this->getpac(i)->getrid(), this->getpac(i)->getliv());
		}
	}
	void mutarepac(int i, int j) {
		if (pac > i) delete p[i];
		this->p[i] = new pachet(*this->getpac(i - j));
		this->t[i] = this->t[i - j];
	}
	virtual double calctimp(pozitie poz1, pozitie poz2) = 0;
	virtual bool incapepac(pachet* p) {
		if (this->getmas() >= p->getmas() and this->getvol() >= p->getvol() and p->isrece() == 0) return 1;
		else return 0;
	}
	bool verif(int timp) const {
		int t = timp + this->t[0];
		for (int i = 1; i < pac; i++) {
			t += this->t[i];
			if (this->getpac(i)->getlim() == 1 and t > this->getpac(i)->gettmpl()) {
				return 0;
			}
		}
		return 1;
	}
	int gasiretimp(pachet* p) {
		int t = 0;
		if (this->PeDrum() == 0) return this->calctimp(this->getpoz(), p->getrid()) + this->calctimp(p->getrid(), p->getliv());
		else {
			int poz = this->gasirepoz(p);
			this->adaugarepachet(p, poz);
			for (int i = 0; i <= poz; i++) {
				t += this->gett(i);
			}
			return t;
		}
	}
	int gasirepoz(pachet* p) const {
		int s = this->getstrat();
		int pac = this->getnrpac();
		if (s == 3) return pac;
		if (s == 2) {
			for (int i = 1; i < pac; i++) {
				if (this->getpac(0)->getliv() - p->getrid() <= this->getpac(0)->getliv() - this->getpac(i)->getrid()) return i;
			}
			return pac;
		}
		if (s == 1) {
			if (p->getlim() == 1) {
				for (int i = 1; i < pac; i++) {
					if (this->getpac(i)->getlim() == 1) {
						if (this->getpac(i)->gettmpl() >= p->gettmpl()) return i;
					}
				}
				return pac;
			}
			else if (p->getlim() == 2) {
				for (int i = 1; i < pac; i++) {
					if (this->getpac(i)->getlim() == 2) {
						if (this->getpac(i)->gettmpl() + this->getpac(i)->gettmpr() >= p->gettmpl() + p->gettmpr()) return i;
					}
				}
				return pac;
			}
			else return pac;
		}
	}
	void adaugarepachet(pachet* p, int poz) {
		int pac = this->getnrpac();
		if (poz != pac) {
			for (int i = pac; i > poz; i--) {
				this->mutarepac(i, 1);
			}
		}
		if (poz == 0) {
			try {
				this->adagpac(p, this->calctimp(this->getpoz(), p->getrid()) + this->calctimp(p->getrid(), p->getliv()), poz);
			}
			catch (int x) {
				cout << "Vehiculul nu mai are loc pentru pachet";
			}
		}
		else {
			try {
				this->adagpac(p, this->calctimp(this->getpac(poz - 1)->getliv(), p->getrid()) + this->calctimp(p->getrid(), p->getliv()), poz);
			}
			catch (int x) {
				cout << "Vehiculul nu mai are loc pentru pachet";
			}
		}
		this->plecare();
		this->recalctimp(poz);
	}
	virtual int calcmpm() {
		return vit * 1000 / 60;
	}
	bool veriftimp(pachet* p, int t);
	void updateveh(int t) {
		if (pedrum == 0) return;
		if (this->t[0] == 1) {
			afisaretimp(t);
			if (this->lr == 0) {
				cout << "Vehicului cu id-ul " << this->id << " a ridicat si livrat pachetul cu id-ul " << this->getpac(0)->getid() << '\n';
			}
			else {
				cout << "Vehicului cu id-ul " << this->id << " a livrat pachetul cu id-ul " << this->getpac(0)->getid() << '\n';
			}
			this->sprrid();
			this->schpoz(this->getpac(0)->getliv());
			this->eliberarespatiu(this->getpac(0)->getdim());
			if (pac > 1) {
				for (int j = 0; j < pac - 1; j++) {
					this->mutarepac(j, -1);
				}
				this->schimbdest(this->getpac(0)->getrid());
			}
			else pedrum = 0;
			pac--;
		}
		else {
			int dist = pozdest - poz;
			float mpm = this->calcmpm();
			if (mpm >= dist) {
				afisaretimp(t);
				this->schpoz(this->getdest());
				this->schimbdest(this->getpac(0)->getliv());
				this->sprliv();
				poz.setx(1.0 * (poz.getx() + (pozdest.getx() - poz.getx()) / this->t[0] * (mpm - dist) / mpm));
				poz.sety(1.0 * (poz.gety() + (pozdest.gety() - poz.gety()) / this->t[0] * (mpm - dist) / mpm));
				cout << "Vehicului cu id-ul " << this->id << " a ridicat pachetul cu id-ul " << this->getpac(0)->getid();
				cout << " si va ajunge la destinatie la ora ";
				if ((this->t[0] - 1 + t) / 60 % 24 < 10) cout << '0' << (this->t[0] + t - 1) / 60 % 24 << ':';
				else cout << (this->t[0] + t) / 60 % 24 << ':';
				if ((t + this->t[0] - 1) % 60 < 10) cout << '0' << (t + this->t[0] - 1) % 60;
				else cout << (t + this->t[0] - 1) % 60;
				if ((this->t[0] - 1 + t) / 60 / 24 == 1) cout << " in ziua urmatoare";
				else if ((this->t[0] - 1 + t) / 60 / 24 > 1) {
					cout << " peste " << (this->t[0] - 1 + t) / 60 / 24 << " zile";
				}
				cout << '\n';
			}
			else {
				if (lr) {
					poz.setx(1.0 * (poz.getx() + (pozdest.getx() - poz.getx()) / this->t[0]));
					poz.sety(1.0 * (poz.gety() + (pozdest.gety() - poz.gety()) / this->t[0]));
				}
				else {
					int timp = this->calctimp(pozdest, poz);
					if (poz.getx() - pozdest.getx() < 0) poz.setx(1.0 * (poz.getx() + (pozdest.getx() - poz.getx()) / timp));
					else poz.setx(1.0 * (poz.getx() - abs((pozdest.getx() - poz.getx())) / timp));
					if (poz.gety() - pozdest.gety() < 0) poz.sety(1.0 * (poz.gety() + (pozdest.gety() - poz.gety()) / timp));
					else poz.sety(1.0 * (poz.gety() - abs((pozdest.gety() - poz.gety()) / timp)));
				}
			}
			this->t[0]--;
		}
	}
};
class scuter :public vehicul {
	static int const vit = 20;
public:
	scuter(int id) : vehicul(id) {};
	int calcmpm() {
		return vit * 1000 / 60;
	}
	double calctimp(pozitie poz1, pozitie poz2) {
		return floor((poz2 - poz1) * 60) / vit / 1000;
	}
};
class masina :public vehicul {
	static int const vit = 10;
public:
	masina(int id) : vehicul(id) {};
	masina(scuter& s) :vehicul(s) {};
	int calcmpm() {
		return vit * 1000 / 60;
	}
	double calctimp(pozitie poz1, pozitie poz2) {
		return floor((poz2 - poz1) * 60) / vit / 1000;
	};
};
class duba :public vehicul {
	static int const vit = 5;
public:
	duba(int id) : vehicul(id) {};
	int calcmpm() {
		return vit * 1000 / 60;
	}
	double calctimp(pozitie poz1, pozitie poz2) {
		return floor((poz2 - poz1) * 60) / vit / 1000;
	}
};
class dubafrig :public duba {
	static int const vit = 5;
public:
	dubafrig(int id) : duba(id) {};
	int calcmpm() {
		return vit * 1000 / 60;
	}
	double calctimp(pozitie poz1, pozitie poz2) {
		return floor((poz2 - poz1) * 60) / vit / 1000;
	}
	virtual bool incapepac(pachet* p) {
		if (this->getmas() >= p->getmas() and this->getvol() >= p->getvol()) return 1;
		else return 0;
	}
};
bool test(vehicul* v, pachet* p, int poz, int t) {
	if (dynamic_cast<scuter*>(v)) {
		scuter vaux = *(dynamic_cast<scuter*>(v));
		vaux.adaugarepachet(p, poz);
		return vaux.verif(t);
	}
	else if (dynamic_cast<masina*>(v)) {
		masina vaux = *(dynamic_cast<masina*>(v));
		vaux.adaugarepachet(p, poz);
		return vaux.verif(t);
	}
	else if (dynamic_cast<dubafrig*>(v)) {
		dubafrig vaux = *(dynamic_cast<dubafrig*>(v));
		vaux.adaugarepachet(p, poz);
		return vaux.verif(t);
	}
	else if (dynamic_cast<duba*>(v)) {
		duba vaux = *(dynamic_cast<duba*>(v));
		vaux.adaugarepachet(p, poz);
		return vaux.verif(t);
	}
}
class manager {
	vehicul* v[100];
	int vnr = 0, s;
	manager() {}
	manager(const manager&) = delete;
	manager& operator=(const manager&) = delete;
	static manager* instance;
	bool schstrateg(int i, int t) {
		if (v[i]->PeDrum() == 0 or v[i]->getnrpac() <= 2) {
			v[i]->setStrat(s);
			return 1;
		}
		int poz;
		vehicul* vaux = new scuter(1000);
		if (dynamic_cast<scuter*>(v[i])) {
			vaux = new scuter(v[i]->getid());
			vaux->setStrat(s);
			vaux->schpoz(v[i]->getpoz());
			vaux->adaugarepachet(v[i]->getpac(0), 0);
			for (int i = 1; i < v[i]->getnrpac(); i++) {
				poz = vaux->gasirepoz(v[i]->getpac(i));
				if (test(vaux, v[i]->getpac(i), poz, t + v[i]->gett(0))) {
					vaux->adaugarepachet(v[i]->getpac(i), poz);
				}
				else return 0;
			}
			for (int i = 1; i < vaux->getnrpac(); i++) {
				v[i]->adagpac(vaux->getpac(i), vaux->gett(i), i);
			}
			v[i]->setStrat(s);
			return 1;
		}
		else if (dynamic_cast<masina*>(v[i])) {
			vaux = new masina(v[i]->getid());
			vaux->setStrat(s);
			vaux->schpoz(v[i]->getpoz());
			vaux->adaugarepachet(v[i]->getpac(0), 0);
			for (int i = 1; i < v[i]->getnrpac(); i++) {
				poz = vaux->gasirepoz(v[i]->getpac(i));
				if (test(vaux, v[i]->getpac(i), poz, t + v[i]->gett(0))) {
					vaux->adaugarepachet(v[i]->getpac(i), poz);
				}
				else return 0;
			}
			for (int i = 1; i < vaux->getnrpac(); i++) {
				v[i]->adagpac(vaux->getpac(i), vaux->gett(i), i);
			}
			v[i]->setStrat(s);
			return 1;
		}
		else if (dynamic_cast<duba*>(v[i])) {
			vaux = new duba(v[i]->getid());
			vaux->setStrat(s);
			vaux->schpoz(v[i]->getpoz());
			vaux->adaugarepachet(v[i]->getpac(0), 0);
			for (int i = 1; i < v[i]->getnrpac(); i++) {
				poz = vaux->gasirepoz(v[i]->getpac(i));
				if (test(vaux, v[i]->getpac(i), poz, t + v[i]->gett(0))) {
					vaux->adaugarepachet(v[i]->getpac(i), poz);
				}
				else return 0;
			}
			for (int i = 1; i < vaux->getnrpac(); i++) {
				v[i]->adagpac(vaux->getpac(i), vaux->gett(i), i);
			}
			v[i]->setStrat(s);
			return 1;
		}
		else if (dynamic_cast<dubafrig*>(v[i])) {
			vaux = new dubafrig(v[i]->getid());
			vaux->setStrat(s);
			vaux->schpoz(v[i]->getpoz());
			vaux->adaugarepachet(v[i]->getpac(0), 0);
			for (int i = 1; i < v[i]->getnrpac(); i++) {
				poz = vaux->gasirepoz(v[i]->getpac(i));
				if (test(vaux, v[i]->getpac(i), poz, t + v[i]->gett(0))) {
					vaux->adaugarepachet(v[i]->getpac(i), poz);
				}
				else return 0;
			}
			for (int i = 1; i < vaux->getnrpac(); i++) {
				v[i]->adagpac(vaux->getpac(i), vaux->gett(i), i);
			}
			v[i]->setStrat(s);
			return 1;
		}
	}
	~manager() {}
public:
	static manager& getinstanta() {
		static manager instanta;
		return instanta;
	}
	void setStrat(int s) {
		this->s = s;
	}
	vehicul* const getVehicul(int i) {
		return v[i];
	}
	void citireVehicul(istream* in) {
		int tip;
		cout << "Ce tip de vehicul? (1=scuter, 2=masina, 3=duba, 4=duba frigorifica): \n";
		*in >> tip;
		cout << "Introduceti datele vehiculului (masa, volum, pozitie, strategie): \n";
		if (tip == 1) {

			v[vnr] = new scuter(vnr + 1);
		}
		if (tip == 2) {
			v[vnr] = new masina(vnr + 1);
		}
		if (tip == 3) {
			v[vnr] = new duba(vnr + 1);
		}
		if (tip == 4) {
			v[vnr] = new dubafrig(vnr + 1);
		}
		*in >> *v[vnr];
		vnr++;
	}
	void citirePachet(istream* in, int& i, int t) {
		pachet* p = new pachet(i, t);
		i++;
		afisaretimp(t);
		cout << "Introduceti datele pachetului (masa, volumul, 1 daca trebuie tinut la rece, 0 altfel, pozitia de ridicare, pozitia de livrare, tipul limitei de timp, timpul limita(daca are)): \n";
		*in >> *p;
		int x = this->asignpac(p, t);
		afisaretimp(t);
		if (x == 0) {
			cout << "Pachetul " << i << " nu poate fi livrat la timp sau nu este loc pentru el in niciun vehicul, ne pare rau.\n";
		}
		else cout << "Pachetul " << i << " va fi preluat de vehiculul " << x << '\n';
		delete p;
	}
	void strategief(pachet* p) {
		if (s == 1) {
			for (int i = 0; i < vnr; i++) {
				for (int j = 0; j < vnr; j++) {
					if (p->getrid() - v[i]->getpoz() > p->getrid() - v[j]->getpoz()) {
						swap(v[i], v[j]);
					}
				}
			}
		}
		if (s == 2) {
			int t[100];
			for (int i = 0; i < vnr; i++) t[i] = v[i]->gasiretimp(p);
			for (int i = 0; i < vnr; i++) {
				for (int j = 0; j < vnr; j++) {
					if (t[i] > t[j]) {
						swap(v[i], v[j]);
					}
				}
			}
		}
		if (s == 3) {
			for (int i = 0; i < vnr; i++) {
				for (int j = 0; j < vnr; j++) {
					if (v[i]->getmas() < v[j]->getmas()) {
						swap(v[i], v[j]);
					}
				}
			}
		}
		return;
	}
	void schStrat(int id, int saux, int t) {
		if (id == 0) {
			s = saux;
			cout << "Strategia a fost schimbata cu success! \n";
		}
		else {
			for (int i = 0; i < vnr; i++) {
				if (v[i]->getid() == id - 1) {
					if (this->schstrateg(id - 1, t)) {
						cout << "Strategia a fost schimbata cu success! \n";
					}
					else cout << "Strategia nu a putut fi schimbata. \n";
				}
			}
		}
	}
	int asignpac(pachet* p, int t) {
		this->strategief(p);
		for (int i = 0; i < vnr; i++) {
			if (v[i]->incapepac(p)) {
				if (v[i]->veriftimp(p, t)) {
					return v[i]->getid();
				}
			}
		}
		return 0;
	}
	static void del() {
		getinstanta().~manager();
	}
};
void swap(vehicul* v1, vehicul* v2) {
	vehicul* aux = v1;
	v1 = v2;
	v2 = aux;
}
bool vehicul::veriftimp(pachet* p, int t) {
	if (pedrum == 0) {
		if (p->getlim() == 3) {
			this->adaugarepachet(p, 0);
			this->schimbdest(p->getrid());
			this->sprrid();
			return 1;
		}
		if (p->getlim() == 2) {
			if (p->gettmpl() >= this->calctimp(p->getrid(), p->getliv())) {
				this->adaugarepachet(p, 0);
				this->schimbdest(p->getrid());
				this->sprrid();
				return 1;
			}
			else return 0;
		}
		if (p->getlim() == 1) {
			if (p->gettmpl() >= this->calctimp(this->getpoz(), p->getliv()) + this->calctimp(p->getrid(), p->getliv()) + t) {
				this->adaugarepachet(p, 0);
				this->schimbdest(p->getrid());
				this->sprrid();
				return 1;
			}
			else return 0;
		}
	}
	else {
		if (p->getlim() == 2) {
			if (p->gettmpl() < this->calctimp(p->getliv(), p->getrid()))
				return 0;
		}
		int poz = this->gasirepoz(p);
		if (test(this, p, poz, t)) {
			this->adaugarepachet(p, poz);
			return 1;
		}
		else return 0;
	}

}

int main(int argc, char* argv[]) {
	istream* in;
	if (argc <= 1) {
		in = &cin;
	}
	else {
		in = new fstream(argv[1]);
	}
	manager& mng = manager::getinstanta();
	int t = 0, n, s, nr, pi = 0, saux, id, taux;
	cout << "Cate vehicule vor lucra astazi: \n";
	*in >> n;
	for (int j = 0; j < n; j++) {
		mng.citireVehicul(in);
	}
	cout << "\nIntroduceti strategia firmei de ziua de astazi: \n";
	*in >> s;
	mng.setStrat(s);
	int index1 = 0, index2 = 0;
	for (t = 0; t <= 1440;) {
		afisaretimp(t);
		cout << "Ce doriti sa faceti:\n 1.Introduceti un vehicul nou. \n 2.Introduceti un pachet. \n 3.Schimbati o strategie. \n 4.Asteptati pana la un timp. \n 5.Aflati pozitia unui vehicul.\n 6.Simulati ziua cu aceste informatii pana la final.\n";
		*in >> nr;
		if (nr == 1) {
			afisaretimp(t);
			mng.citireVehicul(in);
			afisaretimp(t);
			cout << "Vehiculul a fost inregistrat.\n";
		}
		if (nr == 2) {
			mng.citirePachet(in, pi, t);
		}
		if (nr == 3) {
			afisaretimp(t);
			cout << "Introduceti vehiculul caruia doriti sa ii schimbati strategia sau 0 daca este strategia firmei, apoi strategia in care doriti sa schimbati: \n";
			afisaretimp(t);
			*in >> id >> saux;
			mng.schStrat(id, saux, t);
		}
		if (nr == 4) {
			afisaretimp(t);
			cout << "Introduceti timpul pana la care doriti sa asteptati: \n";
			*in >> taux;
			if (taux < t) {
				afisaretimp(t);
				cout << "Timpul acela este mai mic decat timpul curent.\n";
			}
			else {
				for (; t < taux; t++) {
					for (int i = 0; i < n; i++) {
						mng.getVehicul(i)->updateveh(t);
					}
				}
			}
		}
		if (nr == 5) {
			afisaretimp(t);
			cout << "Introduceti id-ul vehicului caruia doriti sa ii aflati pozitia: \n";
			afisaretimp(t);
			*in >> id;
			for (int i = 0; i < n; i++) {
				if (mng.getVehicul(i)->getid() == id) {
					cout << mng.getVehicul(i)->getpoz();
				}
			}
		}
		if (nr == 6) {
			for (; t <= 1440; t++) {
				for (int i = 0; i < n; i++) {
					mng.getVehicul(i)->updateveh(t);
				}
			}
		}
	}
	mng.del();
	scuter sc(15);
	pozitie poz(100, 100);
	sc.schpoz(poz);
	masina m(sc);
	cout<<m.getpoz();
	return 0;
}