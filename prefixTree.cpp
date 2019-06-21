
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#define NUM_ALPHABETS 27


struct trie{
  // Trie data structure
  trie *children[NUM_ALPHABETS];
  bool isWordEnd;
};


trie *createTrieNode(){
  // returns a pointer to a Trie Node.
  trie *temp = new trie;
  for (int i=0; i<NUM_ALPHABETS; i++){
    temp->children[i] = nullptr;
  }
  temp->isWordEnd = false;
  return temp;
}


bool isLastNode(trie *root){
  // checks whether the node passes is the last node in the tree.
  for (int i=0; i<NUM_ALPHABETS; i++){
    if (root->children[i]!=nullptr){
      return false;
    }
  }
  return true;
}


void getAllWords(trie *root, std::string currPrefix, std::vector<std::string> &words){
  // Given a Trie node "root", appends the "words" vector with all words starting from
  // that node and prefixes them with currPrefix.
  // Achieves this functionality through recursion.

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



// our actual prefix tree class
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
  // constructor method
  root = createTrieNode();
  num_words = 0;
}


void prefixTree::insert(std::string key){
  // Inserts key into Trie in O(M)
  trie *crawler = this->root;
  for (char c: key){
    // we have to take space into account. I've relegated to the last index in the children nodes.
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
  // Searches for a key in Trie in O(M)
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
  // similar to the search method above but has a smaller condition (needn't be a word)
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
  // Returns current number of words in the Trie
  return this->num_words;
}


std::vector<std::string> prefixTree::wordsWIthPrefix(std::string key){
  // autocomplete function!!
  // uses a crawler to get to the node at key and then employs the method getAllWords

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
  // Trivial tests

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
  // Useful to insert content of large files in a Trie.

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
  // Actual Tests.
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
  // Run all.
  // test();
  test2();
  return 0;
}
