#include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  // TODO student
  if(token.empty()){
    return "";
  }
  string toBeReturned = "";
  int start = 0;
  int end = 0;
  int j = 0;
  int numberLetters = 0;
  while(ispunct(token[j])&&j!=token.size()){
    start++;
    j++;
  }
  j = token.size()-1;
  while(ispunct(token[j])&&j!=0){
    end++;
    j--;
  }
  for(int i = start; i < token.size()-end; i++){
    toBeReturned+=tolower(token[i]);
    if(isalpha(token[i])){
      numberLetters++;
    }
  }
  if(numberLetters > 0){
    return toBeReturned;
  } else {
    return "";
  }
}

set<string> gatherTokens(const string& text) {
  // TODO student
  set<string> tokenSet;
  string token;
  istringstream stream(text);
  while(stream >> token){
    string word = cleanToken(token);
    if(!word.empty()){
      tokenSet.insert(cleanToken(token));
    }
  }
  return tokenSet;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  // TODO student
  int webProc = 0;
  ifstream fin(filename);
  string line;
  string website;
  while(getline(fin,line)){
    webProc++;
    if(webProc%2!=0){
      website = line;
    } else {
      set<string> tokenSet = gatherTokens(line);
      for(string token: tokenSet){
        index[token].insert(website);
      }
    }
  }
  return webProc/2;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  // TODO student
  istringstream stream(sentence);
  string token;
  set<string> setResult;
  string prev;
  string unclean;
  bool x = true;
  while(stream >> token){
    unclean = token;
    token=cleanToken(token);
    if(index.count(token)==1){
      if(unclean[0]=='+'){ //intersection
        for(string a:setResult){
          int i = 0;
          for(string b:index.at(token)){
            if(a==b){
              setResult.insert(b);
              x = false;
            }else if(x&&i==index.at(token).size()-1){
              setResult.erase(a);
            }
            i++;
          }
          x = true;
        }

      }else if(unclean[0]=='-'){ //difference
        for(string a:index.at(token)){
          for(string b:setResult){
            if(a==b){
              setResult.erase(a);
            }
          }
        }

      }else{ //union
        for(string a:index.at(token)){
          setResult.insert(a);
        }
      }
    } else {
      if(unclean[0]=='+'){
        for(string a:setResult){
          setResult.erase(a);
        }
      }
    }
  }
  return setResult;
}

void searchEngine(const string& filename) {
  // TODO student
  bool keepGoing = true;
  map<string,set<string>> index;
  set<string> results;
  int webProc = buildIndex(filename,index);
  if(webProc==0){
    cout << "Invalid filename." << endl;
  }
  cout << "Stand by while building index..." << endl;
  cout << "Indexed " << webProc << " pages containing " << index.size() << " unique terms" << endl;
  while(keepGoing){
    string query;
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin,query);
    if(query.empty()){
      cout << "Thank you for searching!";
      break;
    }else{
      results=findQueryMatches(index,query);
      cout << "Found " << results.size() << " matching pages" << endl;
      for(string s:results){
        cout << s << endl;
      }
    }
  }
}
