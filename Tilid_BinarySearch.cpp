//Carl Christian G. Tilid
//Binary Search Tree
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

// Node structure
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree class
class BST {
public:
    Node* root;

    BST() {
        root = nullptr;
    }

    // insert a value into the BST
    void insert(int value) {
        root = insertNode(root, value);
    }

    // search for a value in the BST
    bool search(int value) {
        return searchNode(root, value);
    }

    void remove(int value){
        root = deleteNode(root, value);
    }   

    // Helper function to insert a node recursively
    Node* insertNode(Node* root, int value) {
        if (root == nullptr) {
            root = new Node(value);
        } else if (value <= root->data) {
            root->left = insertNode(root->left, value);
        } else {
            root->right = insertNode(root->right, value);
        }
        return root;
    }

    // Helper function to delete a node recursively
    Node* deleteNode(Node* root, int value) {
        if (root == nullptr) {
            return root;
        } else if (value < root->data) {
            root->left = deleteNode(root->left, value);
        } else if (value > root->data) {
            root->right = deleteNode(root->right, value);
        } else {
            // Node to be deleted found

            // Case 1: No child or only one child
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            // Case 2: Two children
            Node* minNode = findMinNode(root->right);
            root->data = minNode->data;
            root->right = deleteNode(root->right, minNode->data);
        }
        return root;
    }

    // Helper function to search for a node recursively
    bool searchNode(Node* root, int value) {
        if (root == nullptr) {
            return false;
        } else if (value == root->data) {
            return true;
        } else if (value < root->data) {
            return searchNode(root->left, value);
        } else {
            return searchNode(root->right, value);
        }
    }

    // Helper function to find the minimum node in the BST
    Node* findMinNode(Node* root) {
        while (root->left != nullptr) {
            root = root->left;
        }
        return root;
    }
};

// Test the Binary Search Tree implementation
int main() {
    srand(time(0));
    clock_t time_req;
    BST bst;
    int insert=0;
    int size=1000000;
    int first=0;

    time_req = clock();
    // Insert nodes into the BST
    for (int i=0; i < size; i++){
        insert = rand() % size;
        bst.insert(insert);
        if (i==0){
            first = insert;
        }
    }
    cout << bst.search(20) << endl;
    bst.remove(first);

    time_req = clock() - time_req;
    cout << "The time it took = " << ((float)time_req/CLOCKS_PER_SEC)*1000 << endl;
    return 0;
}
