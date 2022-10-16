#include <fstream>
#include <string>
#include <iostream>

#include "shared_testing_code.h"

/** Determines how many points the default constructor is worth */
std::string default_constructor_str("default_constructor");

/** Determines how many points the copy constructor is worth */
std::string copy_constructor_str("copy_constructor");

/** Determines how many points the = operator is worth */
std::string assignment_str("=_operator");

/** Determines how many points the self = test is worth */
std::string self_assignment_str("self_=");

/** Determines how many points basic tests are worth */
std::string basic_test_str("basic_tests");

/** Determines how many points the basic iterator is worth */
std::string iterator_str("iterator");

/** Determines how many points the get bucket sizes function is worth */
std::string get_bucket_sizes("get_bucket_sizes");

/** Determines how many points the get all keys function is worth */
std::string get_all_keys_str("get_all_keys");

/** Determines how many points the get all keys in sorted order is worth */
std::string get_all_keys_sorted_str("get_all_keys_sorted");

/** Determines how many points the dynamic size part of the map is worth */
std::string dynamic_size_str("dynamic_size");

test_multipliers::test_multipliers()
{
    default_constructor_multiplier = 1;
    copy_constructor_multiplier = 1;
    assignment_operator_multiplier = 1;
    self_assignment_multiplier = 1;
}

test_results::test_results()
{
    multiplier = 1;
    basic_tests = {false, 0};
    iterator = {false, 0};
    get_bucket_sizes = {false, 0};
    get_all_keys = {false, 0};
    get_all_sorted_keys = {false, 0};
    dynamic_size = {false, 0};
}

std::pair<test_results, test_multipliers>
parse_trace_file_points(std::ifstream &trace_file)
{
    test_results results;
    test_multipliers multipliers;
    std::string current_line;
    std::string operation;
    std::string operation_args;
    size_t delimeter_pos;
    size_t idx = 1;
    std::string delimeter = ":";

    while (getline(trace_file, current_line))
    {

        delimeter_pos = current_line.find(delimeter);

        if (delimeter_pos == std::string::npos)
        {
            std::cout << "Error on line " << idx << ", expected delimeter " << delimeter
                      << " in current line but didn't find it" << std::endl;
        }

        operation = current_line.substr(0, delimeter_pos);
        operation_args = current_line.substr(delimeter_pos + 1, std::string::npos);

        if (operation == default_constructor_str)
        {
            multipliers.default_constructor_multiplier = stoi(operation_args);
        }
        else if (operation == copy_constructor_str)
        {
            multipliers.copy_constructor_multiplier = stoi(operation_args);
        }
        else if (operation == assignment_str)
        {
            multipliers.assignment_operator_multiplier = stoi(operation_args);
        }
        else if (operation == self_assignment_str)
        {
            multipliers.self_assignment_multiplier = stoi(operation_args);
        }
        else if (operation == basic_test_str)
        {
            results.basic_tests.second = stoi(operation_args);
        }
        else if (operation == iterator_str)
        {
            results.iterator.second = stoi(operation_args);
        }
        else if (operation == get_bucket_sizes)
        {
            results.get_bucket_sizes.second = stoi(operation_args);
        }
        else if (operation == get_all_keys_str)
        {
            results.get_all_keys.second = stoi(operation_args);
        }
        else if (operation == get_all_keys_sorted_str)
        {
            results.get_all_sorted_keys.second = stoi(operation_args);
        }
        else if (operation == dynamic_size_str)
        {
            results.dynamic_size.second = stoi(operation_args);
        }
        /** We've reached the end of the scores so we can return the scores to the caller */
        else
        {
            return {results, multipliers};
        }
    }

    return {results, multipliers};
}

void write_test_results_to_file(test_results &default_results,
                                test_results &copy_constructed_results,
                                test_results &assignment_operator_results,
                                test_results &self_assignment_results,
                                bool write_copy_results,
                                bool write_assignment_results,
                                bool write_dynamic_size_sorted_keys,
                                std::ofstream &grading_file)
{
    size_t total_score = 0;

    grading_file << "Default constructor tests " << std::endl;
    total_score += calculate_score(default_results,
                                   default_results.multiplier,
                                   write_dynamic_size_sorted_keys,
                                   grading_file);

    if (write_copy_results)
    {
        grading_file << "Copy constructor tests " << std::endl;
        total_score += calculate_score(copy_constructed_results,
                                       copy_constructed_results.multiplier,
                                       write_dynamic_size_sorted_keys,
                                       grading_file);
    }

    if (write_assignment_results)
    {
        grading_file << "Assignment operator tests" << std::endl;
        total_score += calculate_score(assignment_operator_results,
                                       assignment_operator_results.multiplier,
                                       write_dynamic_size_sorted_keys,
                                       grading_file);

        grading_file << "Self assignment tests" << std::endl;
        total_score += calculate_score(self_assignment_results,
                                       self_assignment_results.multiplier,
                                       write_dynamic_size_sorted_keys,
                                       grading_file);
    }

    grading_file << "score: " << total_score << std::endl;
}

size_t calculate_score(test_results &results,
                       size_t multiplier,
                       bool write_dynamic_size_sorted_keys,
                       std::ofstream &grading_file)
{
    size_t total_score = 0;

    if (results.basic_tests.first)
    {
        total_score += results.basic_tests.second;
        grading_file << "Basic tests passed " << std::endl;
    }
    else
    {
        grading_file << "Basic tests failed " << std::endl;
    }

    if (results.get_bucket_sizes.first)
    {
        total_score += results.get_bucket_sizes.second;
        grading_file << "Get bucket sizes passed " << std::endl;
    }
    else
    {
        grading_file << "Get bucket sizes failed " << std::endl;
    }

    if (results.get_all_keys.first)
    {
        total_score += results.get_all_keys.second;
        grading_file << "Get all keys passed" << std::endl;
    }
    else
    {
        grading_file << "Get all keys failed " << std::endl;
    }

    if (write_dynamic_size_sorted_keys)
    {
        if (results.get_all_sorted_keys.first)
        {
            total_score += results.get_all_sorted_keys.second;
            grading_file << "Get all sorted keys passed" << std::endl;
        }
        else
        {
            grading_file << "Get all sorted keys failed" << std::endl;
        }

        if (results.dynamic_size.first)
        {
            total_score += results.dynamic_size.second;
            grading_file << "Dynamic size passed " << std::endl;
        }
        else
        {
            grading_file << "Dynamic size failed" << std::endl;
        }
    }

    grading_file << std::endl;

    return total_score * multiplier;
}
