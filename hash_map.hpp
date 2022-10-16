#include <bits/stdc++.h>
using namespace std;
#include "hash_map.h"
#include <cmath>

/*
// this is the hashing function
template <typename K>
size_t hashing_function(K key, size_t capacity)
{
    return (_hash(key) % (capacity));
}
*/

template <typename K, typename V>
hash_map<K, V>::hash_map(size_t capacity, float upper_load_factor, float lower_load_factor)
{
    _size = 0;
    _capacity = capacity;
    _head = new hash_list<K, V>[_capacity];
    for(size_t i = 0; i < _capacity; i++)
    {
        _head[i] = hash_list<K, V>();
    }
}

template <typename K, typename V>
hash_map<K, V>::hash_map(const hash_map &other)
{
    // create empty hashmap
    _size = 0;
    _capacity = other._capacity;
    _head = new hash_list<K,V>[_capacity];
    for(size_t i = 0; i < _capacity; i++)
    {
        _head[i] = other._head[i];
    }
    _size = other._size;
    return;
}

template <typename K, typename V>
hash_map<K, V> &hash_map<K, V>::operator=(const hash_map<K, V> &other)
{
    if(this == &other){
        return *this;
    }
    _capacity = other._capacity;
    _size = other._size;
    for(size_t i = 0; i < _capacity; i++)
    {
        _head[i] = other._head[i];
    }
    return *this;
}

template <typename K, typename V>
void hash_map<K, V>::insert(K key, V value)
{
    size_t i = _hash(key) % (_capacity); // get index
    _size -= _head[i].get_size();
    _head[i].insert(key, value);
    _size += _head[i].get_size();
}

template <typename K, typename V>
std::optional<V> hash_map<K, V>::get_value(K key) const
{
    size_t i = _hash(key) % (_capacity); // get index
    return _head[i].get_value(key);
}

template <typename K, typename V>
bool hash_map<K, V>::remove(K key)
{
    size_t i = _hash(key) % (_capacity);
    _size -= _head[i].get_size();
    bool isSuccessful = _head[i].remove(key);
    _size += _head[i].get_size();
    return isSuccessful;
}

template <typename K, typename V>
size_t hash_map<K, V>::get_size() const
{
    return _size;
}

template <typename K, typename V>
size_t hash_map<K, V>::get_capacity() const
{
    return _capacity;
}

template <typename K, typename V>
void hash_map<K, V>::get_all_keys(K *keys)
{   int count = 0;
    for (size_t i = 0; i < _capacity; i++)
    {   
        _head[i].reset_iter();
        while (! _head[i].iter_at_end())
        {   
            keys[count] = * _head[i].get_iter_value().value().first;
            _head[i].increment_iter();
            count++; 
        }
        
    }
    
}

template <typename K, typename V>
void hash_map<K, V>::get_bucket_sizes(size_t * buckets)
{
    for (size_t i = 0; i < _capacity; i++)
    {
        buckets[i] = _head[i].get_size();
    }
    return;
}
template <typename K, typename V>
hash_map<K, V>::~hash_map()
{
    delete[] _head ;
}
template <typename K, typename V>
void hash_map<K,V>::get_all_sorted_keys(K *keys){

}



