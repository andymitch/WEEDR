### Final Project
Andrew Mitchell
CSCI 2270-302

# WEEDR![leaf logo](https://github.com/andymitch/WEEDR/blob/master/leaf-logo.png "WEEDR logo")
### Resume Parser

## Overview

WEEDR is a simple resume parser designed to match the most qualified candidates for specific job opportunities based on matching keywords. WEEDR is written in C++ and is command line dependent. The program is able to take multiple job listing in .txt format and compare it to any number of resumes that are also in .txt format. It will the set up a priority queue of the applicants based on number of keywords matched from the job listing and place (time of apllication).
  
## Data Structures

  * Priority Queue/Max Heap - ranks potential candidates by keywords and time of submission.
  * Hash Table - allows for a quick reference to keywords as well as words to ignore.
  * Struct - retains each candidate’s identifying and contact information as well as their matched keywords.
  * STD::Vector - dynamically stores a candidate’s matched keywords.

## How to Use

1. Be sure that you have at least C++11 installed.
  
2. Download the source files from Github.
  
   * From terminal: `$ git clone https://github.com/andymitch/WEEDR.git`
  
   * From link: [WEEDR](https://github.com/andymitch/WEEDR/archive/master.zip).
  
3. Insert any job listings into the positions folder, referencing them in the positions/list.txt file.
  
4. Insert any resumes into the applicants folder, referencing them in the applicants/list.txt file.
  
5. Compile: `g++ -std=c++11 WEEDR.cpp WEEDR_Driver.cpp -o weedr`.
  
6. Run: `./weedr`.

## Future Development

  * Add software skills factor
  * Develop UI
  * Allow for other file formats to be read than just .txt files.
  * Continue improving information parcing accuracy.

## Open Issues/Bugs
  * duplicate keywords
