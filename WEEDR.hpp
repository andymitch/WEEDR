#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
using namespace std;


string format(string); //turns title or name into .txt file format
bool nonEss(char); //if char is a certain type
void clean(string&); //formats strings for simplicity when comparing
bool has(string, char); //string has char in it
int digitsIn(string); //returns how many digits are in string
vector<string> split(string); //split strings into multiple strings based on char

//education levels
enum Ed{N,A,B,M,D}; //None, Associate's, Bachelor's, Master's, Doctorate/PhD
struct Education{
  Ed level;
  string major;
  bool isEd(string);
  bool setLevel(string);
};

//linked list
struct LL{
  string word; //word
  LL* next; //points to next word
};

//hash table
class Hash{
  LL** table; //has table
  int size; //size of hash array
  LL* search(string); //finds and returns word, else returns null
  unsigned int getHash(string); //hash function
public:
  Hash(int); //constructor takes size
  ~Hash(); //destructor
  void add(string); //add string to table
  bool exist(string); //if word exists in hash, calls search()
  void get(); //build ignore hash
  void get(string, Hash*); //build position hash
};

//applicant information and matching resume phrases
struct Application{
  string first, last, email; //name and email address
  char phone [10]; //phone number
  int place; //when they applied
  Education education;
  vector<vector<string>> keywords; //words matched from each position
  Application(){education.level = N;}
  void get(int, string, Hash*, vector<Hash*>&); //get all info and keywords
  bool push(string, vector<Hash*>&); //add word if keyword
  void peek(int i); //print all applicant info
  string getEd();
};

//heap array (priority queue structure)
class PriorityQueue{
  Application* queue; //heap array
  int currentSize, maxSize; //actual number of items in array, capacity of array
  void repairUp(int, int); //sort by priority
  void repairDown(int, int); //sort by priority
  bool priority(Application, Application, int); //based on size of keywords then place
  void swap(Application*, Application*); //swap places in queue
public:
  PriorityQueue(int); //constructor
  ~PriorityQueue(); //destructor
  void enqueue(Application, int); //add to queue and repair
  void dequeue(int); //remove top of queue and repair
  void peek(int); //calls Application::peek on top of queue
  void peekKeys(); //prints name and keywords then dequeues until the heap is empty
  Application top(); //returns top of queue
  bool isEmpty(); //if queue is empty
};
