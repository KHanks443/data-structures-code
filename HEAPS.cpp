#include "heap.h"

HEAP::HEAP(int size)
{
    capacity = size;
    heapSize = 0;
    array = new int[capacity];
}

HEAP::~HEAP()
{
    delete[] array;
}

int HEAP::leftChild(int index)
{
    return 2 * index + 1;
}

int HEAP::rightChild(int index)
{
    return 2 * index + 2;
}

int HEAP::parent(int index)
{
    return (index - 1) / 2;
}

void HEAP::heapifyUp(int index)
{
    while(index > 0 && array[parent(index)] < array[index])
    {
        swap(array[parent(index)], array[index]);
        index = parent(index);
    }
}

void HEAP::heapifyDown(int index)
{
    int largest = index;
    int left = leftChild(index);
    int right = rightChild(index);

    if(left < heapSize && array[left] > array[largest])
        largest = left;
    if(right < heapSize && array[right] > array[largest])
        largest = right;

    if(largest != index)
    {
        swap(array[index], array[largest]);
        heapifyDown(largest);
    }
}

void HEAP::heapifyDownMin(int index)
{
    int smallest = index;
    int left = leftChild(index);
    int right = rightChild(index);

    if(left < heapSize && array[left] < array[smallest])
        smallest = left;
    if(right < heapSize && array[right] < array[smallest])
        smallest = right;

    if(smallest != index)
    {
        swap(array[index], array[smallest]);
        heapifyDownMin(smallest);
    }
}

void HEAP::insertH(int data)
{
    if(heapSize == capacity)
        return;
    array[heapSize] = data;
    heapifyUp(heapSize);
    heapSize++;
}

void HEAP::deleteMax()
{
    if(heapSize == 0)
        return;
    array[0] = array[heapSize - 1];
    heapSize--;
    heapifyDown(0);
}

void HEAP::deleteH(int data)
{
    int index = -1;
    for(int i = 0; i < heapSize; i++)
        if(array[i] == data)
        {
            index = i;
            break;
        }

    if(index == -1)
        return;

    array[index] = array[heapSize - 1];
    heapSize--;
    heapifyDown(index);
}

int HEAP::peek()
{
    if(heapSize == 0)
        return -1;
    return array[0];
}

void HEAP::buildH(int arr[], int n)
{
    heapSize = n;
    for(int i = 0; i < n; i++)
        array[i] = arr[i];
    for(int i = (heapSize / 2) - 1; i >= 0; i--)
        heapifyDown(i);
}

void HEAP::replace(int oldData, int newData)
{
    int index = -1;
    for(int i = 0; i < heapSize; i++)
        if(array[i] == oldData)
        {
            index = i;
            break;
        }
    if(index == -1)
        return;

    array[index] = newData;

    if(index > 0 && array[index] > array[parent(index)])
        heapifyUp(index);
    else
        heapifyDown(index);
}

void HEAP::heapSort(int arr[], int n)
{
    buildH(arr, n);
    for(int i = heapSize - 1; i >= 0; i--)
    {
        swap(array[0], array[i]);
        heapSize--;
        heapifyDown(0);
    }
    for(int i = 0; i < n; i++)
        arr[i] = array[i];
}

void HEAP::printHeap(int index, int depth)
{
    if(index >= heapSize)
        return;

    printHeap(rightChild(index), depth + 1);
    for(int i = 0; i < depth; i++)
        cout << "   ";
    cout << array[index] << endl;
    printHeap(leftChild(index), depth + 1);
}

void HEAP::switchMinMax()
{
    bool isMax = true;
    for(int i = 1; i < heapSize; i++)
        if(array[i] < array[parent(i)])
        {
            isMax = false;
            break;
        }

    if(isMax)
    {
        for(int i = (heapSize / 2) - 1; i >= 0; i--)
            heapifyDownMin(i);
    }
    else
    {
        for(int i = (heapSize / 2) - 1; i >= 0; i--)
            heapifyDown(i);
    }
}
