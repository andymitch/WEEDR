#include "WEEDR.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
using namespace std;


//just for aesthetics
ostream& bold(ostream& os){return os << "\e[1m";} //bold on
ostream& _bold(ostream& os){return os << "\e[0m";} //bold off
ostream& clear(ostream& os){return os << "\033[2J\033[1;1H";} //clear screen

int main(){
  //get list of open positions and potential applicants
  ifstream inpos, inapp;
  inpos.open("positions/list.txt");
  inapp.open("applicants/list.txt");
  string p, a;
  vector<string> _pos, pos, _app, app;
  while(inpos){
    getline(inpos, p);
    _pos.push_back(p);
  }
  inpos.close();
  while(inapp){
    getline(inapp, a);
    _app.push_back(a);
  }
  inapp.close();

  //get options for positions then format as file names
  cout << clear << "***************************************************************************" << endl;
  cout << "WELCOME TO WEEDR! A resume parcing service." << endl;
  cout << "***************************************************************************" << endl;
  cout << "What positions would you like to begin hiring for? ('s' to stop)" << endl;
  for(int i = 0; i < _pos.size()-1; i++) cout << i+1 << ". " << _pos[i] << endl;
  while(true){
    cin >> p;
    if(p == "s") break;
    else if(isdigit(p[0])){
      if(stoi(p) > 0 && stoi(p) < _pos.size()){
        pos.push_back(format(_pos[stoi(p)-1]));
        cout << "\t" << _pos[stoi(p)-1] << endl;;
      }else cout << "Invalid entry." << endl;
    }
    else {cout << "Invalid entry." << endl;}
  }

  //get options for applicants then format as file names
  cout << "\n***************************************************************************" << endl;
  cout << "Who would you like to apply to the positions? ('s' to stop)" << endl;
  for(int i = 0; i < _app.size()-1; i++) cout << i+1 << ". " << _app[i] << endl;
  while(true){
    cin >> a;
    if(a == "s") break;
    else if(isdigit(a[0])){
      if(stoi(a) > 0 && stoi(a) < _app.size()){
        app.push_back(format(_app[stoi(a)-1]));
        cout << "\t" << _app[stoi(a)-1] << endl;
      }else cout << "Invalid entry." << endl;
    }
    else {cout << "Invalid entry." << endl;}
  }

  //build hash for ignore words
  Hash* ignore = new Hash(100);
  ignore->get();

  //build hashes for positions
  vector<Hash*> positions;
  for(int i = 0; i < pos.size(); i++){
    Hash* position = new Hash(50);
    position->get(pos[i], ignore);
    positions.push_back(position);
  }

  //build queue initially
  PriorityQueue* applicants = new PriorityQueue(app.size());
  PriorityQueue* temp = new PriorityQueue(app.size());
  for(int i = 0; i < app.size(); i++){
    Application applicant;
    applicant.get(i+1, app[i], ignore, positions);
    temp->enqueue(applicant, 0);
  }

  //print and rebuild queue
  cout << clear;
  for(int i = 0; i < pos.size(); i++){
    while(!temp->isEmpty()){
      applicants->enqueue(temp->top(), i);
      temp->dequeue(i);
    }
    cout << "\n" << bold << _pos[i] << _bold << "\n\n";
    cout << left << setw(21) << "Name:" << left << setw(23) << "Email:" << left << setw(15) << "Phone:" << left << setw(10) << "Keywords:" << "Place:" << endl;
    cout << "***************************************************************************" << endl;
    while(!applicants->isEmpty()){
      applicants->peek(i);
      temp->enqueue(applicants->top(), i);
      applicants->dequeue(i);
    }
    cout << "***************************************************************************\n\n";
  }


  cout << "Would you like to view the list of keywords? (y/n): ";
  while(true){
    cin >> a;
    if(a == "y"){temp->peekKeys(); break;}
    else if(a == "n") break;
    else cout << "\nInvalid entry. Try 'y' or 'n': ";
  }

  //free memory
  delete temp;
  delete applicants;
  delete ignore;
  for(auto& p : positions) delete p;
  return 0;
}
