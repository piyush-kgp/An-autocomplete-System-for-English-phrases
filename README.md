
This is a small demo of an autocomplete system using Trie data structure written in C++.

Example:
```
every
--------------------------------------------------------------
everybody out
everything but the kitchen sink
every cloud has a silver lining
every dog has its day
every man has his price
______________________________________________________________

```

### Trie
Trie or prefix tree is a beautiful data structure. Beautiful because it is extremely efficient for searching. It can search for a key in O(M) time where M is the maximum string length.

Conceptually `trie` is very similar to any other tree data structure. Every node has 26 children pointers most of which are null pointers, with `trie` pointers at nodes with prefixes.

Below is a prefix tree for words ["this", "thin", "there", "thus"].

```
       t
       |
       h
    /  |  \
   e   i   u
   |  / \  |
   r  n  s s
   |
   e  
```

Our implementation of `prefixTree` class has these useful methods:
- `insert(key)` - Inserts a new word/phrase in the Trie in O(M) time where M=length of key.
- `search(key)` - Searches for a word/phrase in the Trie in O(M) time where M=length of key.
- `wordsWIthPrefix(key)` - This is the autocomplete system. Takes O(N*M) time where N = total number of words and M=length of key. But that is the worst case complexity. In reality, N is the number of words with prefix as the key; so it will practically be far less than the total number of words/phrases in Trie. For example for the example "every" above, N=5.
