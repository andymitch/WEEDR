#include "WEEDR.hpp"
#include <iostream>
#include <vector>
using namespace std;


int main(int argc, char** argv){
  PriorityQueue applicants(argc-2); //Heap of applicants
  cout << "\nTEST 1" << endl;
  Hash ignore; //hash table of ignore words
  cout << "\nTEST 2" << endl;
  Hash listing(argv[1], ignore); //hash table of words from a job listing
  cout << "\nTEST 3" << endl;
  for(int i = 2; i < argc; i++){
    Application applicant;
    cout << "\nTEST 4" << endl;
    applicant.setInfo(argv[i], ignore);
    cout << applicant.first << " - " << applicant.last << endl;
    cout << "Phone: " << applicant.phone << endl;
    cout << "Email: " << applicant.email << endl;
    cout << "\nTEST 5" << endl;
    applicant.setKeywords(argv[i], ignore, listing);
    cout << "\nTEST 6" << endl;
    applicant.position = i-1;
    cout << "\nTEST 7" << endl;
    applicants.enqueue(applicant); //queue up applicants
    cout << "\nTEST 8" << endl;
  }

  cout << "Press ENTER to see the next Applicant." << endl;
  string enter;
  cin >> enter;
  while(!applicants.isEmpty()){
    applicants.peek();
    cin >> enter;
    applicants.dequeue();
  }
  return 0;
}
