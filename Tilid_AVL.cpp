// Carl Christian G. Tilid
// AVL
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
    int height;
    Node* left;
    Node* right;

    Node(int value) {
        data = value;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

// AVL Binary Search Tree class
class AVLTree {
public:
    Node* root;

    AVLTree() {
        root = nullptr;
    }

    // Public function to insert a value into the AVL tree
    void insert(int value) {
        root = insertNode(root, value);
    }

    // Public function to remove a value from the AVL tree
    void remove(int value) {
        root = deleteNode(root, value);
    }

    // Public function to display the AVL tree (in-order traversal)
    void display() {
        inOrderTraversal(root);
        cout << endl;
    }

    // Public function to construct an AVL tree from an array
    void constructFromArray(int arr[], int size) {
        for (int i = 0; i < size; i++) {
            insert(arr[i]);
        }
    }

private:
    // Helper function to get the height of a node
    int getHeight(Node* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    // Helper function to calculate the balance factor of a node
    int getBalanceFactor(Node* node) {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    // Helper function to update the height of a node
    void updateHeight(Node* node) {
        if (node == nullptr)
            return;
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }

    // Helper function to perform a right rotation
    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        Node* subtree = newRoot->right;

        newRoot->right = node;
        node->left = subtree;

        updateHeight(node);
        updateHeight(newRoot);

        return newRoot;
    }

    // Helper function to perform a left rotation
    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        Node* subtree = newRoot->left;

        newRoot->left = node;
        node->right = subtree;

        updateHeight(node);
        updateHeight(newRoot);

        return newRoot;
    }

    // Helper function to balance the AVL tree after insertion or deletion
    Node* balance(Node* node) {
        if (node == nullptr)
            return node;

        updateHeight(node);

        int balanceFactor = getBalanceFactor(node);

        // Left Left Case
        if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0)
            return rotateRight(node);

        // Left Right Case
        if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Right Case
        if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0)
            return rotateLeft(node);

        // Right Left Case
        if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    // Helper function to insert a node recursively
    Node* insertNode(Node* node, int value) {
        if (node == nullptr)
            return new Node(value);

        if (value < node->data)
            node->left = insertNode(node->left, value);
        else if (value > node->data)
            node->right = insertNode(node->right, value);
        else
            return node;  // Duplicate values are not allowed in the tree

        return balance(node);
    }

    // Helper function to find the node with the minimum value in a subtree
    Node* findMinNode(Node* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    // Helper function to delete a node recursively
    Node* deleteNode(Node* node, int value) {
        if (node == nullptr)
            return node;

        if (value < node->data)
            node->left = deleteNode(node->left, value);
        else if (value > node->data)
            node->right = deleteNode(node->right, value);
        else {
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = node->left ? node->left : node->right;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                Node* temp = findMinNode(node->right);
                node->data = temp->data;
                node->right = deleteNode(node->right, temp->data);
            }
        }

        if (node == nullptr)
            return node;

        return balance(node);
    }

    // Helper function for in-order traversal (used to display the tree)
    void inOrderTraversal(Node* node) {
        if (node == nullptr)
            return;

        inOrderTraversal(node->left);
        cout << node->data << " ";
        inOrderTraversal(node->right);
    }
};

// Test the AVL Binary Search Tree implementation
int main() {
    srand(time(0));
    clock_t time_req;

    int size = 10;
    int arr[size];
    
    for(int i=0; i<size; i++){
        arr[i] = i;
    }
    random_shuffle(&arr[0], &arr[size]);

    time_req = clock();

    AVLTree avl;
    avl.constructFromArray(arr, size);

    time_req = clock() - time_req;
    cout << "The time it took = " << ((float)time_req/CLOCKS_PER_SEC)*1000 << endl;
    return 0;
}