#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
using namespace std;


string format(string); //turns title or name into .txt file format
bool nonEss(char); //if char is a certain type
string clean(string); //formats strings for simplicity when comparing
bool has(string, char); //string has char in it
int digitsIn(string); //returns how many digits are in string
vector<string> split(string); //split strings into multiple strings based on char

struct LL{
  string word;
  LL* next;
};
class Hash{
  LL** table;
  int size; //size of hash array
  LL* search(string); //finds and returns word, else returns null
  unsigned int getHash(string); //hash function
public:
  Hash(int);
  ~Hash();
  void add(string); //add string to table
  bool exist(string); //if word exists in hash, calls search()
  void get(); //build ignore hash
  void get(string, Hash*); //build position hash
};

struct Application{
  string first, last, email;
  char phone [10];
  int place; //when they applied
  vector<vector<string>> keywords; //words matched from each position
  void get(int, string, Hash*, vector<Hash*>&); //get all info and keywords
  void push(string, vector<Hash*>&); //add word if keyword
  void peek(int i); //print all applicant info
};

class PriorityQueue{
  Application* queue;
  int currentSize, maxSize;
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
  Application top(); //returns top of queue
  bool isEmpty(); //if queue is empty
};
