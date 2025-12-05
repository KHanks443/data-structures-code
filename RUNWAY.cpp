#include "reservation.h"

reservation::reservation(int t) {
    time = t;
    left = nullptr;
    right = nullptr;
    subtreeSize = 1;
}

BST::BST() {
    root = nullptr;
}

BST::~BST() {
    clearTree(root);
}

void BST::clearTree(reservation* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

bool BST::reserve(int time) {
    if (checkConflict(root, time))
        return false;

    insert(root, time);
    return true;
}

bool BST::checkConflict(reservation* node, int time) {
    if (!node)
        return false;

    if (abs(node->time - time) <= 3)
        return true;

    if (time < node->time)
        return checkConflict(node->left, time);
    else
        return checkConflict(node->right, time);
}

void BST::insert(reservation*& node, int time) {
    if (!node) {
        node = new reservation(time);
        return;
    }

    if (time < node->time)
        insert(node->left, time);
    else
        insert(node->right, time);

    int leftSize = node->left ? node->left->subtreeSize : 0;
    int rightSize = node->right ? node->right->subtreeSize : 0;
    node->subtreeSize = 1 + leftSize + rightSize;
}

int BST::countPlanes(reservation* node, int time) {
    if (!node)
        return 0;

    if (node->time <= time) {
        int leftSize = node->left ? node->left->subtreeSize : 0;
        return leftSize + 1 + countPlanes(node->right, time);
    } else {
        return countPlanes(node->left, time);
    }
}

void BST::inorder(reservation* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->time << " ";
    inorder(root->right);
}
