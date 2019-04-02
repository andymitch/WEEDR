#include "WEEDR.hpp"
#include <iostream>
#include <vector>
using namespace std;


int main(int argc, char** argv){
  PriorityQueue applicants(argc-2); //Heap of applicants
  Hash ignore; //hash table of ignore words
  Hash listing(argv[1], ignore); //hash table of words from a job listing

  for(int i = 2; i < argc; i++){

    Application applicant;
    applicant.setInfo(argv[i], ignore);
    applicant.setKeywords(argv[i], ignore, listing);
    applicant.position = i-1;

    //Application* applicant = new Application(argv[i], i-1, ignore, listing);
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
