#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Allan Jean-Jacques
COP3502
Lab Assignment 10
Due 4/19/24
*/

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int count;
};

// Trie structure
struct Trie {
    struct TrieNode* root;
};

// Helper function to create a new trie node
struct TrieNode* createNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (!newNode) {
        return NULL; // Handle malloc failure
    }
    newNode->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Inserts the word to the trie structure
void insert(struct Trie* pTrie, char* word) {
    if (!pTrie || !word) {
        return; // Early exit if invalid pointers
    }

    struct TrieNode* currentNode = pTrie->root;
    for (int i = 0; word[i]; i++) {
        int index = word[i] - 'a';
        if (!currentNode->children[index]) {
            currentNode->children[index] = createNode();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie* pTrie, char* word) {
    if (!pTrie || !word) {
        return 0; // Return 0 if pointers are invalid
    }

    struct TrieNode* currentNode = pTrie->root;
    for (int i = 0; word[i]; i++) {
        int index = word[i] - 'a';
        if (!currentNode->children[index]) {
            return 0;
        }
        currentNode = currentNode->children[index];
    }
    return currentNode->count;
}

// Recursive helper to deallocate trie nodes
void deallocateTrieNodes(struct TrieNode* node) {
    if (node) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                deallocateTrieNodes(node->children[i]);
            }
        }
        free(node);
    }
}

// Deallocates the trie structure
struct Trie* deallocateTrie(struct Trie* pTrie) {
    if (!pTrie) return NULL;

    deallocateTrieNodes(pTrie->root); // Correctly deallocate all nodes
    free(pTrie);
    return NULL;
}

// Initializes a trie structure
struct Trie* createTrie() {
    struct Trie* pTrie = (struct Trie*)malloc(sizeof(struct Trie));
    if (!pTrie) {
        return NULL; // Handle malloc failure
    }
    pTrie->root = createNode();
    if (!pTrie->root) {
        free(pTrie); // Handle malloc failure of the node
        return NULL;
    }
    return pTrie;
}

// This function will return the number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    int numWords;
    fscanf(file, "%d", &numWords);

    for (int i = 0; i < numWords; i++) {
        pInWords[i] = (char*)malloc(256 * sizeof(char));
        fscanf(file, "%s", pInWords[i]);
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char* inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie == NULL) {
        printf("Trie deallocated successfully.\n");
    }

    // Free dynamically allocated memory for inWords
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}