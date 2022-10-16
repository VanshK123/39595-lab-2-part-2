#ifndef __SHARED_TESTING_CODE_H
#define __SHARED_TESTING_CODE_H

#include <string>

struct test_multipliers
{
    /** The multiplier for the default constructed map */
    size_t default_constructor_multiplier;

    /** The multiplier for the copy constructed map */
    size_t copy_constructor_multiplier;

    /** The multiplier for the assignment constructed map */
    size_t assignment_operator_multiplier;

    /** The multiplier for the self assignment constructed map */
    size_t self_assignment_multiplier;

    /** Constructs a default test multipliers struct with all values initialized to 1 */
    test_multipliers();
};

/** Holds the results of each test and the associated point values */
struct test_results
{

    /** The multiplier associated with this test */
    size_t multiplier;

    /** Flag and point values for passing basic tests */
    std::pair<bool, size_t> basic_tests;

    /** Flag and point values for passing basic iterator tests */
    std::pair<bool, size_t> iterator;

    /** Flag and point value for passing get bucket sizes */
    std::pair<bool, size_t> get_bucket_sizes;

    /** Flag and point value for passing get all keys */
    std::pair<bool, size_t> get_all_keys;

    /** Flag and point value for passing get all sorted keys */
    std::pair<bool, size_t> get_all_sorted_keys;

    /** Flag and point value for passing dynamic size */
    std::pair<bool, size_t> dynamic_size;

    /**
     * Construct a test_results object with all tests initalized to false and with
     * point values of 0
     */
    test_results();
};

std::pair<test_results, test_multipliers> parse_trace_file_points(std::ifstream &trace_file);

void write_test_results_to_file(test_results &default_results,
                                test_results &copy_constructed_results,
                                test_results &assignment_operator_results,
                                test_results &self_assignment_results,
                                bool write_copy_results,
                                bool write_assignment_results,
                                bool write_dynamic_size_sorted_keys,
                                std::ofstream &grading_file);

size_t calculate_score(test_results &results,
                       size_t multiplier,
                       bool write_dynamic_size_sorted_keys,
                       std::ofstream &grading_file);

#endif