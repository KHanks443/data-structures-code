#include "bstA.h"
#include <climits>

BST::BST(int size)
{
    capacity = size;
    tree = new int[capacity];
    for (int i = 0; i < capacity; i++)
        tree[i] = INT_MIN;
}

BST::~BST()
{
    delete[] tree;
}

int BST::leftChild(int index) { return 2 * index + 1; }
int BST::rightChild(int index) { return 2 * index + 2; }
int BST::parent(int index) { return (index - 1) / 2; }

void BST::insertI(int data)
{
    int index = 0;

    while (index < capacity)
    {
        if (tree[index] == INT_MIN)
        {
            tree[index] = data;
            return;
        }
        if (data == tree[index])
            return;
        if (data < tree[index])
            index = leftChild(index);
        else
            index = rightChild(index);
    }
}

void BST::insertR(int data)
{
    insertRhelper(0, data);
}

void BST::insertRhelper(int index, int data)
{
    if (index >= capacity)
        return;

    if (tree[index] == INT_MIN)
    {
        tree[index] = data;
        return;
    }
    if (data == tree[index])
        return;

    if (data < tree[index])
        insertRhelper(leftChild(index), data);
    else
        insertRhelper(rightChild(index), data);
}

bool BST::searchI(int data)
{
    int index = 0;

    while (index < capacity && tree[index] != INT_MIN)
    {
        if (tree[index] == data)
            return true;
        if (data < tree[index])
            index = leftChild(index);
        else
            index = rightChild(index);
    }
    return false;
}

bool BST::searchR(int data)
{
    return searchRhelper(0, data);
}

bool BST::searchRhelper(int index, int data)
{
    if (index >= capacity || tree[index] == INT_MIN)
        return false;

    if (tree[index] == data)
        return true;

    if (data < tree[index])
        return searchRhelper(leftChild(index), data);
    return searchRhelper(rightChild(index), data);
}

void BST::preorder(int index)
{
    if (index >= capacity || tree[index] == INT_MIN)
        return;
    cout << tree[index] << " ";
    preorder(leftChild(index));
    preorder(rightChild(index));
}

void BST::inorder(int index)
{
    if (index >= capacity || tree[index] == INT_MIN)
        return;
    inorder(leftChild(index));
    cout << tree[index] << " ";
    inorder(rightChild(index));
}

void BST::postorder(int index)
{
    if (index >= capacity || tree[index] == INT_MIN)
        return;
    postorder(leftChild(index));
    postorder(rightChild(index));
    cout << tree[index] << " ";
}

void BST::printTree(int index, int depth)
{
    if (index >= capacity || tree[index] == INT_MIN)
        return;

    printTree(rightChild(index), depth + 1);

    for (int i = 0; i < depth; i++)
        cout << "   ";
    cout << tree[index] << endl;

    printTree(leftChild(index), depth + 1);
}

// ==================== TASKS ====================

int BST::height(int index)
{
    if (index >= capacity || tree[index] == INT_MIN)
        return -1;

    int lh = height(leftChild(index));
    int rh = height(rightChild(index));

    return max(lh, rh) + 1;
}

int BST::countNodes(int index)
{
    if (index >= capacity || tree[index] == INT_MIN)
        return 0;

    return 1 + countNodes(leftChild(index)) + countNodes(rightChild(index));
}

string BST::isBalanced()
{
    int lh = height(leftChild(0));
    int rh = height(rightChild(0));

    if (lh == rh)
        return "Yes";
    if (lh > rh)
        return "Left-heavy";
    return "Right-heavy";
}

void BST::deleteNode(int data)
{
    int index = 0;

    while (index < capacity && tree[index] != INT_MIN)
    {
        if (tree[index] == data)
            break;

        if (data < tree[index])
            index = leftChild(index);
        else
            index = rightChild(index);
    }

    if (index >= capacity || tree[index] == INT_MIN)
        return;

    int left = leftChild(index);
    int right = rightChild(index);

    if ((left >= capacity || tree[left] == INT_MIN) &&
        (right >= capacity || tree[right] == INT_MIN))
    {
        tree[index] = INT_MIN;
        return;
    }

    if ((left < capacity && tree[left] != INT_MIN) &&
        (right < capacity && tree[right] != INT_MIN))
    {
        int succ = right;
        while (leftChild(succ) < capacity && tree[leftChild(succ)] != INT_MIN)
            succ = leftChild(succ);

        tree[index] = tree[succ];
        deleteNode(tree[succ]);
        return;
    }

    int child = (left < capacity && tree[left] != INT_MIN) ? left : right;
    tree[index] = tree[child];
    deleteNode(tree[child]);
}

void BST::BFS()
{
    Queue q(1000);

    if (tree[0] == INT_MIN)
        return;

    q.enqueue(0);

    while (!q.isEmpty())
    {
        int i = q.dequeue();

        if (i < capacity && tree[i] != INT_MIN)
        {
            cout << tree[i] << " ";

            int left = leftChild(i);
            int right = rightChild(i);

            if (left < capacity && tree[left] != INT_MIN)
                q.enqueue(left);
            if (right < capacity && tree[right] != INT_MIN)
                q.enqueue(right);
        }
    }
    cout << endl;
}
