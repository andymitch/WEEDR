#include "WEEDR.hpp"
#include <iostream>
#include <vector>
using namespace std;


int main(int argc, char** argv){
  PriorityQueue applicants(argc-2); //Heap of applicants
  Hash ignore; //hash table of ignore words
  Hash listing(argv[1], ignore); //hash table of words from a job listing

  for(int i = 2; i < argc; i++){
    Application applicant = new Application(argv[i], i-1);
    applicants.enqueue(applicant); //queue up applicants
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
