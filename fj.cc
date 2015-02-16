#include <iostream>
#include <iomanip>
#include <vector>

#include <fastjet/ClusterSequence.hh>

using namespace std;

template<typename T> inline T sq(T x) { return x*x; }

istream& operator>>(istream &in, vector<fastjet::PseudoJet>& p) {
  static double px, py, pz, E;
  in >> px >> py >> pz >> E;
  if (in) p.push_back(fastjet::PseudoJet(px,py,pz,E));
  return in;
}

int main(int argc, char **argv)
{
  double R = 0.6;
  if (argc>1) R = atof(argv[1]);

  fastjet::ClusterSequence::print_banner();
  cout << endl;

  const fastjet::PseudoJet beam(0.,0.,0.,0.);

  vector<fastjet::PseudoJet> particles;
  cout << fixed << setprecision(8);
  while (cin >> particles) {
    fastjet::PseudoJet &p = particles.back();
    cout << setw(12) << p.pt() << ", "
         << setw(12) << p.rap() << ", "
         << setw(12) << sq(p.pt()*p.eta()) << endl;
  }
  cout << endl;

  fastjet::JetDefinition jet_def(fastjet::kt_algorithm, R);

  fastjet::ClusterSequence seq(particles, jet_def, true);

  const vector<fastjet::PseudoJet> jets = seq.inclusive_jets();

  cout << endl << showpoint << fixed << setprecision(5);
  cout << setw(10) << jets[0].px()
       << setw(10) << jets[0].py()
       << setw(10) << jets[0].pz()
       << setw(10) << jets[0].e() << endl;

  return 0;
}
