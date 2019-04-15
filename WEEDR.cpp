#include "WEEDR.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <array>
#include <iomanip>
using namespace std;


//HELPER FUNCTIONS
/*****************************************************************************/
int digitsIn(string word){
  int count = 0;
  for(int i = 0; i < word.length(); i++) if(isdigit(word[i])) count++;
  return count;
}
bool has(string word, char c){
  for(int i = 0; i < word.length(); i++) if(word[i] == c) return true;
  return false;
}
vector<string> splitString(const string& str, char c){
  vector<string> words;
  string word;
  istringstream ss(str);
  while (getline(ss, word, c)) words.push_back(word);
  return words;
}
bool nonEss(char c){
  return(c==','||c=='.'||c==';'||c==':'||c=='\''||c=='\"'||c=='!'||c=='('||c==')'||c=='-');
}
void clean(string& word){
  for(int i = 0; i < word.length(); i++){
    if(nonEss(word[i])) word.erase(word.begin()+i);
    if(isalpha(word[i])) word[i] = tolower(word[i]);
  }
}
void clean(vector<string>& v){
  auto end = v.end();
  for (auto it = v.begin(); it != end; ++it) end = remove(it+1, end, *it);
  v.erase(end, v.end());
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
  if(!exists(word)){
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
bool Hash::exists(string word){
  LL* exists = search(word);
  return(exists != nullptr);
}

//APPLICATION STRUCT
/*****************************************************************************/
void Application::setInfo(string file, Hash& ignore){
  ifstream infile;
  infile.open("applicants/"+file); //resume should be in folder 'applicants'
  string word;
  bool gotEmail = false, gotPhone = false;
  if(!infile) cout << file << " does not exist." << endl;
  else{
    infile >> word;
    clean(word);
    while(ignore.exists(word)){
      infile >> word; //bypass any words before name
      clean(word);
    }
    if(!ignore.exists(word)){
      word[0] = toupper(word[0]);
      first = word; //get first name
    }
    infile >> word;
    clean(word);
    word[0] = toupper(word[0]);
    last = word; //get last name
    while(infile){
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
          clean(num);
          gotPhone = true;
        }
      }
      if(num.length() == 10) for(int i = 0; i < 10; i++) phone[i] = num[i];
      if(gotEmail && gotPhone) break; //stop when email and phone are found
    }
  }
  infile.close();
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
  queue[currentSize] = applicant;
  repairUp(currentSize);
  currentSize++;
}
void PriorityQueue::dequeue(){
  queue[0] = queue[currentSize-1];
  currentSize--;
  repairDown(0);
}
void PriorityQueue::peekAll(){
  cout << "***************************************************************************" << endl;
  cout << left << setw(21) <<"Name:" << left << setw(23) << "Email:" << left << setw(15) << "Phone:" << left << setw(10) << "Keywords:" << "Place:" << endl;
  cout << "***************************************************************************" << endl;
  while(!isEmpty()){
    cout << left << setw(8) << queue[0].first << " " << left << setw(12) << queue[0].last;
    cout << left << setw(22) << queue[0].email;
    cout << right << " (" << queue[0].phone[0] << queue[0].phone[1] << queue[0].phone[2] << ")" << queue[0].phone[3] << queue[0].phone[4] << queue[0].phone[5] << "-" << queue[0].phone[6] << queue[0].phone[7] << queue[0].phone[8] << queue[0].phone[9] << " ";
    cout << right << setw(10) << queue[0].keywords.size();
    cout << right << setw(7) << queue[0].position << endl;
    dequeue();
  }
  cout << "***************************************************************************" << endl;
  cout << endl;
}
void PriorityQueue::peek(){
  cout << "*************************************" << endl;
  cout << "Name:  " << queue[0].first << " " << queue[0].last << endl;
  cout << "Email: " << queue[0].email << endl;
  cout << "Phone: (" << queue[0].phone[0] << queue[0].phone[1] << queue[0].phone[2] << ")" << queue[0].phone[3] << queue[0].phone[4] << queue[0].phone[5] << "-" << queue[0].phone[6] << queue[0].phone[7] << queue[0].phone[8] << queue[0].phone[9] << endl;
  cout << "Words: " << queue[0].keywords.size() << endl;
  //cout << "Words: ";
  //for(auto k : queue[0].keywords) cout << k << " ";
  //cout << endl;
  cout << "Place: " << queue[0].position << "\n\n";
}
void PriorityQueue::repairUp(int index){
  for(int i = index; i > 0; i--) if(priority(queue[i], queue[i/2])) swap(&queue[i], &queue[i-1]);
}
void PriorityQueue::repairDown(int index){
  int left = (index*2)+1, right = (index*2)+2, smallest = index;
  if(left < currentSize) if(priority(queue[left], queue[smallest])) smallest = left;
  if(right < currentSize) if(priority(queue[right], queue[smallest])) smallest = right;
  if(smallest != index){
    swap(&queue[smallest], &queue[index]);
    repairDown(smallest);
  }
}
bool PriorityQueue::priority(Application a, Application b){
  if(a.keywords.size() > b.keywords.size()) return true;
  if(a.keywords.size() < b.keywords.size()) return false;
  if(a.position < b.position) return true;
  return false;
}
void PriorityQueue::swap(Application* a, Application* b){
  Application temp = *a;
  *a = *b;
  *b = temp;
}
bool PriorityQueue::isEmpty(){
  return(currentSize == 0);
}
