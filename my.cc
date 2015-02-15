#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <cmath>

using namespace std;

class particle {
private:
  int _id;
  double _px, _py, _pz, _E, _pt, _P, _rap, _eta, _M;
  pair<const particle*,const particle*> _mothers;

  static int num;

public:
  particle(double px, double py, double pz, double E)
  : _id(num++), _px(px), _py(py), _pz(pz), _E(E),
    _pt(sqrt(_px*_px+_py*_py)), _P(sqrt(_pt*_pt+_pz*_pz)),
    _rap(0.5*ln((_E+_pz)/(_E-_pz)), _eta(0.5*ln((_P+_pz)/(_P-_pz)),
    _M(sqrt(_E*_E-_P*_P)), _mothers(NULL,NULL)
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

};
int particle::num = 0;

int main()
{
  list<particle> particles;
  do {
    static double px, py, pz, E;
    cin >> px >> py >> pz >> E;
    particles.push_back(particle(px,py,pz,E));
  } while (cin);

  

  return 0;
}
