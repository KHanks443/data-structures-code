#include "bstP.h"

node::node(int value)
{
    data = value;
    leftChild = nullptr;
    rightChild = nullptr;
}

BST::BST()
{
    root = nullptr;
}

BST::~BST()
{
    clearTree(root);
}

void BST::clearTree(node* root)
{
    if (root == nullptr)
        return;

    clearTree(root->leftChild);
    clearTree(root->rightChild);
    delete root;
}

void BST::insertNode(node*& root, int data)
{
    if (root == nullptr)
    {
        root = new node(data);
        return;
    }
    if (data == root->data)
        return; // duplicate
    if (data < root->data)
        insertNode(root->leftChild, data);
    else
        insertNode(root->rightChild, data);
}

node* BST::searchNode(node*& root, int data)
{
    if (root == nullptr || root->data == data)
        return root;
    if (data < root->data)
        return searchNode(root->leftChild, data);
    return searchNode(root->rightChild, data);
}

void BST::deleteNode(node*& root, int data)
{
    if (root == nullptr)
        return;

    if (data < root->data)
        deleteNode(root->leftChild, data);

    else if (data > root->data)
        deleteNode(root->rightChild, data);

    else
    {
        if (root->leftChild == nullptr)
        {
            node* temp = root->rightChild;
            delete root;
            root = temp;
        }
        else if (root->rightChild == nullptr)
        {
            node* temp = root->leftChild;
            delete root;
            root = temp;
        }
        else
        {
            node* temp = root->rightChild;
            while (temp->leftChild != nullptr)
                temp = temp->leftChild;

            root->data = temp->data;
            deleteNode(root->rightChild, temp->data);
        }
    }
}

void BST::preorder(node* root)
{
    if (root == nullptr) return;
    cout << root->data << " ";
    preorder(root->leftChild);
    preorder(root->rightChild);
}

void BST::inorder(node* root)
{
    if (root == nullptr) return;
    inorder(root->leftChild);
    cout << root->data << " ";
    inorder(root->rightChild);
}

void BST::postorder(node* root)
{
    if (root == nullptr) return;
    postorder(root->leftChild);
    postorder(root->rightChild);
    cout << root->data << " ";
}

void BST::printTree(node* root, int space)
{
    if (root == nullptr)
        return;

    space += 5;
    printTree(root->rightChild, space);

    cout << endl;
    for (int i = 5; i < space; i++)
        cout << " ";
    cout << root->data << endl;

    printTree(root->leftChild, space);
}

// ====================== TASK FUNCTIONS ======================

int BST::height(node* root)
{
    if (root == nullptr)
        return -1;
    int lh = height(root->leftChild);
    int rh = height(root->rightChild);
    return max(lh, rh) + 1;
}

int BST::countNodes(node* root)
{
    if (root == nullptr)
        return 0;
    return 1 + countNodes(root->leftChild) + countNodes(root->rightChild);
}

string BST::isBalanced()
{
    int lh = height(root->leftChild);
    int rh = height(root->rightChild);

    if (lh == rh)
        return "Yes";
    if (lh > rh)
        return "Left-heavy";
    return "Right-heavy";
}

void BST::BFS()
{
    if (root == nullptr)
        return;

    Queue q(1000);
    q.enqueue((long long)root);

    while (!q.isEmpty())
    {
        node* curr = (node*)q.dequeue();
        cout << curr->data << " ";

        if (curr->leftChild != nullptr)
            q.enqueue((long long)curr->leftChild);
        if (curr->rightChild != nullptr)
            q.enqueue((long long)curr->rightChild);
    }
    cout << endl;
}
