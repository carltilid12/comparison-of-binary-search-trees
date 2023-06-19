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
    
    void deleteKey(int key) {
        if (root == nullptr) {
            cout << "Tree is empty." << endl;
            return;
        }
        
        deleteFromNode(root, key);
        
        // If the root node has no keys after deletion, set the new root
        if (root->keyCount == 0) {
            Node* temp = root;
            if (root->isLeaf) {
                root = nullptr;
            } else {
                root = root->children[0];
            }
            delete temp;
        }
    }

private:
    void deleteFromNode(Node* node, int key) {
        int i = 0;
        while (i < node->keyCount && key > node->keys[i]) {
            i++;
        }

        // Case 1: Key is found in the current node
        if (i < node->keyCount && node->keys[i] == key) {
            if (node->isLeaf) {
                // Case 1a: Key is in a leaf node
                for (int j = i; j < node->keyCount - 1; j++) {
                    node->keys[j] = node->keys[j + 1];
                }
                node->keyCount--;
            } else {
                // Case 1b: Key is in an internal node
                Node* predecessorChild = node->children[i];
                Node* successorChild = node->children[i + 1];
                
                if (predecessorChild->keyCount >= 2) {
                    // Sub-case 1b(i): The predecessor child has at least 2 keys
                    int predecessorKey = findPredecessor(predecessorChild);
                    node->keys[i] = predecessorKey;
                    deleteFromNode(predecessorChild, predecessorKey);
                } else if (successorChild->keyCount >= 2) {
                    // Sub-case 1b(ii): The successor child has at least 2 keys
                    int successorKey = findSuccessor(successorChild);
                    node->keys[i] = successorKey;
                    deleteFromNode(successorChild, successorKey);
                } else {
                    // Sub-case 1b(iii): Both predecessor and successor child have only 1 key
                    mergeNodes(node, i, predecessorChild, successorChild);
                    deleteFromNode(predecessorChild, key);
                }
            }
        } else {
            // Case 2: Key is not found in the current node
            Node* childNode = node->children[i];
            Node* leftSibling = (i > 0) ? node->children[i - 1] : nullptr;
            Node* rightSibling = (i < node->keyCount) ? node->children[i + 1] : nullptr;
            
            // Case 2a: The child node has only 1 key
            if (childNode->keyCount == 1) {
                // Case 2ai: The left sibling has at least 2 keys
                if (leftSibling != nullptr && leftSibling->keyCount >= 2) {
                    borrowFromLeftSibling(node, i, childNode, leftSibling);
                }
                // Case 2aii: The right sibling has at least 2 keys
                else if (rightSibling != nullptr && rightSibling->keyCount >= 2) {
                    borrowFromRightSibling(node, i, childNode, rightSibling);
                }
                // Case 2aiii: Merge with the left sibling
                else if (leftSibling != nullptr) {
                    mergeNodes(node, i - 1, leftSibling, childNode);
                    childNode = leftSibling;
                }
                // Case 2aiv: Merge with the right sibling
                else if (rightSibling != nullptr) {
                    mergeNodes(node, i, childNode, rightSibling);
                }
            }
            
            deleteFromNode(childNode, key);
        }
    }
    
    int findPredecessor(Node* node) {
        if (node->isLeaf) {
            return node->keys[node->keyCount - 1];
        }
        return findPredecessor(node->children[node->keyCount]);
    }
    
    int findSuccessor(Node* node) {
        if (node->isLeaf) {
            return node->keys[0];
        }
        return findSuccessor(node->children[0]);
    }
    
    void borrowFromLeftSibling(Node* parentNode, int childIndex, Node* childNode, Node* leftSibling) {
        // Shift the keys and children in the child node
        for (int i = childNode->keyCount - 1; i >= 0; i--) {
            childNode->keys[i + 1] = childNode->keys[i];
        }
        if (!childNode->isLeaf) {
            for (int i = childNode->keyCount; i >= 0; i--) {
                childNode->children[i + 1] = childNode->children[i];
            }
        }
        
        // Move the key from the parent node to the child node
        childNode->keys[0] = parentNode->keys[childIndex - 1];
        
        // Move the rightmost key from the left sibling to the parent node
        parentNode->keys[childIndex - 1] = leftSibling->keys[leftSibling->keyCount - 1];
        
        if (!childNode->isLeaf) {
            // Move the rightmost child from the left sibling to the child node
            childNode->children[0] = leftSibling->children[leftSibling->keyCount];
        }
        
        // Update the key counts
        childNode->keyCount++;
        leftSibling->keyCount--;
    }
    
    void borrowFromRightSibling(Node* parentNode, int childIndex, Node* childNode, Node* rightSibling) {
        // Move the key from the parent node to the child node
        childNode->keys[childNode->keyCount] = parentNode->keys[childIndex];
        
        // Move the leftmost key from the right sibling to the parent node
        parentNode->keys[childIndex] = rightSibling->keys[0];
        
        // Shift the keys and children in the right sibling
        for (int i = 0; i < rightSibling->keyCount - 1; i++) {
            rightSibling->keys[i] = rightSibling->keys[i + 1];
        }
        if (!rightSibling->isLeaf) {
            for (int i = 0; i <= rightSibling->keyCount; i++) {
                rightSibling->children[i] = rightSibling->children[i + 1];
            }
        }
        
        // Update the key counts
        childNode->keyCount++;
        rightSibling->keyCount--;
    }
    
    void mergeNodes(Node* parentNode, int childIndex, Node* leftNode, Node* rightNode) {
        // Move the key from the parent node to the left node
        leftNode->keys[leftNode->keyCount] = parentNode->keys[childIndex];
        
        // Move the keys and children from the right node to the left node
        for (int i = 0; i < rightNode->keyCount; i++) {
            leftNode->keys[leftNode->keyCount + 1 + i] = rightNode->keys[i];
        }
        if (!leftNode->isLeaf) {
            for (int i = 0; i <= rightNode->keyCount; i++) {
                leftNode->children[leftNode->keyCount + 1 + i] = rightNode->children[i];
            }
        }
        
        // Shift the keys in the parent node
        for (int i = childIndex; i < parentNode->keyCount - 1; i++) {
            parentNode->keys[i] = parentNode->keys[i + 1];
        }
        // Shift the children in the parent node
        for (int i = childIndex + 1; i < parentNode->keyCount; i++) {
            parentNode->children[i] = parentNode->children[i + 1];
        }
        
        // Update the key counts
        leftNode->keyCount += rightNode->keyCount + 1;
        parentNode->keyCount--;
        
        delete rightNode;
    }
};

int main() {
    Tree tree;
    srand(time(0));
    clock_t time_req;

    int size = 100;
    int arr[size];
    
    for(int i=0; i<size; i++){
        arr[i] = i;
    }
    random_shuffle(&arr[0], &arr[size]);

    time_req = clock();
    // Insert some keys
    for(int i=0; i<size; i++){
        tree.insert(arr[i]);
    }
    
    // Display the tree
    //cout << "Tree after insertion: ";
    //tree.display();
    
    // Search for keys
    //cout << "Searching for keys..." << endl;
    //cout << "Key 10: " << (tree.search(10) ? "Found" : "Not found") << endl;

    // Delete some keys
    //cout << "Deleting keys..." << endl;
    //tree.deleteKey(5);
    
    // Display the tree after deletion
    //cout << "Tree after deletion: ";
    //tree.display();
    time_req = clock() - time_req;
    cout << "The time it took = " << ((float)time_req/CLOCKS_PER_SEC)*1000 << endl;
    return 0;
}
