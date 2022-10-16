#include "hash_list.h"

#include<bits/stdc++.h>
using namespace std;

template <typename K, typename V>
node<K, V> *_insnode(K key, V value);

// Constructor
template <typename K, typename V>
hash_list<K, V>::hash_list()
{
    size = 0;
    //node <key,value> &head = node<key,value> *head;
    head = NULL;
    iter_ptr = NULL;
    
}

// Copy Constructor
template <typename K, typename V>
hash_list<K, V>::hash_list(const hash_list<K, V> &other)
{
     size = 0;
     head = NULL;
     iter_ptr = NULL;
     //recreate new linked_list
     node<K, V>* cnode = other.head;
    
     while(cnode != NULL)
     {
        insert(cnode->key, cnode->value);
        cnode = cnode->next;
     }
    
}

// Assignment operator
template <typename K, typename V>
hash_list<K, V> &hash_list<K, V>::operator=(const hash_list<K, V> &other)
{
    
    if(head == other.head){
        return *this;
    }
    
    hash_list<K, V> Tempobject = hash_list(other);
    node<K, V>* ptr = NULL;
    ptr = this -> head;
    this -> head = Tempobject.head;
    this -> size = Tempobject.size;
    Tempobject.head= ptr;
    return *this;
   
}

// Insert Node Function
template <typename K, typename V>
void hash_list<K, V>::insert(K key, V value)
{
    // Find the head node, set current to head
    node<K, V>* current = head;
    node<K, V>* previousNode = head;
    bool hasWritten = false;

    // Check if first, update head if is
    if (current == NULL)
    {
        //// std::cout << "(INSERT) head is null, writing here" << std::endl;
        current = _insnode(key, value);
        head = current;
        size += 1;
        return;
    }

    // Traverse list
    while (current != NULL)
    {

        // If you find a key that matches, update value and bool
        if (current->key == key)
        {
            //// std::cout << "(INSERT) Found duplicate, overwriting" << std::endl;
            current->value = value;
            hasWritten = true;
        }
        if (current != NULL)
        {
            previousNode = current;
        }
        current = current->next;
    }

    // If no values have been updated (new key), make a new node and append to end
    if (hasWritten == false)
    {
        //// std::cout << "(INSERT) Appending node" << std::endl;
        current = _insnode(key, value);
        previousNode->next = current;
        size += 1;
    }

    return;
}

// Get Value Function
template <typename K, typename V>
std::optional<V> hash_list<K, V>::get_value(K key) const
{
    node<K, V>* current = head;
    while (current != NULL)
    {
        // finds
        if (current->key == key)
        {
            // std::cout << "(GETVALUE) Value: " << current->value << std::endl;
            return current->value;
        }
        current = current->next;
    }
    // std::cout << "(GETVALUE) Did not Find" << std::endl;
    return {};
}
template <typename K, typename V>
node<K, V>* _insnode(K key, V value)
{
    // create node
    node<K, V>* newNode = new (node<K, V>);
    newNode->key = key;
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

template <typename K, typename V>
bool hash_list<K, V>::remove(K key)
{
    node<K, V>* current = head;
    node<K, V>* prev = head;

    if (head != NULL && (head->key == key))
    {
        node<K, V>* temp = head;
        head = head->next;
        delete temp;
        // std::cout << "(REMOVE) Did remove Head: "<< "Key: " << key  << std::endl;
        size -= 1;
        return true;
    }

    while (current != NULL)
    {
        if (current == NULL)
        {
            // DID NOT FIND TO REMOVE
            //  std::cout << "(REMOVE) Did Not Find: "<< "Key: " << key  << std::endl;
            return false;
        }

        if (current->key == key)
        {
            prev->next = current->next;
            // free current
            delete current;
            // std::cout << "(REMOVE) Did Find: "<< "Key: " << key  << std::endl;
            size -= 1;
            return true;
        }

        prev = current;
        current = current->next;
    }
    // std::cout << "(REMOVE) Did Not Find: "<< "Key: " << key  << std::endl;
    return false;
}

template <typename K, typename V>
size_t hash_list<K, V>::get_size() const
{
    // std::cout << size << std::endl;
    return size;
}

template <typename K, typename V>
hash_list<K, V>::~hash_list()
{
    node<K, V>* current;

    while (head != NULL)
    {
        current = head;
        // std::cout << current->key << std::endl;
        head = head->next;
        delete current;
    }

    //// std::cout << "(DESTRUCTOR)  Destruct Finished" << std::endl;
}

/** Dont modify this function for this lab. Leave it as is */
template <typename K, typename V>
void hash_list<K, V>::reset_iter() {
    if(head == NULL){
        iter_ptr = NULL;
    }
    iter_ptr = head;
    return;
}

/** Dont modify this function for this lab. Leave it as is */
/**
     * Moves the iterator to the next element. If the iterator points to the last element
     * of the list when this is called the iterator is set to NULL. If the iterator is NULL
     * when this function is called then this function does nothing
     */
    template <typename K, typename V>
void hash_list<K, V>::increment_iter() {
    if(iter_ptr == NULL){
        return;
    }
    if(iter_ptr->next == NULL){
        iter_ptr = NULL;
        return;
    }
    iter_ptr = iter_ptr->next;
}

/** Dont modify this function for this lab. Leave it as is */
template <typename K, typename V>
std::optional<std::pair<const K *, V *>> hash_list<K, V>::get_iter_value() {

    if(iter_ptr == NULL){
        return {};
    }

    if(iter_ptr == NULL)
    {
        return {};
    }
    K* ptrkey = &(iter_ptr->key);
    V* ptrvalue = &(iter_ptr->value);


    return std::make_pair(ptrkey, ptrvalue);
}

/** Dont modify this function for this lab. Leave it as is */
template <typename K, typename V>
bool hash_list<K, V>::iter_at_end() {
    if(iter_ptr == NULL){
        return true;
    }
    return false;
}
