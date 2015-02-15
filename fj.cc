#include <iostream>
#include <iomanip>
#include <vector>

#include <fastjet/ClusterSequence.hh>

using namespace std;

int main()
{
  vector<fastjet::PseudoJet> particles;
  double px, py, pz, E;
  while ( cin >> px >> py >> pz >> E ) {
    particles.push_back( fastjet::PseudoJet( px,py,pz,E ) );
  }

  fastjet::ClusterSequence::print_banner();
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
