#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
using namespace std;


//HELPER FUNCTIONS
int digitsIn(string);
bool has(string, char);
vector<string> splitString(const string&, char);
bool nonEssential(char);
void clean(string&);
void clean(vector<vector<string>>&);

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
  int place; //when they applied
  vector<vector<string>> keywords;
  void setInfo(string, Hash&);
  void setKeywords(string, Hash&, vector<Hash>&);
};

class PriorityQueue{
  Application* queue;
  int currentSize, maxSize;
  void repairUp(int, int);
  void repairDown(int, int);
  bool priority(Application, Application, int);
  void swap(Application*, Application*);
public:
  PriorityQueue(int);
  ~PriorityQueue();
  void enqueue(Application, int);
  void dequeue(int);
  void peekAll(vector<string>, vector<Application>&);
  bool isEmpty();
};
