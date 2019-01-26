#include <iostream>
#include <string.h>
#include <fstream>
#include <cmath>
#include "spell.h"

using namespace std;

/* insert your function definitions here */

int frequency(string target){

  fstream input_stream;
  input_stream.open("words.dat");

  if (input_stream.fail()){
    cerr << "Error opening \"words.dat\"" << endl;
    return -1;
  }
  
  string word;
  string frequency;
  string definition;
 
  while(input_stream >> word){

    if (isdigit(word[0])){
      frequency = word;
    } else {
      definition = word;
    }

    if (definition == target){
      return stoi(frequency);
    }
  }

  return 0;
}


int edit_distance(string a, string b){

  static int distance_count = 0;
  static int iteration_count = 0;

  if (iteration_count == 0){
    distance_count = 0;
  }
  iteration_count++;
  // check for perfect match
  
  if (a == b){
    iteration_count = 0;
    return distance_count;
  }
  
  // insertion and deletion

  if (a.length() != b.length()){

    string temp_word;
    int length_diff = a.length() - b.length();

    // make b the smaller word if not the case already
    if (length_diff < 0){
      temp_word = a;
      a = b;
      b = temp_word;
      temp_word.clear();
    }

    unsigned i = 0;
    
    while (i < b.length()){
      if (a[i] == b[i]){
	i++;
      } else {
	temp_word.clear();
	// check for transposition here first
	if (i != (b.length() - 1) &&
	    b.length() > 1 &&
	    a[i] == b[i+1] &&
	    a[i+1] == b[i]){

	  char temp_ch;
	  temp_ch = b[i];
	  b[i] = b[i+1];
	  b[i+1] = temp_ch;	    

	  distance_count++;
	  goto recursion;
	}
	
	if (i == 0){
	  temp_word = string(1,a[i]) + b.substr(0,b.length());
	  b = temp_word;
	} else {
	  temp_word = b.substr(0,i) + a[i] + b.substr(i, b.length() -i);
	  b = temp_word;
	}

	distance_count++;
	goto recursion;
      }
    }
    
    distance_count += a.length() - b.length();
    iteration_count = 0;
    return distance_count;

  
    // transposition and replacement
	  
  } else {
  
    unsigned i = 0;
    while (i < a.length()){
      if (a[i] != b[i]){

	if (i != (a.length() - 1) &&
	    a[i] == b[i+1] &&
	    a[i+1] == b[i]){

	  distance_count++;
	  i = i + 2;

	} else {
	  distance_count++;
	  i++;
	}
      } else {
	i++;
      }
    }
    goto end;
  }

 recursion:
  edit_distance(a,b);

 end:
  iteration_count = 0;
  return distance_count;
}

bool spell_correct(const string word, char* fixed){

  fstream input_stream;
  input_stream.open("words.dat");

  if (input_stream.fail()){
    cerr << "Error opening \"words.dat\"" << endl;
    return -1;
  }
  
  string input_word;
  string definition;

  int lowest_distance = 10;
  int lowest_dist_freq = 0;
  int int_freq;
  
  while(input_stream >> input_word){

    if (isdigit(input_word[0])){
      int_freq = stoi(input_word);
    } else {
      definition = input_word;

      if (definition == word){
	strcpy(fixed, word.c_str());
	return false;
      }
      
      int temp_distance = edit_distance(word, definition);

      //cout << definition << " : " << temp_distance << endl;
      
      if (temp_distance < lowest_distance){
	
	lowest_distance = temp_distance;
	lowest_dist_freq = int_freq;
	strcpy(fixed, definition.c_str());
	
      } else if (temp_distance == lowest_distance){
	
	if (int_freq > lowest_dist_freq){
	  
	  lowest_dist_freq = int_freq;
	  lowest_distance = temp_distance;
	  strcpy(fixed, definition.c_str());
	}
      }
    }
  }

  if (lowest_distance < 3){
    return true;
  }
  // default condition if no word can be found with an edit distance of 2 or less
  
  strcpy(fixed, word.c_str());
  return false;
}
