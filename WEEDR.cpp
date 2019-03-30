#include "WEEDR.hpp"
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;


//HELPER FUNCTIONS
/*****************************************************************************/
//Application::setInfo
bool inString(string word, char c){
  for(int i = 0; i < word.length(); i++) if(c == word[i]) return true;
  return false;
}
//Application::setInfo
int digitsIn(string word){
  int count = 0;
  for(int i = 0; i < word.length(); i++) if(word[i].isDigit()) count++;
  return count;
}
//Application::setResume, Hash::get
bool has(string word, char c){
  for(int i = 0; i < word.length(); i++) if(word[i] == c) return true;
  return false;
}
//Application::setResume
vector<string> splitString(const string& str, char c){
   vector<string> words;
   string word;
   istringstream wordStream(str);
   while (getline(wordStream, word, c)) words.push_back(word);
   return words;
}

//HASH CLASS
/*****************************************************************************/
Hash::Hash(){
  //ignore object
  size = 100;
  table = new LL*[size];
  get("ignoreWords.txt");
}
Hash::Hash(string file, Hash ignore){
  //keywords object
  size = 20;
  table = new LL*[size];
  get(file, ignore);
}
Hash::~Hash(){

}
int Hash::getHash(string word){

}
void Hash::add(string word){
  int index = getHash(word);
  //table[index]...
}
bool Hash::exists(string word){
  //word exists in hash table

}
void Hash::get(){
  //for ignore object
  ifstream infile;
  string word;
  infile.open("ignoreWords.txt");
  if(!infile) cout << file << "does not exist." << endl;
  else{
    infile >> word;
    if(has(word, '/')){
      vector<string> words = splitString(word, '/');
      for(auto a : words) add(a);
    }
    else if(has(word, '-')){
      vector<string> words = splitString(word, '-');
      for(auto a : words) add(a);
    }else add(word);
  }
}
void Hash::get(string file, Hash ignore){
  //for keywords object
  ifstream infile;
  string word;
  infile.open("positions/"+file);
  if(!infile) cout << file << "does not exist." << endl;
  else{
    infile >> word;
    if(has(word, '/')){
      vector<string> words = splitString(word, '/');
      for(auto a : words) if(!ignore.exists(a)) add(a);
    }else if(has(word, '-')){
      vector<string> words = splitString(word, '-');
      for(auto a : words) if(!ignore.exists(a)) add(a);
    }else{if(!ignore.exists(word)) add(word);}
  }
}

//APPLICATION CLASS
/*****************************************************************************/
Application::Application(string file, int pos){
  setInfo(file);
  getResume(file);
  position = pos;
}
void Application::setInfo(string file, Hash ignore){
  ifstream infile;
  inFile.open("applicants/"+file); //resume should be in folder 'applicants'
  string word;
  bool gotEmail = false, gotPhone = false;
  if(!inFile) cout << file << " does not exist." << endl;
  else{
    inFile >> word;
    while(ignore.exists(word)) inFile >> word; //bypass any words before name
    if(!ignore.exists(word)) first = word; //get first name
    inFile >> last; //get last name
    while(inFile){
      inFile >> word;
      if(inString(word, @)){ //get email address
        email = word;
        gotEmail = true;
      }
      else if(digitsIn(word) == 10){ //get phone number
        phone = word;
        gotPhone = true;
      }
      else if(digitsIn(word) == 3){
        string nextWord;
        inFile >> nextWord;
        if(digitsIn(nextWord) == 7){ //get phone number (if number format has a space)
          phone = word+nextWord;
          gotPhone = true;
        }
      }
      if(gotEmail && gotPhone) break; //stop when email and phone are found
    }
  }
  inFile.close();
}
void Application::setResume(string file){
  ifstream infile;
  string word;
  infile.open("applicants/"+file);
  if(!infile) cout << file << "does not exist." << endl;
  else{
    infile >> word;
    if(has(word, '/')){
      vector<string> words = splitString(word, '/');
      for(auto a : words) if(isKeyword(a)) keywords.push_back(a);
    }
    else if(has(word, '-')){
      vector<string> words = splitString(word, '-');
      for(auto a : words) if(isKeyword(a)) keywords.push_back(a);
    }else if(!exists(a)) keywords.push_back(word);
  }
}

//PRIORITY QUEUE HEAP CLASS
/*****************************************************************************/
PriorityQueue::PriorityQueue(int size){
  currentSize = 0;
  maxSize = size;
  queue = new Application[maxSize];
}
PriorityQueue::~PriorityQueue(){
  delete[] queue;
}
void PriorityQueue::enqueue(string file){

}
void PriorityQueue::dequeue(){

}
Applicant PriorityQueue::peek(){
  cout << "*************************************" << endl;
  cout << "Name: " << queue[0].first << ", " << queue[0].last << endl;
  cout << "Email: " << queue[0].email << endl;
  cout << "Phone: " << queue[0].phone << endl;
  cout << endl;
}
bool PriorityQueue::isEmpty(){
  return (currentSize <= 0);
}
void PriorityQueue::repairUpward(int index){

}
void PriorityQueue::repairDownward(int index){

}
bool PriorityQueue::priority(Application a, Application b){

}
void PriorityQueue::swap(Application* a, Application* b){

}
