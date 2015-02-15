#include <iostream>
#include <iomanip>
#include <vector>

#include <fastjet/ClusterSequence.hh>

using namespace std;

istream& operator>>(istream &in, vector<fastjet::PseudoJet>& p) {
  static double px, py, pz, E;
  in >> px >> py >> pz >> E;
  p.push_back(fastjet::PseudoJet(px,py,pz,E));
  return in;
}

int main()
{
  fastjet::ClusterSequence::print_banner();
  cout << endl;

  const fastjet::PseudoJet beam(0.,0.,0.,0.);

  vector<fastjet::PseudoJet> particles;
  cout << fixed << setprecision(8);
  while (cin >> particles) {
    fastjet::PseudoJet &p = particles.back();
    cout << setw(12) << p.pt() << ", "
         << setw(12) << p.kt_distance(beam) << endl;
  }
  cout << endl;

  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, 0.6);

  fastjet::ClusterSequence seq(particles, jet_def, true);

  const vector<fastjet::PseudoJet> jets = seq.inclusive_jets();

  cout << endl << showpoint << fixed << setprecision(5);
  cout << setw(10) << jets[0].px()
       << setw(10) << jets[0].py()
       << setw(10) << jets[0].pz()
       << setw(10) << jets[0].e() << endl;

  return 0;
}
