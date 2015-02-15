#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <cmath>

using namespace std;

template<typename T> inline T sq(T x) { return x*x; }

class particle {
private:
  int _id;
  double _px, _py, _pz, _E, _pt, _P, _rap, _eta, _phi, _M, _d;
  pair<const particle*,const particle*> _mothers;

  static int num;

public:
  particle(double px, double py, double pz, double E)
  : _id(num++), _px(px), _py(py), _pz(pz), _E(E),
    _pt(sqrt(_px*_px+_py*_py)), _P(sqrt(_pt*_pt+_pz*_pz)),
    _rap(0.5*log((_E+_pz)/(_E-_pz))), _eta(0.5*log((_P+_pz)/(_P-_pz))),
    _phi(px == 0. && px == 0. ? 0. : atan2(py,px)),
    _M(sqrt(_E*_E-_P*_P)), _d(sq(_pt*_eta)), _mothers(NULL,NULL)
  { }

  particle operator+(const particle &p) const {
    particle pp(_px+p._px,_py+p._py,_pz+p._pz,_E+p._E);
    pp._mothers = make_pair(this,&p);
    return pp;
  }

  double  px() const { return  _px; }
  double  py() const { return  _py; }
  double  pz() const { return  _pz; }
  double   E() const { return   _E; }
  double  pt() const { return  _pt; }
  double   P() const { return   _P; }
  double rap() const { return _rap; }
  double eta() const { return _eta; }
  double   M() const { return   _M; }
  const pair<const particle*,const particle*>& mothers() const {
    return _mothers;
  }

  double d() const { return _d; }
  double d(const particle &p) const {
    return sq( min(_pt,p._pt) ) * ( sq(_eta-p._eta)+sq(_phi-p._phi) );
  }

  ostream& prt(ostream &out, int depth=0) const {
    for (int i=0;i<depth;++i) out << "  ";
    out << fixed
        << setw( 3) << _id << ": ("
        // << setw(12) << _px << ", "
        // << setw(12) << _py << ", "
        // << setw(12) << _pz << ", "
        // << setw(12) <<  _E
        << setw(12) << _pt << ", "
        << setw(12) << _d
        << " )" << endl;
    if (_mothers.first)  _mothers.first ->prt(out,depth+1);
    if (_mothers.second) _mothers.second->prt(out,depth+1);
    return out;
  }

};
int particle::num = 0;

ostream& operator<<(ostream &out, const particle& p) {
  const ios_base::fmtflags flags = out.flags(
    ios::right | ios::showpoint | ios::fixed | ios::scientific
  );
  const streamsize prec = out.precision(8);
  p.prt(out);
  out.flags(flags);
  out.precision(prec);
  return out;
}

istream& operator>>(istream &in, list<particle>& p) {
  static double px, py, pz, E;
  in >> px >> py >> pz >> E;
  p.push_back(particle(px,py,pz,E));
  return in;
}

typedef list<particle>::iterator iter_t;

int main()
{
  list<particle> particles;
  cout << "Original particles:" << endl;
  while (cin >> particles) cout << particles.back();
  cout << endl;

  return 0;
}
