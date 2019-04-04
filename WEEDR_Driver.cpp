#include "WEEDR.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
using namespace std;


bool nonEssential(char c){
  return(c==','||c=='.'||c==';'||c==':'||c=='\''||c=='\"'||c=='!'||c=='('||c==')');
}
void clean(string& word){
  for(int i = 0; i < word.length(); i++){
    if(nonEssential(word[i])) word.erase(word.begin()+i);
    if(isalpha(word[i])) word[i] = tolower(word[i]);
  }
}
void clean(vector<string>& v){
  auto end = v.end();
  for (auto it = v.begin(); it != end; ++it) end = remove(it+1, end, *it);
  v.erase(end, v.end());
}

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
void get(string file, Hash& keywords, Hash& ignore){
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
}

vector<string> setKeywords(string file, Hash& ignore, Hash& key){
  ifstream infile;
  string word;
  vector<string> keywords;
  infile.open("applicants/"+file);
  if(!infile) cout << file << "does not exist." << endl;
  else{
    while(infile){
      infile >> word;
      clean(word);
      if(has(word, '/')){
        vector<string> words = splitString(word, '/');
        for(auto a : words) if(!ignore.exists(a) && key.exists(a)) keywords.push_back(a);
      }
      else if(has(word, '-')){
        vector<string> words = splitString(word, '-');
        for(auto a : words) if(!ignore.exists(a) && key.exists(a)) keywords.push_back(a);
      }else if(!ignore.exists(word) && key.exists(word)) keywords.push_back(word);
    }
  }
  infile.close();
  clean(keywords);
  return keywords;
}

int main(int argc, char** argv){
  cout << "\n\n";
  PriorityQueue applicants(argc-2); //Heap of applicants
  Hash ignore(100); //hash table of ignore words
  get(ignore);
  Hash listing(50);
  get(argv[1], listing, ignore);
  for(int i = 2; i < argc; i++){
    Application applicant;
    applicant.setInfo(argv[i], ignore);
    applicant.keywords = setKeywords(argv[i], ignore, listing);
    applicant.position = i-1;
    applicants.enqueue(applicant); //queue up applicants
  }
  cout << "Type something to see the next Applicant." << endl;
  string enter;
  cin >> enter;
  while(!applicants.isEmpty()){
    applicants.peek();
    cin >> enter;
    applicants.dequeue();
  }
  return 0;
}
