#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "ISort.h"
#include <stdexcept>
using namespace std;

template<class T>
class QuickSort : public ISort<T> {
private:
    int (*pivotSelection)(T*, int);

public:
    QuickSort(int (*pivotSelection)(T*, int) = 0)
        : pivotSelection(pivotSelection) {}

    void sort(T array[], int size, int (*comparator)(T&, T&) = 0) override {
        // TODO Q4
        if (size <= 1) {
            return;
        }
        quickSort(array, 0, size - 1, comparator);
    }

private:
    void quickSort(T array[], int left, int right,
                   int (*comparator)(T&, T&) = 0) {
        while (left < right) {
            int pivotIndex = partition(array, left, right, comparator);
            if (pivotIndex - left < right - pivotIndex) {
                quickSort(array, left, pivotIndex - 1, comparator);
                left = pivotIndex + 1;
            } else {
                quickSort(array, pivotIndex + 1, right, comparator);
                right = pivotIndex - 1;
            }
        }
    }

    int partition(T array[], int left, int right,
                  int (*comparator)(T&, T&) = 0) {
        int pivotIndex = left + (right - left) / 2;

        if (pivotSelection != 0) {
            int length = right - left + 1;
            int selected = pivotSelection(array + left, length);

            if (selected < 0 || selected >= length) {
                throw out_of_range("Pivot index is out of range");
            }

            pivotIndex = left + selected;
        }
        swap(array[pivotIndex], array[right]);

        int position = left;

        for (int i = left; i < right; ++i) {
            bool comesBefore;

            if (comparator != 0) {
                comesBefore = comparator(array[i], array[right]) < 0;
            } else {
                comesBefore = array[i] < array[right];
            }

            if (comesBefore) {
                swap(array[i], array[position]);
                ++position;
            }
        }
        swap(array[position], array[right]);

        return position;
    }
};

#endif
