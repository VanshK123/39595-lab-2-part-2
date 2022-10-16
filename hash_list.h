#ifndef HASH_LIST_H
#define HASH_LIST_H

#include <optional>
#include <stddef.h>
#include <stdlib.h>

template <typename K, typename V>
struct node
{
    /** The key the node is storing */
    K key;

    /** the value the node is storing */
    V value;

    /** a pointer to the next node */
    node *next;
};

template <typename K, typename V>
class hash_list
{

public:
    /** Create empty list. Should set head to null and size to 0 */
    hash_list();

    /** Copy constructor for hash_list */
    hash_list(const hash_list &other);

    /** Copy assignment operator for hash_list */
    hash_list &operator=(const hash_list &other);

    /**
     * Insert a node with the corresponding key value pair into the list.
     * If a node with the associated key already exists, update that node with the
     * new value instead of adding a new node to the list.
     */
    void insert(K key, V value);

    /**
     * Return an optional containing the value associated with the specified key. If the key isn't in
     * the list return an empty optional.
     */
    std::optional<V> get_value(K key) const;

    /**
     * Remove the node containing the specified key from the list and return true.
     * If the key isn't in the list return false.
     */
    bool remove(K key);

    /** Return the number of nodes in the list */
    size_t get_size() const;

    /** Free all memory associated with the nodes. This must not free the nodes recursively */
    ~hash_list();

    /**
     * Resets the iterator back to point to the first element in the list. If the list is
     * empty then the iterator is set to NULL.
     */
    void reset_iter();

    /**
     * Moves the iterator to the next element. If the iterator points to the last element
     * of the list when this is called the iterator is set to NULL. If the iterator is NULL
     * when this function is called then this function does nothing
     */
    void increment_iter();

    /**
     * Return an optional that contains a pointer to the key and a pointer to the value
     * of the node pointed to by the iterator. If the iterator is NULL this returns
     * an empty optional
     */
    std::optional<std::pair<const K *, V *>> get_iter_value();

    /** Returns true if the iterator is NULL */
    bool iter_at_end();

private:
    /** The number of nodes in the list */
    size_t size;

    /** A pointer to the first node in the list */
    node<K, V> *head;

    /** The node that the iterator is currently pointing to */
    node<K,V> *iter_ptr;
};

/**
 * This weird looking line of code allows us to define our template functions in a
 * separate file without running into linking errors. See this FAQ for why
 *
 * https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl-export-keyword
 *
 */
#include "hash_list.hpp"

#endif
