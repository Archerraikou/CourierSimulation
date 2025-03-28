#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
struct strategie {
	int t, id, s;
};
struct pozitie {
	int x, y;
};
struct pachet {
	short id;
	pozitie pozr;
	pozitie pozl;
	float masa;
	float volp;
	bool rece;
	int liv;
	int timpl, timpp;
};
struct vehicul {
	int tip;
	int id;
	float mas, vol;
	pozitie pozv, pozd;
	bool pedrum;
	pachet p[100];
	int t[100];
	int pac = 0;
	int s;
	bool lr, prir;
};
double calcdist(int x1, int y1, int x2, int y2) { //calculeaza distanta intre doua puncte
	return sqrt(1LL * (x1 - x2) * (x1 - x2) + 1LL * (y1 - y2) * (y1 - y2));
}
double calctimp(int t, int x1, int y1, int x2, int y2) { //calculeaza timpul necesar vehiculului de tipul t sa ajunga de la pozitia A la pozitia B
	if (t == 1) return floor(calcdist(x1, y1, x2, y2) * 60) / 20000;
	if (t == 2) return floor(calcdist(x1, y1, x2, y2) * 60) / 10000;
	else return floor(calcdist(x1, y1, x2, y2) * 60) / 5000;
}
void init(vehicul& v, int t, double m, double volum, int x, int y, int s) { //initializeaza un vector de vehicule{
	v.tip = t;
	v.mas = m;
	v.vol = volum;
	v.pozv.x = x;
	v.pozv.y = y;
	v.pedrum = 0;
	v.pozd.x = -1;
	v.pozd.y = -1;
	v.s = s;
}
void primire_pachet(pachet& p, int x1, int y1, int x2, int y2, double m, double v, bool r, int l, int t1, int t2) { //initializeaza un pachet
	p.pozr.x = x1;
	p.pozr.y = y1;
	p.pozl.x = x2;
	p.pozl.y = y2;
	p.masa = m;
	p.volp = v;
	p.rece = r;
	p.liv = l;
	p.timpp = t1;
	p.timpl = t2;
}
void adaugarepachet(vehicul& v, pachet p, int poz) {
	if (poz == v.pac) {
		v.p[poz] = p;
		if (poz == 0) {
			v.t[poz] = calctimp(v.tip, v.pozv.x, v.pozv.y, p.pozr.x, p.pozr.y) + calctimp(v.tip, p.pozr.x, p.pozr.y, p.pozl.x, p.pozl.y);
		}
		else {
			v.t[poz] = calctimp(v.tip, v.p[poz - 1].pozl.x, v.p[poz - 1].pozl.y, p.pozr.x, p.pozr.y) + calctimp(v.tip, p.pozr.x, p.pozr.y, p.pozl.x, p.pozl.y);
		}
		v.pedrum = 1;
		v.mas -= p.masa;
		v.vol -= p.volp;
		v.pac++;
	}
	else {
		for (int i = v.pac + 1; i > poz; i--) {
			v.p[i] = v.p[i - 1];
			v.t[i] = v.t[i - 1];
		}
		v.p[poz] = p;
		if (poz == 0) {
			v.t[poz] = calctimp(v.tip, v.pozv.x, v.pozv.y, p.pozr.x, p.pozr.y) + calctimp(v.tip, p.pozr.x, p.pozr.y, p.pozl.x, p.pozl.y);
		}
		else {
			v.t[poz] = calctimp(v.tip, v.p[poz - 1].pozl.x, v.p[poz - 1].pozl.y, p.pozr.x, p.pozr.y) + calctimp(v.tip, p.pozr.x, p.pozr.y, p.pozl.x, p.pozl.y);
		}
		v.pedrum = 1;
		v.mas -= p.masa;
		v.vol -= p.volp;
		v.pac++;
		for (int i = poz + 1; i < v.pac; i++) {
			v.t[i] = calctimp(v.tip, v.p[i - 1].pozl.x, v.p[i - 1].pozl.y, v.p[i].pozr.x, v.p[i].pozr.y) + calctimp(v.tip, v.p[i].pozr.x, v.p[i].pozr.y, v.p[i].pozl.x, v.p[i].pozl.y);
		}
	}
}
void updateveh(vehicul v[], int n, int t) {
	for (int i = 0; i < n; i++) {
		if (v[i].pedrum == 0) continue;
		if (v[i].t[0] == 1) {
			cout << '[';
			if (t / 60 % 24 < 10) cout << '0' << t / 60 % 24 << ':';
			else cout << t / 60 % 24 << ':';
			if (t % 60 < 10) cout << '0' << t % 60;
			else cout << t % 60;
			cout << "] ";
			if (v[i].lr == 0) {
				cout << "Vehicului cu id-ul " << v[i].id << " a ridicat si livrat pachetul cu id-ul " << v[i].p[0].id << '\n';
			}
			else {
				cout << "Vehicului cu id-ul " << v[i].id << " a livrat pachetul cu id-ul " << v[i].p[0].id << '\n';
			}
			v[i].lr = 0;
			v[i].pozv = v[i].p[0].pozl;
			v[i].mas += v[i].p[0].masa;
			v[i].vol += v[i].p[0].volp;
			if (v[i].pac > 1) {
				for (int j = 0; j < v[i].pac - 1; j++) {
					v[i].p[j] = v[i].p[j + 1];
					v[i].t[j] = v[i].t[j + 1];
				}
				v[i].pozd = v[i].p[0].pozr;
			}
			else v[i].pedrum = 0;
			v[i].pac--;
		}
		else {
			int mpm, dist = calcdist(v[i].pozv.x, v[i].pozv.y, v[i].pozd.x, v[i].pozd.y);
			if (v[i].tip == 1) mpm = 333;
			else if (v[i].tip == 2) mpm = 167;
			else mpm = 83;
			if (mpm >= dist) {
				cout << '[';
				if (t / 60 % 24 < 10) cout << '0' << t / 60 % 24 << ':';
				else cout << t / 60 % 24 << ':';
				if (t % 60 < 10) cout << '0' << t % 60;
				else cout << t % 60;
				cout << "] ";
				v[i].pozv = v[i].pozd;
				v[i].pozd = v[i].p[0].pozl;
				v[i].lr = 1;
				v[i].pozv.x += (v[i].pozd.x - v[i].pozv.x) / v[i].t[0] * (mpm - dist) / mpm;
				v[i].pozv.y += (v[i].pozd.y - v[i].pozv.y) / v[i].t[0] * (mpm - dist) / mpm;
				cout << "Vehicului cu id-ul " << v[i].id << " a ridicat pachetul cu id-ul " << v[i].p[0].id;
				cout << " si va ajunge la destinatie la ora ";
				if ((v[i].t[0] - 1 + t) / 60 % 24 < 10) cout << '0' << (v[i].t[0] + t - 1) / 60 % 24 << ':';
				else cout << (v[i].t[0] + t) / 60 % 24 << ':';
				if ((t + v[i].t[0] - 1) % 60 < 10) cout << '0' << (t + v[i].t[0] - 1) % 60;
				else cout << (t + v[i].t[0] - 1) % 60;
				if ((v[i].t[0] - 1 + t) / 60 / 24 == 1) cout << " in ziua urmatoare";
				else if ((v[i].t[0] - 1 + t) / 60 / 24 > 1) {
					cout << " peste " << (v[i].t[0] - 1 + t) / 60 / 24 << " zile";
				}
				cout << '\n';
			}
			else {
				if (v[i].lr) {
					v[i].pozv.x += (v[i].pozd.x - v[i].pozv.x) / v[i].t[0];
					v[i].pozv.y += (v[i].pozd.y - v[i].pozv.y) / v[i].t[0];
				}
				else {
					int timp = calctimp(v[i].tip, v[i].pozd.x, v[i].pozd.y, v[i].pozv.x, v[i].pozv.y);
					if (v[i].pozd.x - v[i].pozv.x >= 0) v[i].pozv.x += (v[i].pozd.x - v[i].pozv.x) / timp;
					else v[i].pozv.x -= (abs(v[i].pozd.x - v[i].pozv.x)) / timp;
					if (v[i].pozd.y - v[i].pozv.y >= 0) v[i].pozv.y += (v[i].pozd.y - v[i].pozv.y) / timp;
					else v[i].pozv.y -= (abs(v[i].pozd.y - v[i].pozv.y)) / timp;
				}
			}
			v[i].t[0]--;
		}
	}
}
int gasirepoz(vehicul v, pachet p) {
	if (v.s == 3) return v.pac;
	if (v.s == 2) {
		for (int i = 1; i < v.pac; i++) {
			if (calcdist(v.p[0].pozl.x, v.p[0].pozl.y, p.pozr.x, p.pozr.y) <= calcdist(v.p[0].pozl.x, v.p[0].pozl.y, v.p[i].pozr.x, v.p[i].pozr.y)) return i;
		}
		return v.pac;
	}
	if (v.s == 1) {
		if (p.liv == 1) {
			for (int i = 1; i < v.pac; i++) {
				if (v.p[i].timpl >= p.timpl) return i;
			}
			return v.pac;
		}
		else {
			for (int i = 1; i < v.pac; i++) {
				if (v.p[i].liv >= p.liv) return i;
			}
			return v.pac;
		}
	}
}
bool verif(vehicul v, int timp) {
	int t = timp + v.t[0];
	for (int i = 1; i < v.pac; i++) {
		t += v.t[i];
		if (v.p[i].liv == 1 and t > v.p[i].timpl) {
			return 0;
		}
	}
	return 1;
}
bool test(vehicul v, pachet p, int poz, int t) {
	adaugarepachet(v, p, poz);
	return verif(v, t);
}
int schstrateg(vehicul& v, int s, int t) {
	if (v.pedrum == 0 or v.pac <= 2) {
		v.s = s;
		return 1;
	}
	int poz;
	vehicul vtest;
	vtest.tip = v.tip;
	vtest.s = s;
	vtest.pozv = v.pozv;
	adaugarepachet(vtest, v.p[0], 0);
	for (int i = 1; i < v.pac; i++) {
		poz = gasirepoz(vtest, v.p[i]);
		if (test(vtest, v.p[i], poz, t + v.t[0])) {
			adaugarepachet(vtest, v.p[i], poz);
		}
		else return 0;
	}
	for (int i = 1; i < vtest.pac; i++) {
		v.p[i] = vtest.p[i];
		v.t[i] = vtest.t[i];
	}
	v.s = s;
	return 1;
}
int gasiretimp(vehicul v, pachet p) {
	int t = 0;
	if (v.pedrum == 0) return calctimp(v.tip, v.pozv.x, v.pozv.y, p.pozr.x, p.pozr.y) + calctimp(v.tip, p.pozr.x, p.pozr.y, p.pozl.x, p.pozl.y);
	else {
		int poz = gasirepoz(v, p);
		adaugarepachet(v, p, poz);
		for (int i = 0; i <= poz; i++) {
			t += v.t[i];
		}
		return t;
	}
}
void strategief(vehicul v[], int n, int s, pachet p) {
	vehicul aux;
	if (s == 1) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (calcdist(p.pozr.x, p.pozr.y, v[i].pozv.x, v[i].pozv.y) > calcdist(p.pozr.x, p.pozr.y, v[j].pozv.x, v[j].pozv.y)) {
					aux = v[i];
					v[i] = v[j];
					v[j] = aux;
				}
			}
		}
	}
	if (s == 2) {
		int t[100];
		for (int i = 0; i < n; i++) t[i] = gasiretimp(v[i], p);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (t[i] > t[j]) {
					aux = v[i];
					v[i] = v[j];
					v[j] = aux;
				}
			}
		}
	}
	if (s == 3) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (v[i].mas < v[j].mas) {
					aux = v[i];
					v[i] = v[j];
					v[j] = aux;
				}
			}
		}
	}
	return;
}
bool veriftimp(vehicul& v, pachet p, int t) {
	if (v.pedrum == 0) {
		if (p.liv == 3) {
			adaugarepachet(v, p, 0);
			v.pozd = p.pozr;
			v.lr = 0;
			return 1;
		}
		if (p.liv == 2) {
			if (p.timpl - p.timpp >= calctimp(v.tip, p.pozr.x, p.pozr.y, p.pozl.x, p.pozl.y)) {
				adaugarepachet(v, p, 0);
				v.pozd = p.pozr;
				v.lr = 0;
				return 1;
			}
			else return 0;
		}
		if (p.liv == 1) {
			if (p.timpl >= calctimp(v.tip, v.pozv.x, v.pozv.y, p.pozr.x, p.pozr.y) + calctimp(v.tip, p.pozr.x, p.pozr.y, p.pozl.x, p.pozl.y) + t) {
				adaugarepachet(v, p, 0);
				v.pozd = p.pozr;
				v.lr = 0;
				return 1;
			}
			else return 0;
		}
	}
	else {
		if (p.liv == 2) {
			if (p.timpl - p.timpp < calctimp(v.tip, p.pozr.x, p.pozr.y, p.pozl.x, p.pozl.y))
				return 0;
		}
		int poz = gasirepoz(v, p);
		if (test(v, p, poz, t)) {
			adaugarepachet(v, p, poz);
			return 1;
		}
		else return 0;
	}

}
int asignpac(pachet p, vehicul v[], int n, int s, int t) {
	strategief(v, n, s, p);
	for (int i = 0; i < n; i++) {
		if (v[i].mas >= p.masa and v[i].vol >= p.volp and (p.rece == 0 || (v[i].tip == 4 and p.rece == 1))) {
			if (veriftimp(v[i], p, t)) {
				return v[i].id;
			}
		}
	}
	return 0;
}
int main(int argc, char* argv[]) {
	istream* in;
	if (argc <= 1) {
		in = &cin;
	}
	else {
		in = new fstream(argv[1]);
	}
	int t = 0, n, m, tip, strat, x, y, t1, t2, l, r, x2, y2, s = 1, o;
	double masa, volum;
	vehicul v[100];
	pachet p[10000], aux;
	strategie str[1000], auxstr;
	cout << "Cate vehicule vor lucra astazi: ";
	*in >> n;
	for (int i = 0; i < n; i++) {
		cout << "Introduceti tipul vehiculului " << i + 1 << ": ";
		*in >> tip;
		cout << "Introduceti masa si volum vehiculului " << i + 1 << ": ";
		*in >> masa >> volum;
		cout << "Introduceti pozitia vehiculului " << i + 1 << ": ";
		*in >> x >> y;
		cout << "Introduceti strategia soferului: ";
		*in >> strat;
		init(v[i], tip, masa, volum, x, y, strat);
		v[i].id = i + 1;
	}
	cout << "\nCate pachete vor fi livrate astazi: ";
	*in >> m;
	for (int i = 0; i < m; i++) {
		cout << "Introduceti locatia de unde va fi ridicat pachetul: ";
		*in >> x >> y;
		cout << "Introduceti locatia unde trebuie livrat pachetul: ";
		*in >> x2 >> y2;
		cout << "Introduceti masa,volumul si daca trebuie tinut la rece: ";
		*in >> masa >> volum >> r;
		cout << "Introduceti tipul de terment pentru livrare (1=pana la un moment al zilei, 2=termen limita de la ridicarea pachetului, 3=fara termen limita): ";
		*in >> l;
		if (l == 3) {
			cout << "Introduceti timpul de la care va putea fi ridicat pachetul: ";
			*in >> t1;
			t2 = 0;
		}
		if (l == 2) {
			cout << "Introduceti timpul de la care va putea fi ridicat pachetul si termenul limita de la ridicarea pachetului: ";
			*in >> t1 >> t2;
		}
		if (l == 1) {
			cout << "Introduceti timpul de la care va putea fi ridicat pachetul si termenul limita: ";
			*in >> t1 >> t2;
		}
		primire_pachet(p[i], x, y, x2, y2, masa, volum, r, l, t1, t2);
		p[i].id = i + 1;
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			if (p[j].timpp > p[i].timpp) {
				aux = p[j];
				p[j] = p[i];
				p[i] = aux;
			}
		}
	}
	cout << "\nIntroduceti strategia firmei de ziua de astazi: ";
	*in >> s;
	cout << "Introduceti numarul de schimbari de strategie pe ziua de astazi: ";
	*in >> o;
	for (int i = 0; i < o; i++) {
		cout << "Introduceti momentul schimbari de strategie, vehiculul al carui sofer sa schimbe strategie sau 0 pentru schimbarea strategiei firmei si strategia in care sa schimbe: ";
		*in >> str[i].t >> str[i].id >> str[i].s;
	}
	for (int i = 0; i < o; i++) {
		for (int j = 0; j < o; j++) {
			if (str[j].t > str[i].t) {
				auxstr = str[j];
				str[j] = str[i];
				str[i] = auxstr;
			}
		}
	}
	cout << "\n\n";
	int index1 = 0, index2 = 0;
	for (t = 0; t <= 1440; t++) {
		while (p[index1].timpp == t) {
			x = asignpac(p[index1], v, n, s, t);
			cout << '[';
			if (t / 60 % 24 < 10) cout << '0' << t / 60 % 24 << ':';
			else cout << t / 60 % 24 << ':';
			if (t % 60 < 10) cout << '0' << t % 60;
			else cout << t % 60;
			cout << "] ";
			if (x == 0) {
				cout << "Pachetul " << p[index1].id << " nu poate fi livrat la timp sau nu este loc pentru el in niciun vehicul, ne pare rau.\n";
			}
			else cout << "Pachetul " << p[index1].id << " va fi preluat de vehiculul " << x << '\n';
			index1++;
		}
		while (str[index2].t == t) {
			if (str[index2].id == 0) {
				cout << '[';
				if (t / 60 % 24 < 10) cout << '0' << t / 60 % 24 << ':';
				else cout << t / 60 % 24 << ':';
				if (t % 60 < 10) cout << '0' << t % 60;
				else cout << t % 60;
				cout << "] ";
				s = str[index2].s;
				cout << "Strategia firmei a fost schimbata in strategia " << s << '\n';
			}
			else {
				for (int j = 0; j < n; j++) {
					if (v[j].id == str[index2].id) {
						cout << '[';
						if (t / 60 % 24 < 10) cout << '0' << t / 60 % 24 << ':';
						else cout << t / 60 % 24 << ':';
						if (t % 60 < 10) cout << '0' << t % 60;
						else cout << t % 60;
						cout << "] ";
						if (schstrateg(v[j], str[index2].s, t) == 1) {
							cout << "Soferul vehiculului " << v[j].id << " a schimbat strategia in strategia " << str[index2].s << '\n';
						}
						else cout << "Soferul vehiculului " << v[j].id << " nu a putut schimba strategie deoarece un pachet nu ar mai fi fost livrat la timp \n";
						break;
					}
				}
			}
			index2++;
		}
		updateveh(v, n, t);
	}
	return 0;
}