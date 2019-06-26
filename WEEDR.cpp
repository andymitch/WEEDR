#include "WEEDR.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <array>
#include <iomanip>
#include <string>
using namespace std;

//HELPER FUNCTION
/*****************************************************************************/
string format(string word){
  for(int i = 0; i < word.length(); i++) if(word[i] == ' ') word.erase(word.begin()+i);
  word.append(".txt");
  return word;
}

bool nonEss(char c){
  return(c==','||c=='.'||c==';'||c==':'||c=='\''||c=='\"'||c=='!'||c=='?'||c=='('||c==')'||c=='-');
}

string clean(string word){
  for(int i = 0; i < word.length(); i++){
    if(nonEss(word[i])) word.erase(word.begin()+i);
    if(isalpha(word[i])) word[i] = tolower(word[i]);
  }
  return word;
}

bool has(string word, char c){
  for(int i = 0; i < word.length(); i++) if(word[i] == c) return true;
  return false;
}

int digitsIn(string word){
  int count = 0;
  for(int i = 0; i < word.length(); i++) if(isdigit(word[i])) count++;
  return count;
}

vector<string> split(string str, char a, char b){
  vector<string> wordsA, wordsB;
  string word;
  istringstream sa(str);
  while (getline(sa, word, a)) wordsA.push_back(word);
  for(auto w : wordsA){
    istringstream sb(w);
    while(getline(sb, word, b)) wordsB.push_back(word);
  }
  return wordsB;
}


//HASH CLASS
/*****************************************************************************/
Hash::Hash(int sz){
  size = sz;
  table = new LL*[size];
}

Hash::~Hash(){
  for(int i = 0; i < size; i++){
    LL* next;
    LL* current = table[i];
    while (current != nullptr){
      next = current->next;
      free(current);
      current = next;
    }
    table[i] = nullptr;
  }
  delete [] table;
}

unsigned int Hash::getHash(string word){
  unsigned int hash = 5381;
  for(int i = 0; i < word.length(); i++) hash = (hash*33)+word[i];
  hash = hash%size;
  if(hash < 0) hash += size;
  return hash;
}

void Hash::add(string word){
  if(!exist(word)){
    int index = getHash(word);
    LL* prev = nullptr;
    LL* current = table[index];
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

bool Hash::exist(string word){
  LL* exist = search(word);
  return(exist != nullptr);
}

void Hash::get(){
  string word;
  ifstream infile;
  infile.open("ignoreWords.txt");
  while(infile){
    infile >> word;
    clean(word);
    this->add(word);
  }
}

void Hash::get(string file, Hash* ignore){
  string word;
  ifstream infile;
  infile.open("positions/"+file);
  while(infile){
    infile >> word;
    word = clean(word);
    if(!ignore->exist(word)) this->add(word);
  }
  infile.close();
}

//APPLICATION STRUCT
/*****************************************************************************/
void Application::push(string word, vector<Hash*>& pos){
  for(int i = 0; i < pos.size(); i++) if(pos[i]->exist(word)) keywords[i].push_back(word);
}

void Application::get(int p, string file, Hash* ignore, vector<Hash*>& pos){
  place = p;
  string word;
  ifstream infile;
  infile.open("applicants/"+file);
  bool gotEmail = false, gotPhone = false;

  //skip to name
  infile >> word;
  word = clean(word);
  while(ignore->exist(word)){
    infile >> word;
    word = clean(word);
  }

  //get name
  word[0] = toupper(word[0]);
  first = word; //get first name
  infile >> word;
  word = clean(word);
  word[0] = toupper(word[0]);
  last = word; //get last name

  //get email and phone number
  while(!gotEmail || !gotPhone){
    infile >> word;
    string num;
    if(has(word, '@')){ //get email address
      email = word;
      gotEmail = true;
    }
    else if(digitsIn(word) == 10){ //get phone number
      num = word;
      gotPhone = true;
    }
    else if(digitsIn(word) == 3){
      string nextWord;
      infile >> nextWord;
      if(digitsIn(nextWord) == 7){ //get phone number (if number format has a space)
        num = word+nextWord;
        num = clean(num);
        gotPhone = true;
      }
    }
    if(num.length() == 10) for(int i = 0; i < 10; i++) phone[i] = num[i];
  }

  //set size of keywords to match the number of positions to index easier
  keywords.resize(pos.size());

  //get keywords
  while(infile){
    infile >> word;
    word = clean(word);
    if(has(word, '/')||has(word, '-')){
      vector<string> words = split(word, '/', '-');
      for(auto w : words) push(word, pos);
    }else push(word, pos);
  }
  infile.close();
}

void Application::peek(int i){
  cout << left << setw(8) << first << " " << left << setw(12) << last;
  cout << left << setw(22) << email;
  cout << right << " (" << phone[0] << phone[1] << phone[2] << ")" << phone[3] << phone[4] << phone[5] << "-" << phone[6] << phone[7] << phone[8] << phone[9] << " ";
  cout << right << setw(10) << keywords[i].size();
  cout << right << setw(7) << place << endl;
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

void PriorityQueue::enqueue(Application applicant, int pos){
  queue[currentSize] = applicant;
  repairUp(currentSize, pos);
  currentSize++;
}

void PriorityQueue::dequeue(int pos){
  queue[0] = queue[currentSize-1];
  currentSize--;
  repairDown(0, pos);
}

void PriorityQueue::repairUp(int index, int pos){
  for(int i = index; i > 0; i--){
    if(priority(queue[i], queue[i/2], pos)){
      swap(&queue[i], &queue[i-1]);
    }
  }
}

void PriorityQueue::repairDown(int index, int pos){
  int left = (index*2)+1, right = (index*2)+2, smallest = index;
  if(left < currentSize) if(priority(queue[left], queue[smallest], pos)) smallest = left;
  if(right < currentSize) if(priority(queue[right], queue[smallest], pos)) smallest = right;
  if(smallest != index){
    swap(&queue[smallest], &queue[index]);
    repairDown(smallest, pos);
  }
}

bool PriorityQueue::priority(Application a, Application b, int pos){
  if(a.keywords[pos].size() > b.keywords[pos].size()) return true;
  if(a.keywords[pos].size() < b.keywords[pos].size()) return false;
  if(a.place < b.place) return true;
  return false;
}

void PriorityQueue::swap(Application* a, Application* b){
  Application temp = *a;
  *a = *b;
  *b = temp;
}

void PriorityQueue::peek(int i){
  queue[0].peek(i);
}

void PriorityQueue::peekKeys(){
  while(!isEmpty()){
    cout << endl << queue[0].first << " " << queue[0].last << ": \n";
    for(auto ks : queue[0].keywords){cout << "\n\n"; for(auto k : ks) cout << k << " ";}
    cout << endl;
    dequeue(0);
  }
}

Application PriorityQueue::top(){
  return queue[0];
}

bool PriorityQueue::isEmpty(){
  return(currentSize == 0);
}
