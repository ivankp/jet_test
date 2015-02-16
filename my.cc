#include <iostream>
#include <iomanip>
#include <list>
#include <set>
#include <cmath>
#include <cstdlib>

using namespace std;

template<typename T> inline T sq(T x) { return x*x; }

template<class ForwardIt> ForwardIt next(ForwardIt it) { return ++it; }

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
  ~particle() {
    // delete _mothers.first;
    // delete _mothers.second;
  }

  particle operator+(const particle &p) const {
    particle pp(_px+p._px,_py+p._py,_pz+p._pz,_E+p._E);
    pp._mothers = make_pair(new particle(*this),new particle(p));
    return pp;
  }

  int     id() const { return  _id; }
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

  double R(const particle &p) const {
    return sqrt( sq(_eta-p._eta)+sq(_phi-p._phi) );
  }

  double d() const { return _d; }
  double d(const particle &p) const {
    return sq( min(_pt,p._pt) ) * ( sq(_eta-p._eta)+sq(_phi-p._phi) );
  }

  bool operator<(const particle &p) const {
    if (_d==p._d) return ( _id < p._id );
    else return ( _d < p._d );
  }

  ostream& prt(ostream &out, int depth=0) const {
    for (int i=0;i<depth;++i) out << "  |";
    out << fixed
        << setw(3) << _id << ": ("
        << setw(9) << _px << ", "
        << setw(9) << _py << ", "
        << setw(9) << _pz << ", "
        << setw(9) <<  _E << " | "
        << setw(9) << _pt << ", "
        << setw(9) << _d
        << " )" << endl;
    if (_mothers.first)  _mothers.first ->prt(out,depth+1);
    if (_mothers.second) _mothers.second->prt(out,depth+1);
    return out;
  }

};
int particle::num = 0;

ostream& operator<<(ostream &out, const particle& p) {
  const ios_base::fmtflags flags = out.flags();
  const streamsize prec = out.precision(5);
  p.prt(out);
  out.flags(flags);
  out.precision(prec);
  return out;
}

typedef set<particle>::iterator iter_t;

istream& operator>>(istream &in, set<particle>& p) {
  static double px, py, pz, E;
  in >> px >> py >> pz >> E;
  if (in) cout << *p.insert(particle(px,py,pz,E)).first;
  return in;
}

void cluster(set<particle>& particles, list<particle>& jets, double R) {
  iter_t it = particles.begin();
  iter_t end = particles.end(), it1 = end, it2 = end;
  bool merged = false;
  double dist = it->d();
  double merged_R = 0.;
  for (;it!=end;++it) {
    for (iter_t jt=next(it);jt!=end;++jt) {
      double _R = it->R(*jt);
      if ( _R <= R ) { // only if particles are close enough
        double d = it->d(*jt);
        if (d < dist) {
          dist = d;
          merged_R = _R;
          it1 = it;
          it2 = jt;
          if (!merged) merged = true;
        }
      }
    }
  }
  if (merged) {
    particles.insert((*it1)+(*it2));
    particles.erase(it1);
    particles.erase(it2);
    cout << "Merged "
         << setw(2) << it1->id() << " & "
         << setw(2) << it2->id() << "  d = ";
    const ios_base::fmtflags flags = cout.flags();
    cout << fixed << setprecision(8) << dist
         << " R = " << merged_R << endl;
    cout.flags(flags);
  } else {
    jets.push_back(*particles.begin());
    particles.erase(particles.begin());
    cout << jets.back().id() << " is a jet" << endl;
  }
}

int main(int argc, char **argv)
{
  double R = 0.6;
  if (argc>1) R = atof(argv[1]);

  set<particle> particles;
  cout << "Original particles:" << endl;
  cout << setw(10) << "px"
       << setw(11) << "py"
       << setw(11) << "pz"
       << setw(10) << "E"
       << setw(13) << "pt"
       << setw(10) << "d"
       << endl;
  while (cin >> particles);
  cout << endl;

  cout << "Sorted by d:" << endl;
  for (iter_t it=particles.begin(),end=particles.end();it!=end;++it) {
    cout << *it;
  }
  cout << endl;

  cout << "Max R = " << R << endl << endl;

  list<particle> jets;
  while (particles.size()) cluster(particles, jets, R);
  cout << endl;

  for (list<particle>::iterator it=jets.begin(),
       end=jets.end();it!=end;++it) cout << *it;

  return 0;
}
