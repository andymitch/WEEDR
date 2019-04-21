#include "WEEDR.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
#include <map>
using namespace std;


void get(Hash& ignore){
  ifstream infile;
  string word;
  infile.open("ignoreWords.txt");
  if(!infile) cout << "ignoreWords.txt is missing from directory." << endl;
  else while(infile){
    infile >> word;
    clean(word);
    ignore.add(word);
  }
}
Hash get(string file, Hash& ignore){
  Hash keywords(20);
  ifstream infile;
  string word;
  infile.open("positions/"+file);
  if(!infile) cout << file << "does not exist." << endl;
  else{
    while(infile){
      infile >> word;
      if(has(word, '/')){
        vector<string> words = splitString(word, '/');
        for(auto a : words){
          clean(word);
          if(!ignore.exists(a)) keywords.add(a);
        }
      }else if(has(word, '-')){
        vector<string> words = splitString(word, '-');
        for(auto a : words){
          clean(word);
          if(!ignore.exists(a)) keywords.add(a);
        }
      }else{
        clean(word);
        if(!ignore.exists(word)) keywords.add(word);
      }
    }
  }
  return keywords;
}

vector<vector<string>> setKeywords(string file, vector<Hash>& pos){
  ifstream infile;
  string word, w;

  cout << "TEST KA" << endl;

  vector<vector<string>> keywords(pos.size());
  infile.open("applicants/"+file);
  if(!infile) cout << file << "does not exist." << endl;
  else{
    while(infile){
      infile >> word;

      cout << word << endl;

      cout << "TEST KB" << endl;

      clean(word);

      cout << "TEST KC" << endl;

      for(int i = 0; i < pos.size(); i++){

        cout << "TEST KD" << endl;

        if(has(word, '/')){

          cout << "TEST KE" << endl;

          vector<string> words = splitString(word, '/');
          for(auto a : words) if(pos[i].exists(a)) keywords[i].push_back(a);

          cout << "TEST KF" << endl;

        }
        else if(has(word, '-')){

          cout << "TEST KG" << endl;

          vector<string> words = splitString(word, '-');

          cout << "TEST KH" << endl;

          for(auto a : words) if(pos[i].exists(a)) keywords[i].push_back(a);

          cout << "TEST KI" << endl;

        }else if(pos[i].exists(word)){
          keywords[i].push_back(word);
          cout << "TEST KJ" << endl;
        }else{
          w = word;
          cout << "Doesn't exist." << endl;
        }
      }
    }
  }
  infile.close();
  clean(keywords);
  return keywords;
}

vector<string> uniquify(vector<string> words){
  cout << "Words size: " << words.size() << endl;
  for(int i = 0; i < words.size()-1; i++){
    cout << "###" << endl;
    for(int j = i+1; j < words.size(); j++){
      cout << words[i] << endl;
      if(words[i] == words[j]){
        words.erase(words.begin()+j);
        cout << "#" << words[i] << endl;
        j--;
      }
    }
  }
  return words;
}

void title(vector<string>& positions, vector<string>& applicants){
  string c = "x"; //initialize stopper
  vector<string> _applicants, _positions;

  cout << "***************************************************************" << endl;
  cout << "WELCOME TO WEEDR! A resume parcing service." << endl;

  cout << "***************************************************************" << endl;
  cout << "What positions would you like to begin hiring for? ('s' to stop)" << endl;
  for(int i = 0; i < positions.size()-1; i++) cout << i+1 << ". " << positions[i] << endl;
  while(true){
    cin >> c;
    if(isdigit(c[0])){
      if(stoi(c) > 1 && stoi(c) < positions.size()){
        _positions.push_back(positions[stoi(c)-1]);
        cout << "\nGot it...\n";
      }
    }else if(c == "s") break;
  }
  //cout << "***" << endl;
  //for (auto a : _positions) cout << a << endl;
  positions = _positions;//uniquify(_positions);
  //cout << "***" << endl;
  //for (auto a : positions) cout << a << endl;
  //cout << "***" << endl;

  c = "x"; //reset stopper

  cout << "\n***************************************************************" << endl;
  cout << "Who would you like to apply to the positions? ('s' to stop)" << endl;
  for(int i = 0; i < applicants.size()-1; i++) cout << i+1 << ". " << applicants[i] << endl;
  while(true){
    cin >> c;
    if(isdigit(c[0])){
      if(stoi(c) > 1 && stoi(c) < applicants.size()-1){
        _applicants.push_back(applicants[stoi(c)]);
        cout << "\nGot 'em...\n";
      }
    }else if(c == "s") break;
  }
  //cout << "***" << endl;
  //for (auto a : _applicants) cout << a << endl;
  applicants = _applicants;//uniquify(_applicants);
  //cout << "***" << endl;
  //for (auto a : applicants) cout << a << endl;
  //cout << "***" << endl;
  cout << endl;
}

vector<string> initiate(vector<string>& pos, vector<string>& app){
  ifstream inapp, inpos;
  inapp.open("applicants/list.txt");
  inpos.open("positions/list.txt");

  //get list
  string applicant, position;
  while(inapp){
    getline(inapp, applicant);
    app.push_back(applicant);
  }
  while(inpos){
    getline(inpos, position);
    pos.push_back(position);
  }

  //customize list
  vector<string> op = pos;
  title(pos, app);

  //format list
  for(auto& p : pos){
    for(int i = 0; i < p.length(); i++) if(p[i] == ' ') p.erase(p.begin()+i);
    p.append(".txt");
  }
  for(auto& p : app){
    for(int i = 0; i < p.length(); i++) if(p[i] == ' ') p.erase(p.begin()+i);
    p.append(".txt");
  }
  return op;
}

int main(){

  //get user input
  vector<string> pos, app;
  vector<string> peekPos = initiate(pos, app);

  //get ignore words
  Hash ignore(100);
  get(ignore);

  cout << "TEST" << endl;

  //get positions
  vector<Hash> positions;
  for(auto& p : pos) positions.push_back(get(p, ignore));

  cout << "TEST" << endl;

  //get applicants' info
  PriorityQueue applicants(app.size());
  for(int i = 0; i < app.size(); i++){
    Application applicant;
    cout << "test A" << endl;
    applicant.setInfo(app[i], ignore);
    cout << "test B" << endl;
    applicant.keywords = setKeywords(app[i], positions);
    cout << "test C" << endl;
    applicant.place = i+1;
    cout << "test D" << endl;
    applicants.enqueue(applicant, 0);

    cout << "test E" << endl;
  }

  //show all
  vector<Application> apps;
  for(int i = 1; i < pos.size(); i++){
    applicants.peekAll(peekPos, apps);
    if(i < pos.size()-1) for(auto a : apps) applicants.enqueue(a, i);
    cout << "TEST" << endl;
  }

  return 0;
}
