#include "WEEDR.hpp"
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;


//HELPER FUNCTIONS
/*****************************************************************************/
bool inString(string word, char c){
  for(int i = 0; i < word.length(); i++) if(c == word[i]) return true;
  return false;
}
int digitsIn(string word){
  int count = 0;
  for(int i = 0; i < word.length(); i++) if(word[i].isDigit()) count++;
  return count;
}
bool has(string word, char c){
  for(int i = 0; i < word.length(); i++) if(word[i] == c) return true;
  return false;
}
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
  size = 100;
  table = new LL*[size];
  get("ignoreWords.txt");
}
Hash::Hash(string file, Hash ignore){
  size = 20;
  table = new LL*[size];
  get(file, ignore);
}
Hash::~Hash(){
  for(int i = 0; i < size; i++){
    LL* next, current = table[i];
    while (current != nullptr){
      next = current->next;
      free(current);
      current = next;
    }
    table[i] = nullptr;
  }
  delete [] table;
}
int Hash::getHash(string word){
  int hash = 5381;
  for(int i = 0; i < word.length(); i++) hash = (hash*33)+word[i];
  hash = hash%hashTableSize;
  if(hash < 0) hash += hashTableSize;
  return hash;
}
void Hash::add(string word){
  if(!exists(word)){
    int index = getHash(word);
    LL* prev = nullptr, current = table[index];
    while(current != nullptr){
      prev = current;
      current = current->next;
    }
    current = new LL;
    current->word = word;
    current->next = nullptr;
    if(prev != nullptr) prev->next = current;
    else table[index] = current;
  }
}
LL* Hash::search(string word){
  int index = getHash(word);
  LL* current = table[index];
  while(current != nullptr){
    if(current->word == word) break;
    current = current->next;
  }
  return current;
}
bool Hash::exists(string word){
  LL* exists = search(word);
  return(exists != nullptr);
}
void Hash::get(){
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
  setKeywords(file);
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
void Application::setKeywords(string file){
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
void PriorityQueue::enqueue(Application applicant){
  queue[currentQueueSize] = applicant;
  repairUp(currentQueueSize);
  currentSize++;
}
void PriorityQueue::dequeue(){
  queue[0] = queue[currentSize-1];
  currentSize--;
  repairDown(0);
}
Applicant PriorityQueue::peek(){
  cout << "*************************************" << endl;
  cout << "Name: " << queue[0].first << ", " << queue[0].last << endl;
  cout << "Email: " << queue[0].email << endl;
  cout << "Phone: " << queue[0].phone << endl;
  cout << endl;
}
void PriorityQueue::repairUp(int index){
  for(int i = index; i > 0; i--) if(priority(queue[i], queue[i/2])) swap(queue[i], queue[i-1]);
}
void PriorityQueue::repairDown(int index){
  int left = (index*2)+1, right = (index*2)+2, smallest = index;
  if(left < currentSize) if(priority(queue[left], queue[smallest])) smallest = left;
  if(right < currentSize) if(priority(queue[right], queue[smallest])) smallest = right;
  if(smallest != index){
    swap(queue[smallest], queue[index]);
    repairDownward(smallest);
  }
}
bool PriorityQueue::priority(Application a, Application b){
  if(a.keywords.size() < b.keywords.size()) return true;
  if(a.keywords.size() > b.keywords.size()) return false;
  if(a.position < b.position) return true;
  return false;
}
void PriorityQueue::swap(Application* a, Application* b){
  Application temp = *a;
  *a = *b;
  *b = temp;
}
