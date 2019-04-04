#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
using namespace std;


//HELPER FUNCTIONS
int digitsIn(string word); //Application::setInfo
bool has(string word, char c); //Application::setInfo, Application::setResume, Hash::get
vector<string> splitString(const string& str, char c); //Application::setResume

struct LL{
  string word;
  LL* next;
};

class Hash{
  LL** table;
  int size;
  unsigned int getHash(string); //hash function
public:
  Hash(int);
  ~Hash();
  void add(string); //add string to table
  LL* search(string);
  bool exists(string);
};

struct Application{
  string first, last, email;
  char phone [10];
  int position; //when they applied
  vector<string> keywords;
  void setInfo(string, Hash&);
  void setKeywords(string, Hash&, Hash&);
};

class PriorityQueue{
  Application* queue;
  int currentSize, maxSize;
  void repairUp(int);
  void repairDown(int);
  bool priority(Application, Application);
  void swap(Application*, Application*);
public:
  PriorityQueue(int);
  ~PriorityQueue();
  void enqueue(Application);
  void dequeue();
  void peek();
  bool isEmpty();
};
