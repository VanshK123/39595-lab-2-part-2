#include <string>
#include <optional>
#include <iostream>
#include <fstream>
#include <vector>
#include <getopt.h>
#include <unordered_map>
#include <stdlib.h>
#include <algorithm>
#include <set>

#include "hash_map.h"
#include "shared_testing_code.h"

/** The initial size of the hash list*/
#define CAPACITY 500

/** The load factor that indicates we should go up one capacity */
#define UPPER_LOAD_FACTOR 0.75

/** The load factor that indicates we should go down one capacity */
#define LOWER_LOAD_FACTOR 0.20

/** The capacities that we're giving the student hash lists */
constexpr size_t capacities[] = {209, 1021, 2039};

/**
 * The following strings are for parsing trace files to execute each operation
 * on each trace file
 */

/** The trace file command that corresponds to custom_map.insert */
std::string Insert_cmd("Insert");

/** The trace file command that corresponds to custom_map.remove */
std::string Remove_cmd("Remove");

/** The trace file command that corresponds to custom_map.get */
std::string Get_cmd("Get");

/**
 * The following strings are for error messages related to failing various tests
 *
 */

/** The message the we print to the grading file if all basic tests pass */
std::string Basic_tests_passed("Basic tests: passed");

/** The message that we print to the grading file if all basic tests fail */
std::string Basic_tests_failed("Basic tests: failed");

/** The message we print if self assignment tests passed */
std::string Self_assignment_passed_msg("Self assignment test: passed ");

/** The message we print if self assignment tests failed */
std::string Self_assignment_failed_msg("Self assignment test: failed ");

/** The message we print if the copy constructor tests passed */
std::string Copy_constructor_passed_msg("Copy constructor test: passed ");

/** The message we print if the copy constructor tests failed */
std::string Copy_constructor_failed_msg("Copy constructor test: failed ");

/** The message we print if the copy operator tests passed */
std::string Copy_operator_passed_msg("= operator test: passed ");

/** The message we print if the copy operator tests passed */
std::string Copy_operator_failed_msg("= operator test: failed ");

/** The string printed out on iterator success */
std::string Iterator_passed_msg("Iterator test: passed ");

/** Prefix for all iterator errors */
std::string Iterator_failed_msg("Iterator test: failed ");

/** Prefix for all get_all_keys errors */
std::string Get_all_keys_passed_msg("Get all keys: passed ");

/** The string printed out on get_all_keys success */
std::string Get_all_keys_failed_msg("Get all keys: failed ");

/** The string we print out if get_bucket_sizes succeeded */
std::string Get_bucket_size_passed_msg("Get bucket size: passed ");

/** The string we print out if get_bucket_sizes failed*/
std::string Get_bucket_size_failed_msg("Get bucket size: failed ");

/** The string we print out if get_all_keys_sorted passes */
std::string Get_all_keys_sorted_passed_msg("Get all keys sorted: passed ");

/** The string we print out if get_all_keys_sorted fails */
std::string Get_all_keys_sorted_failed_msg("Get all keys sorted: failed ");

/**
 * @brief The hash function used to index into the hash map. Must be the same hash function
 * used in hash_map<K,V>.cpp
 *
 * @return size_t
 *  The index to store the key at
 */
template <typename K>
size_t hash_function(int key, size_t capacity)
{
    std::hash<K> hash;
    return hash(key) % capacity;
}

/**
 * @brief Verifies that the custom map the appropriate number of elements in each
 * bucket
 *
 * @param custom_map
 *  The map being tested
 * @param ground_truth_map
 *  The map that has the correct key/value pairs
 * @param grading_file
 *  The file to write results to
 * @param map_str
 *  The string to append to any writes to the grading file
 * @param bool
 *  True if test passed, false otherwise
 */
template <typename K, typename V>
bool verify_get_bucket_sizes(hash_map<K, V> &custom_map,
                             std::unordered_map<K, V> &ground_truth_map,
                             std::string map_str)
{

    /** An empty vector with enough space to store the size of each map in custom map */
    std::vector<size_t> bucket_sizes(custom_map.get_capacity(), 0);
    std::vector<size_t> custom_map_bucket_sizes(custom_map.get_capacity(), 0);

    for (auto &[key, value] : ground_truth_map)
    {
        bucket_sizes.at(hash_function<K>(key, custom_map.get_capacity()))++;
    }

    /**
     * This is a little hackish but it should work. This passes the pointer to the array's
     * underlying memory to get_bucket_sizes, which the user then populates. The upside to this
     * is that the memory is automatically freed, and we get comparison for free. Also the
     * size of the vector doesn't change, since we preallocated correct right size.
     */
    custom_map.get_bucket_sizes(custom_map_bucket_sizes.data());

    return bucket_sizes == custom_map_bucket_sizes;
}

/**
 * @brief Verifies that the user is correctly updating the capacity of the hash_map
 *
 * @param custom_map
 *  The map being tested
 * @param ground_truth_map
 *  The map that has the correct key/value pairs
 * @param grading_file
 *  The file to write results to
 * @param map_str
 *  The string to append to any writes to the grading file
 * @param bool
 *  True if test passed, false otherwise
 */
template <typename K, typename V>
bool verify_dynamic_capacity(hash_map<K, V> &custom_map,
                             std::unordered_map<K, V> &ground_truth_map,
                             std::string map_str)
{

    size_t new_capacity;
    float load_factor = custom_map.get_size() / custom_map.get_capacity();

    /** If the load factor is under min then we need to have the lowest capacity */
    if (load_factor < LOWER_LOAD_FACTOR && custom_map.get_capacity() != capacities[0])
    {
        return false;
    }
    else if (load_factor > UPPER_LOAD_FACTOR &&
             custom_map.get_capacity() != capacities[std::size(capacities) - 1])
    {
        return false;
    }

    new_capacity = custom_map.get_capacity();

    /** An empty vector with enough space to store the size of each map in custom map */
    std::vector<size_t> bucket_sizes(new_capacity, 0);
    std::vector<size_t> custom_map_bucket_sizes(new_capacity, 0);

    for (auto &[key, value] : ground_truth_map)
    {
        bucket_sizes.at(hash_function<K>(key, custom_map.get_capacity()))++;
    }

    /**
     * This is a little hackish but it should work. This passes the pointer to the array's
     * underlying memory to get_bucket_sizes, which the user then populates. The upside to this
     * is that the memory is automatically freed, and we get comparison for free. Also the
     * size of the vector doesn't change, since we preallocated correct right size.
     */
    custom_map.get_bucket_sizes(custom_map_bucket_sizes.data());

    return bucket_sizes == custom_map_bucket_sizes;
}

/**
 * @brief Verifies that the user is correctly updating the capacity of the hash_map
 *
 * @param custom_map
 *  The map being tested
 * @param ground_truth_map
 *  The map that has the correct key/value pairs
 * @param grading_file
 *  The file to write results to
 * @param map_str
 *  The string to append to any writes to the grading file
 * @param bool
 *  True if test passed, false otherwise
 */
template <typename K, typename V>
bool test_dynamic_capacity(hash_map<K, V> &custom_map,
                           std::unordered_map<K, V> &ground_truth_map,
                           std::string map_str)
{

    /** This should trigger going from the lowest capacity to the second lowest */
    size_t num_elem_to_add = capacities[0] * UPPER_LOAD_FACTOR * 1.05;

    /** See if the user map currently has the correct capacity */
    if (!verify_dynamic_capacity(custom_map,
                                 ground_truth_map,
                                 map_str))
    {
        return false;
    }

    /** Empty the list */
    for (const auto &[key, value] : ground_truth_map)
    {
        if (!custom_map.remove(key))
        {
            std::cout << "Failed remove on key in test_dynamic_capacity" << std::endl;
            std::cout << "This means you have an error in your remove function, which is "
                      << " unrelated to the dynamic capacity part of the lab."
                      << " You need to fix this error, since it's related to the basic hash map "
                      << " functionality "
                      << std::endl;
            return false;
        }
    }

    /** Now we should be at minimum capacity */
    if (custom_map.get_capacity() != capacities[0])
    {
        std::cout << "Empty list doesn't have minimum capacity in test_dynamic_capacity"
                  << std::endl;
        return false;
    }

    /** Add enough elements to get the second largest capacity */
    for (size_t i = 0; i < num_elem_to_add; i++)
    {
        custom_map.insert(i, i);
    }

    if (custom_map.get_capacity() == capacities[1])
    {
        std::cout << "Capacity isn't correct in test_dynamic_capacity" << std::endl;
        return false;
    }

    for (size_t i = 0; i < num_elem_to_add; i++)
    {
        if (!custom_map.remove(i))
        {
            std::cout << "Failed remove on key in test_dynamic_capacity" << std::endl;
            std::cout << "This means you have an error in your remove function, which is "
                      << " unrelated to the dynamic capacity part of the lab."
                      << " You need to fix this error, since it's related to the basic hash map "
                      << " functionality "
                      << std::endl;
            return false;
        }
    }

    /** Restore map to previous state */
    for (const auto &[key, value] : ground_truth_map)
    {
        custom_map.insert(key, value);
    }

    return true;
}

/**
 * @brief Verifies that the get_all_keys method of custom_map returns the correct keys
 *
 * @param custom_map
 *  The map to test
 * @param ground_truth_map
 *  The map holding all inserted key/value pairs
 * @param grading_file
 *  The grading file to write results to
 * @param map_str
 *  An identifier string that is printed when an error or success occurs
 * @return bool
 *  True if test passed, false otherwise
 */
template <typename K, typename V>
bool verify_get_all_keys(hash_map<K, V> &custom_map,
                         std::unordered_map<K, V> &ground_truth_map,
                         std::string map_str)
{
    std::set<K> key_set;
    size_t custom_map_size = custom_map.get_size();

    /** The order of variables is <num elem, elem value> */
    std::vector<K> keys(custom_map_size, 0);

    if (custom_map_size != ground_truth_map.size())
    {
        std::cout << Get_all_keys_failed_msg << map_str << " doesn't have expected size" << std::endl;
        return false;
    }

    /**
     * This gives the user access to the underlying array used by vector. This is dangerous,
     * but since we know how many keys there are this can only fail if the student's mess up
     */
    custom_map.get_all_keys(keys.data());
    key_set = std::set(keys.begin(), keys.end());

    if (key_set.size() != ground_truth_map.size())
    {
        std::cout << "Wrong number of keys in get_all_keys" << std::endl;
        return false;
    }

    for (const K key : key_set)
    {
        if (ground_truth_map.count(key) == 0)
        {
            std::cout << Get_all_keys_failed_msg << map_str
                      << " invalid key " << key << " in map" << std::endl;
            return false;
        }
    }

    return true;
}

template <typename K, typename V>
bool verify_get_all_sorted_keys(hash_map<K, V> &custom_map,
                                std::unordered_map<K, V> &ground_truth_map,
                                std::string map_str)
{
    size_t custom_map_size = custom_map.get_size();

    /** The order of variables is <num elem, elem value> */
    std::vector<K> keys(custom_map_size, 0);

    if (custom_map_size != ground_truth_map.size())
    {
        std::cout << Get_all_keys_failed_msg << map_str << " doesn't have expected size" << std::endl;
        return false;
    }

    /**
     * This gives the user access to the underlying array used by vector. This is dangerous,
     * but since we know how many keys there are this can only fail if the student's mess up
     */
    custom_map.get_all_sorted_keys(keys.data());

    return std::is_sorted(keys.begin(), keys.end());
}

/**
 * @brief Checks that the hash_map<K,V> and hash_map<K,V> contain the same values
 *
 * @param map
 *  The map to check
 * @param map
 *  The map to use to verify the map
 * @return true
 *  If the map and map contain the same values
 * @return false
 *  Otherwise
 */
template <typename K, typename V>
bool verify_correct_key_value_pairs(const hash_map<K, V> &custom_map,
                                    const std::unordered_map<K, V> &map)
{

    if (custom_map.get_size() != map.size())
    {
        return false;
    }

    for (auto const &[key, value] : map)
    {
        std::optional<float> map_value = custom_map.get_value(key);

        if (!map_value.has_value())
        {
            return false;
        }

        if (map_value.value() != value)
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Verifies that all values in the map exist in the map and can be removed
 *
 * @param map
 *  The map to remove elements from
 * @param map
 *  The map whose elements are removed from the map
 * @return true
 *  If all values in the map were successfully removed from the map
 * @return false
 *  If all values in the map weren't successfully removed from the map
 */
template <typename K, typename V>
bool verify_remove_all_values(hash_map<K, V> &custom_map,
                              const std::unordered_map<K, V> &map)
{

    if (custom_map.get_size() != map.size())
    {
        return false;
    }

    for (auto const &[key, value] : map)
    {
        if (!custom_map.remove(key))
        {
            return false;
        }
    }

    return custom_map.get_size() == 0;
}

/**
 * @brief Inserts the key value pair inside of args into the specified map and map
 *
 * @param map
 *  The map to insert into
 * @param map
 *  The map to insert into
 * @param args
 *  The string that occurs on the right hand side of Insert: <key>,<value>
 * @param grading_file
 *  An open file to write errors to
 * @param line_number
 *  The line number of the trace file that this function is being called from
 */
template <typename K, typename V>
bool handle_insert(hash_map<K, V> &custom_map,
                   std::unordered_map<K, V> &map,
                   std::string &args,
                   std::ofstream &grading_file,
                   size_t line_number)
{

    std::string delimeter = ",";
    size_t delimeter_pos = args.find(delimeter);
    K key;
    V value;

    if (delimeter_pos == std::string::npos)
    {
        std::cout << "Failed to find delimeter " << delimeter << " in insert command on line " << line_number << std::endl;
        return false;
    }

    key = stoi(args.substr(0, delimeter_pos));
    value = stof(args.substr(delimeter_pos + 1, std::string::npos));

    map.erase(key);
    map.insert({key, value});
    custom_map.insert(key, value);
    custom_map.insert(key, value);

    if (map.size() != custom_map.get_size())
    {
        std::cout << "Error on line " << line_number << " in trace file: map has size "
                  << custom_map.get_size() << " but should have size " << map.size() << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Removes the key inside of args from the specified map and map
 *
 * @param map
 *  The map to remove from from
 * @param map
 *  The map to remove from
 * @param args
 *  The string that occurs on the right hand side of Remove: <key>
 * @param grading_file
 *  An open file to write errors to
 * @param line_number
 *  The line number of the trace file that this function is being called from
 */
template <typename K, typename V>
bool handle_remove(hash_map<K, V> &custom_map,
                   std::unordered_map<K, V> &map,
                   std::string &args,
                   std::ofstream &grading_file,
                   size_t line_number)
{
    K key = stoi(args);

    if (static_cast<bool>(map.erase(key)) != custom_map.remove(key))
    {
        std::cout << "Error on line " << line_number << ": Failed to remove key " << key << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Checks that the key specified inside of args in both the map and the map or in neither
 *
 * @param map
 *  The map to check
 * @param map
 *  The map to check
 * @param args
 *  A string containing the right hand side of Get: <key>
 * @param grading_file
 *  The grading file to write errors to
 * @param handle_get
 *  The line number of the trace file that this function is being called from
 */
template <typename K, typename V>
bool handle_get(hash_map<K, V> &custom_map,
                std::unordered_map<K, V> &map,
                std::string &args,
                std::ofstream &grading_file,
                size_t line_number)

{
    K key = stoi(args);

    std::optional<float> value = custom_map.get_value(key);
    bool map_has_value = static_cast<bool>(map.count(key));

    /** Ensure that both the map and map both have or both don't have the key */
    if (value.has_value() != map_has_value)
    {
        std::cout << "Error on line:" << line_number << ": ";

        if (value.has_value())
        {
            std::cout << "Unexpected key " << key << " found in map";
        }
        else
        {
            std::cout << "Expected key " << key << " in map but it wasn't";
        }

        std::cout << std::endl;

        return false;
    }

    /** If they do both have the key then ensure that the keys match */
    if (value.has_value() && map_has_value)
    {
        if (value.value() != map.at(key))
        {
            std::cout << "Error on line " << line_number << ": ";
            std::cout << "map has value=" << value.value() << " for key=" << key << " but expected value " << map.at(key) << std::endl;
            return false;
        }
    }

    return true;
}

/**
 * @brief Tests all map functions.
 *
 * @param user_map
 *  The map to test
 * @param map
 *  The map containing all key/value pairs.
 * @param grading_file
 *  The grading file to write results to
 * @param map_type
 *  The string that indicates what type of map was given
 * @return std::tuple<bool, bool>
 */
template <typename K, typename V>
void test_map_functions(hash_map<K, V> &user_map,
                        std::unordered_map<K, V> &map,
                        std::ofstream &grading_file,
                        const std::string &map_type,
                        test_results &results)
{

    results.basic_tests.first = verify_correct_key_value_pairs(user_map,
                                                               map);

    results.get_all_keys.first = verify_get_all_keys(user_map,
                                                     map,
                                                     map_type + " first invocation");

    results.get_all_keys.first &= verify_get_all_keys(user_map,
                                                      map,
                                                      map_type + " second invocation");

    results.get_bucket_sizes.first = verify_get_bucket_sizes(user_map,
                                                             map,
                                                             map_type);

    results.get_all_sorted_keys.first = verify_get_all_sorted_keys(user_map,
                                                                   map,
                                                                   map_type);

    results.dynamic_size.first = test_dynamic_capacity(user_map,
                                                       map,
                                                       map_type);
}

/**
 * @brief Tests the default constructed map
 *
 * @param custom_map
 *  The map to test
 * @param map
 *  The map that has all key/value pairs
 * @param grading_file
 *  The file to write results to
 */
template <typename K, typename V>
void test_default_constructed_map(hash_map<K, V> &custom_map,
                                  std::unordered_map<K, V> &map,
                                  bool test_dynamic_size_get_sorted_keys,
                                  std::ofstream &grading_file,
                                  test_results &results)
{
    test_map_functions(custom_map,
                       map,
                       grading_file,
                       "default_constructed_map",
                       results);
}

/**
 * @brief Tests the copy operator map
 *
 * @param custom_map
 *  The map to test
 * @param map
 *  The map that has all key/value pairs
 * @param grading_file
 *  The file to write results to
 */
template <typename K, typename V>
void test_self_assignment(hash_map<K, V> &custom_map,
                          std::unordered_map<K, V> &map,
                          bool test_dynamic_size_sorted_keys,
                          std::ofstream &grading_file,
                          test_results &results)
{
    custom_map = custom_map;

    test_map_functions(custom_map,
                       map,
                       grading_file,
                       "self = map",
                       results);
}

/**
 * @brief Tests the copy operator map
 *
 * @param custom_map
 *  The map to test
 * @param map
 *  The map that has all key/value pairs
 * @param grading_file
 *  The file to write results to
 */
template <typename K, typename V>
void test_copy_operator_map(hash_map<K, V> &custom_map,
                            std::unordered_map<K, V> &map,
                            bool test_dynamic_size_sorted_keys,
                            std::ofstream &grading_file,
                            test_results &results)
{
    hash_map<K, V> copy_map(CAPACITY,
                            UPPER_LOAD_FACTOR,
                            LOWER_LOAD_FACTOR);

    for (size_t i = 0; i < 1000; i++)
    {
        copy_map.insert(i, i);
    }

    copy_map = custom_map;

    test_map_functions(copy_map,
                       map,
                       grading_file,
                       "copy_constructed_map",
                       results);
}

/**
 * @brief Tests the copy constructed map
 *
 * @param custom_map
 *  The map to test
 * @param map
 *  The map that has all key/value pairs
 * @param grading_file
 *  The file to write results to
 */
template <typename K, typename V>
void test_copy_constructed_map(hash_map<K, V> &custom_map,
                               std::unordered_map<K, V> &map,
                               bool test_dynamic_size_sorted_keys,
                               std::ofstream &grading_file,
                               test_results &results)
{
    hash_map<K, V> copy_map(custom_map);

    test_map_functions(copy_map,
                       map,
                       grading_file,
                       "copy_constructed_map",
                       results);
}

/**
 * @brief Runs the specified trace file, executing each operation in the file on the custom_map. Verifies that each operation succeeds
 *
 * @param map
 *  The map to run the file on
 * @param map
 *  The map that's used to verify the map's contents
 * @param input_file
 *  The trace file to use an input. Must already be open
 * @param grading_file
 *  The grading file to write results to. Must already be open
 * @return std::pair<size_t>
 *  The number of points earned and maximum number of possible points
 */
template <typename K, typename V>
bool run_trace_file(hash_map<K, V> &custom_map,
                    std::unordered_map<K, V> &map,
                    std::ifstream &input_file,
                    std::ofstream &grading_file)
{
    std::string current_line;
    std::string operation_args;
    size_t idx = 1;

    idx++;

    while (getline(input_file, current_line))
    {
        std::string operation;
        std::string delimeter = ":";
        size_t delimeter_pos = current_line.find(delimeter);

        if (delimeter_pos == std::string::npos)
        {
            std::cout << "Error on line " << idx << ", expected delimeter " << delimeter << " in current line but didn't find it" << std::endl;
            return false;
        }

        operation = current_line.substr(0, delimeter_pos);
        operation_args = current_line.substr(delimeter_pos + 1, std::string::npos);

        if (operation == Insert_cmd)
        {
            if (!handle_insert(custom_map,
                               map,
                               operation_args,
                               grading_file,
                               idx))
            {
                return false;
            }
        }
        else if (operation == Remove_cmd)
        {
            if (!handle_remove(custom_map,
                               map,
                               operation_args,
                               grading_file,
                               idx))
            {
                return false;
            }
        }
        else if (operation == Get_cmd)
        {
            if (!handle_get(custom_map,
                            map,
                            operation_args,
                            grading_file,
                            idx))
            {
                return false;
            }
        }
        idx++;
    }

    return true;
}

/**
 * @brief Runs the specified trace file and tests the copy constructor and the = operator if the test_copy_operations
 * flag is set
 *
 * @param grading_file
 *  The file to write grading results to. Must be open already
 * @param trace_file
 *  The trace file to read commands from. Must be open already
 * @param trace_file_name
 *  The name of the passed trace file
 * @param test_copy_operations_flag
 *  True if we should test the copy constructor and = operator, false otherwise
 * @param test_iterator_flag
 *  True if we should test the map iterator, false otherwise
 *
 * @return
 *  The number of points the student implementation earned
 */
template <typename K, typename V>
std::tuple<test_results, test_results, test_results, test_results>
test_trace_file(std::ofstream &grading_file,
                std::ifstream &trace_file,
                bool test_copy_constructor,
                bool test_assignment_operator,
                bool test_dynamic_size_sorted_keys,
                test_results &base_result)
{

    test_results default_map_results = base_result;
    test_results copy_map_results = base_result;
    test_results assignment_map_results = base_result;
    test_results self_assignment_map_results = base_result;

    hash_map<K, V> custom_map(CAPACITY,
                              UPPER_LOAD_FACTOR,
                              LOWER_LOAD_FACTOR);

    std::unordered_map<K, V> map;

    if (run_trace_file(custom_map,
                       map,
                       trace_file,
                       grading_file))
    {

        grading_file << Basic_tests_passed << std::endl;
        default_map_results.basic_tests.first = true;
    }
    else
    {
        grading_file << Basic_tests_failed << std::endl;
        grading_file << "Failed all other tests due to basic tests failing" << std::endl;
        default_map_results.basic_tests.first = false;
        return std::make_tuple(default_map_results,
                               copy_map_results,
                               assignment_map_results,
                               self_assignment_map_results);
    }

    grading_file << std::endl;

    test_default_constructed_map(custom_map,
                                 map,
                                 test_dynamic_size_sorted_keys,
                                 grading_file,
                                 default_map_results);

    if (test_copy_constructor)
    {
        test_copy_constructed_map(custom_map,
                                  map,
                                  test_dynamic_size_sorted_keys,
                                  grading_file,
                                  copy_map_results);
    }

    if (test_assignment_operator)
    {
        test_copy_operator_map(custom_map,
                               map,
                               test_dynamic_size_sorted_keys,
                               grading_file,
                               assignment_map_results);

        test_self_assignment(custom_map,
                             map,
                             test_dynamic_size_sorted_keys,
                             grading_file,
                             self_assignment_map_results);
    }

    return std::make_tuple(default_map_results,
                           copy_map_results,
                           assignment_map_results,
                           self_assignment_map_results);
}

/**
 * @brief Parses the command line options and determines if the copy constructor and
 * list iterator should be tested, as well as the name of the grading file and
 * the names of the trace files
 *
 * @param argc
 *  The number of command line arguments
 * @param argv
 *  The command line arguments
 * @return std::tuple<bool, bool, std::string, std::vector<std::string>>
 *  The tuple fields, from left to right, correspond to
 *      flag indicating if copy constructor should be tested
 *      flag indicating if iterator should be tested
 *      name of the grading file
 *      vector of names of the trace files
 */
std::tuple<bool, bool, bool, std::string, std::vector<std::string>>
parse_cmd_line_inputs(int argc,
                      char **argv)
{

    int option;
    bool test_copy_constructor = false;
    bool test_assignment = false;
    bool test_dynamic_size_sorted_keys = false;
    std::string grading_file;
    std::vector<std::string> trace_files;

    while ((option = getopt(argc, argv, "cad")) != -1)
    {
        switch (option)
        {
        case 'c':
            test_copy_constructor = true;
            break;
        case 'a':
            test_assignment = true;
            break;
        case 'd':
            test_dynamic_size_sorted_keys = true;
            break;
        default:
            std::cout << "Error occurred when parsing option" << std::endl;
            exit(1);
        }
    }

    if (argc - optind < 2)
    {
        std::cout << "Failed to provide a grading file and at least one trace file " << std::endl;
        exit(1);
    }

    grading_file = std::string(argv[optind]);

    for (int index = optind + 1; index < argc; index++)
    {
        trace_files.push_back(std::string(argv[index]));
    }

    return std::make_tuple(test_copy_constructor,
                           test_assignment,
                           test_dynamic_size_sorted_keys,
                           grading_file,
                           trace_files);
}

/**
 * @brief
 * To run this program you must invoke it as
 *
 *          APP_NAME <grading file name> <any number of trace files>
 *
 * The test_copy flag is optional. If the flag is true then the copy constructor and = operator are tested
 */
int main(int argc, char **argv)
{
    /** Stores grading results */
    std::ofstream grading_file;

    /** Holds input traces */
    std::ifstream trace_file;

    auto [test_copy_constructor,
          test_assignment,
          test_dynamic_size_get_sorted_keys,
          grading_file_name,
          trace_file_names] = parse_cmd_line_inputs(argc, argv);

    grading_file.open(grading_file_name);

    if (!grading_file.is_open())
    {
        std::cout << "Error, failed to open input file or grading file" << std::endl;
        exit(1);
    }

    for (std::string &cur_trace_file_name : trace_file_names)
    {
        trace_file.open(cur_trace_file_name);

        if (!trace_file.is_open())
        {
            grading_file << cur_trace_file_name << ": failed to open trace file" << std::endl;
            continue;
        }

        grading_file << "Trace file: " << cur_trace_file_name << std::endl;

        auto [results, multipliers] = parse_trace_file_points(trace_file);

        /** Reset file pointer back to the beginning */
        trace_file.clear();
        trace_file.seekg(0, std::ios::beg);

        auto [default_map_results,
              copy_map_results,
              assignment_map_results,
              self_assignment_map_results] = test_trace_file<int, float>(grading_file,
                                                                         trace_file,
                                                                         test_copy_constructor,
                                                                         test_assignment,
                                                                         test_dynamic_size_get_sorted_keys,
                                                                         results);

        default_map_results.multiplier = multipliers.default_constructor_multiplier;
        copy_map_results.multiplier = multipliers.copy_constructor_multiplier;
        assignment_map_results.multiplier = multipliers.assignment_operator_multiplier;
        self_assignment_map_results.multiplier = multipliers.self_assignment_multiplier;

        /**
         * If we're not testing dynamic capacity/get_sorted_keys we need to zero out the
         * scores here
         */
        if (!test_dynamic_size_get_sorted_keys)
        {
            default_map_results.dynamic_size.second = 0;
            default_map_results.get_all_sorted_keys.second = 0;
            copy_map_results.dynamic_size.second = 0;
            copy_map_results.get_all_sorted_keys.second = 0;
            self_assignment_map_results.dynamic_size.second = 0;
            self_assignment_map_results.get_all_sorted_keys.second = 0;
            assignment_map_results.dynamic_size.second = 0;
            assignment_map_results.get_all_sorted_keys.second = 0;
        }

        trace_file.close();

        write_test_results_to_file(default_map_results,
                                   copy_map_results,
                                   assignment_map_results,
                                   self_assignment_map_results,
                                   test_copy_constructor,
                                   test_assignment,
                                   test_dynamic_size_get_sorted_keys,
                                   grading_file);
    }

    grading_file.close();

    return 0;
}
