// Carl Christian G. Tilid
// 2-4 Treee
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int keys[3];
    Node* children[4];
    bool isLeaf;
    int keyCount;

    Node(bool leaf = true)
        : isLeaf(leaf), keyCount(0) {
        for (int i = 0; i < 4; i++) {
            children[i] = nullptr;
        }
    }
};

Node* createNode(int key) {
    Node* node = new Node(true);
    node->keys[0] = key;
    node->keyCount = 1;
    return node;
}
void splitChild(Node* parentNode, int childIndex, Node* childNode) {
    Node* newNode = new Node(childNode->isLeaf);
    newNode->keyCount = 1;
    newNode->keys[0] = childNode->keys[2];

    if (!childNode->isLeaf) {
        for (int i = 0; i < 2; i++) {
            newNode->children[i] = childNode->children[i + 2];
        }
    }

    childNode->keyCount = 1;

    for (int i = parentNode->keyCount; i > childIndex; i--) {
        parentNode->children[i + 1] = parentNode->children[i];
    }

    parentNode->children[childIndex + 1] = newNode;

    for (int i = parentNode->keyCount - 1; i >= childIndex; i--) {
        parentNode->keys[i + 1] = parentNode->keys[i];
    }

    parentNode->keys[childIndex] = childNode->keys[1];
    parentNode->keyCount++;
}

void insertNonFull(Node* node, int key) {
    int i = node->keyCount - 1;
    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->keyCount++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        if (node->children[i + 1]->keyCount == 3) {
            splitChild(node, i + 1, node->children[i + 1]);
            if (key > node->keys[i + 1]) {
                i++;
            }
        }
        insertNonFull(node->children[i + 1], key);
    }
}

void displayTree(Node* node) {
    for (int i = 0; i < node->keyCount; i++) {
        if (!node->isLeaf) {
            displayTree(node->children[i]);
        }
        cout << node->keys[i] << " ";
    }

    if (!node->isLeaf) {
        displayTree(node->children[node->keyCount]);
    }
}

bool searchKey(Node* node, int key) {
    int i = 0;
    while (i < node->keyCount && key > node->keys[i]) {
        i++;
    }

    if (node->keys[i] == key) {
        return true;
    }

    if (node->isLeaf) {
        return false;
    }

    return searchKey(node->children[i], key);
}

class Tree {
public:
    Node* root;

    Tree() {
        root = nullptr;
    }

    void insert(int key) {
        if (root == nullptr) {
            root = createNode(key);
        } else {
            if (root->keyCount == 3) {
                Node* newRoot = new Node(false);
                newRoot->children[0] = root;
                splitChild(newRoot, 0, root);
                root = newRoot;
            }
            insertNonFull(root, key);
        }
    }

    bool search(int key) {
        if (root == nullptr) {
            return false;
        }
        return searchKey(root, key);
    }

    void display() {
        if (root == nullptr) {
            cout << "Empty tree." << endl;
        } else {
            displayTree(root);
        }
        cout << endl;
    }
};

int main() {
    Tree tree;

    int arr[] = {30, 40, 70, 20, 10, 50, 25, 55};
    int n = sizeof(arr) / sizeof(arr[0]);

    for (int i = 0; i < n; i++) {
        tree.insert(arr[i]);
    }

    cout << "Key 25 found: " << tree.search(25) << endl;
    cout << "Key 70 found: " << tree.search(70) << endl;
    cout << "2-4 Search Tree: ";
    tree.display();

    return 0;
}
