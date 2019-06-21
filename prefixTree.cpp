
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#define NUM_ALPHABETS 27


struct trie{
  trie *children[NUM_ALPHABETS];
  bool isWordEnd;
};


trie *createTrieNode(){
  trie *temp = new trie;
  for (int i=0; i<NUM_ALPHABETS; i++){
    temp->children[i] = nullptr;
  }
  temp->isWordEnd = false;
  return temp;
}


bool isLastNode(trie *root){
  for (int i=0; i<NUM_ALPHABETS; i++){
    if (root->children[i]!=nullptr){
      return false;
    }
  }
  return true;
}


void getAllWords(trie *root, std::string currPrefix, std::vector<std::string> &words){
  // std::cout<<"called with prefix "<<currPrefix<<'\n';
  if (root->isWordEnd){
    words.push_back(currPrefix);
    // std::cout<<"added word "<<currPrefix<<'\n';
  }
  if (isLastNode(root)){
    return;
  }
  for (int i=0; i<NUM_ALPHABETS; i++){
    if (root->children[i]!=nullptr){
      char c = (i==NUM_ALPHABETS-1)?' ':('a'+i);
      // currPrefix.push_back(c);
      getAllWords(root->children[i], currPrefix+c, words);
    }
  }
}



class prefixTree{
private:
  int num_words;
  trie *root;
public:
  prefixTree();
  void insert(std::string key);
  bool search(std::string key);
  bool searchPrefix(std::string key);
  int getNumWords();
  std::vector<std::string> wordsWIthPrefix(std::string key);
};


prefixTree::prefixTree(){
  root = createTrieNode();
  num_words = 0;
}


void prefixTree::insert(std::string key){
  trie *crawler = this->root;
  for (char c: key){
    int idx = (c==' ')?(NUM_ALPHABETS-1):(c-'a');
    if (crawler->children[idx]==nullptr){
      crawler->children[idx] = createTrieNode();
    }
    crawler = crawler->children[idx];
  }
  crawler->isWordEnd = true;
  this->num_words++;
}


bool prefixTree::search(std::string key){
  trie *crawler = this->root;
  for (char c: key){
    int idx = (c==' ')?(NUM_ALPHABETS-1):(c-'a');
    if (crawler->children[idx]==nullptr){
      return false;
    }
    crawler = crawler->children[idx];
  }
  return crawler!=nullptr && crawler->isWordEnd;
}


bool prefixTree::searchPrefix(std::string key){
  trie *crawler = this->root;
  for (char c: key){
    int idx = (c==' ')?(NUM_ALPHABETS-1):(c-'a');
    if (crawler->children[idx]==nullptr){
      return false;
    }
    crawler = crawler->children[idx];
  }
  return crawler!=nullptr;
}


int prefixTree::getNumWords(){
  return this->num_words;
}


std::vector<std::string> prefixTree::wordsWIthPrefix(std::string key){
  if (!this->searchPrefix(key)){
    return {};
  }
  if (this->search(key)){
    return {key};
  }
  std::vector<std::string> words;
  trie *crawler = this->root;
  for (char c: key){
    int idx = (c==' ')?(NUM_ALPHABETS-1):(c-'a');
    if (crawler->children[idx]==nullptr){
      return {};
    }
    crawler = crawler->children[idx];
  }
  getAllWords(crawler, key, words);
  return words;
}


void test(){
  prefixTree pt;
  pt.insert("thus");
  pt.insert("this");
  pt.insert("there");
  pt.insert("thin");
  // pt.search("thin")?std::cout<<"thin is present"<<'\n':std::cout<<"thin is not present"<<'\n';
  // pt.search("then")?std::cout<<"then is present"<<'\n':std::cout<<"then is not present"<<'\n';
  // std::cout<<"Number of words = "<<pt.getNumWords()<<'\n';
  std::vector<std::string> words = pt.wordsWIthPrefix("t");
  for (std::string word: words){
    std::cout<<word<<'\n';
  }

}


prefixTree insertFileContent(prefixTree pt, std::string filename){
  std::ifstream file;
  std::string line;
  file.open(filename);
  while (std::getline(file, line)){
    std::string mod_line = "";
    for (char c: line){
      if (c>='A' && c<='Z') mod_line.push_back(c+'a'-'A');
      if (c==' ' || (c>='a' && c<='z')) mod_line.push_back(c);
    }
    // std::cout<<mod_line<<'\n';
    pt.insert(mod_line);
  }
  file.close();
  return pt;
}


void test2(){
  prefixTree pt;
  pt = insertFileContent(pt, "corpora/idioms.txt");
  pt = insertFileContent(pt, "corpora/proverbs.txt");
  pt = insertFileContent(pt, "corpora/phrases.txt");

  std::cout<<"every cloud\n--------------------------------------------------------------\n";
  std::vector<std::string> things = pt.wordsWIthPrefix("every cloud");
  for (std::string thing: things){
    std::cout<<thing<<'\n';
  }
  std::cout<<"______________________________________________________________\n";

  std::cout<<"every\n--------------------------------------------------------------\n";
  things = pt.wordsWIthPrefix("every");
  for (std::string thing: things){
    std::cout<<thing<<'\n';
  }
  std::cout<<"______________________________________________________________\n";

}



int main(){
  // test();
  test2();
  return 0;
}
