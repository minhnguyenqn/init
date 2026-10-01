#ifndef CIRCULARLINKEDLIST_H
#define CIRCULARLINKEDLIST_H

#include "IList.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace std;

template<class T>
class CircularLinkedList : public IList<T> {
public:
    class Node;

protected:
    Node* head;
    Node* tail;
    int count;
    bool (*itemEqual)(T& lhs, T& rhs);
    void (*deleteUserData)(CircularLinkedList<T>*);

public:
    CircularLinkedList(
        void (*deleteUserData)(CircularLinkedList<T>*) = 0,
        bool (*itemEqual)(T&, T&) = 0
    )
        : head(nullptr), tail(nullptr), count(0),
          itemEqual(itemEqual), deleteUserData(deleteUserData) {}

    CircularLinkedList(const CircularLinkedList<T>& list)
        : head(nullptr), tail(nullptr), count(0),
          itemEqual(list.itemEqual), deleteUserData(list.deleteUserData) {
        copyFrom(list);
    }

    CircularLinkedList<T>& operator=(const CircularLinkedList<T>& list) {
        if (this == &list) return *this;
        removeInternalData();
        itemEqual = list.itemEqual;
        deleteUserData = list.deleteUserData;
        copyFrom(list);
        return *this;
    }

    ~CircularLinkedList() {
        removeInternalData();
    }

    void add(T e) override {
        // TODO Q2
        Node* newNode = new Node(e);
    if (count == 0) {
        head = tail = newNode;
        newNode->next = newNode;
    } else {
        newNode->next = head;
        tail->next = newNode;
        tail = newNode;
    }
    ++count;
    }

    void add(int index, T e) override {
        // TODO Q2
    if (index < 0 || index > count) {
        throw out_of_range("Index is out of range");
    }

    if (index == count) {
        add(e);
        return;
    }
    if (index == 0) {
        Node* newNode = new Node(e, head);
        head = newNode;
        tail->next = head;
    } else {
        Node* previous = head;

        for (int i = 0; i < index - 1; ++i) {
            previous = previous->next;
        }
        previous->next = new Node(e, previous->next);
    }
    ++count;
    }

    T removeAt(int index) override {
        if (index < 0 || index >= count) {
        throw out_of_range("Index is out of range");
    }

    Node* previous = tail;

    for (int i = 0; i < index; ++i) {
        previous = previous->next;
    }

    Node* deletedNode = previous->next;
    T removedData = deletedNode->data;

    if (count == 1) {
        head = tail = nullptr;
    } else {
        previous->next = deletedNode->next;

        if (deletedNode == head) {
            head = deletedNode->next;
        }

        if (deletedNode == tail) {
            tail = previous;
        }

        tail->next = head;
    }

    delete deletedNode;
    --count;
    return removedData;
    }

    bool removeItem(T item, void (*removeItemData)(T) = 0) override {
       Node* previous = tail;
    Node* current = head;

    for (int i = 0; i < count; ++i) {
        if (equals(current->data, item, itemEqual)) {
            if (count == 1) {
                head = tail = nullptr;
            } else {
                previous->next = current->next;

                if (current == head) {
                    head = current->next;
                }

                if (current == tail) {
                    tail = previous;
                }

                tail->next = head;
            }

            --count;

            if (removeItemData != 0) {
                removeItemData(current->data);
            }

            delete current;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
    }

    void clear() override {
        removeInternalData();
    }

    T& get(int index) override {
        if (index < 0 || index >= count) {
        throw out_of_range("Index is out of range");
    }

    Node* current = head;

    for (int i = 0; i < index; ++i) {
        current = current->next;
    }

    return current->data;
    }

    int indexOf(T item) override {
        Node* current = head;
    for (int i = 0; i < count; ++i) {
        if (equals(current->data, item, itemEqual)) {
            return i;
        }
        current = current->next;
    }
    return -1;
    }

    bool empty() override { return count == 0; }
    int size() override { return count; }
    bool contains(T item) override { return indexOf(item) >= 0; }

    string toString(string (*item2str)(T&) = 0) override {
        stringstream ss;
        ss << "[";
        Node* cur = head;
        for (int i = 0; i < count; ++i) {
            if (i > 0) ss << ", ";
            if (item2str) ss << item2str(cur->data);
            else ss << cur->data;
            cur = cur->next;
        }
        ss << "]";
        return ss.str();
    }

    void println(string (*item2str)(T&) = 0) {
        cout << toString(item2str) << endl;
    }

protected:
    static bool equals(T& lhs, T& rhs, bool (*itemEqual)(T&, T&)) {
        return itemEqual ? itemEqual(lhs, rhs) : (lhs == rhs);
    }

    void copyFrom(const CircularLinkedList<T>& list) {
        Node* cur = list.head;
        for (int i = 0; i < list.count; ++i) {
            add(cur->data);
            cur = cur->next;
        }
    }

    void removeInternalData() {
        if (deleteUserData != 0) deleteUserData(this);
        Node* cur = head;
        for (int i = 0; i < count; ++i) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head = tail = nullptr;
        count = 0;
    }

public:
    class Node {
    public:
        T data;
        Node* next;
        Node(T data, Node* next = nullptr) : data(data), next(next) {}
    };
};

#endif
