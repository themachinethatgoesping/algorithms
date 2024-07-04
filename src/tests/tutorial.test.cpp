// SPDX-FileCopyrightText: 2022 Peter Urban, GEOMAR Helmholtz Centre for Ocean Research Kiel
// SPDX-FileCopyrightText: 2022 - 2023 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>
#include <thread>

#include <themachinethatgoesping/tools/helper/omp_helper.hpp>

// using namespace testing;
using namespace std;
// using namespace themachinethatgoesping;

namespace tutorial_tests {

/**
 * @brief This function checks if the openmp parallel pragma is working.
 * A parallel loop is executed and each thread writes '1' to a vector (size == number of elements)
 * and waits 100ms.
 *
 * If all elements of the vector are set to one, the test is successful.
 */
inline bool openmp_parallel_pragma_is_working()
{
    const int omp_max_threads = omp_get_max_threads();

    // test does not make sense if there is only one thread
    if (omp_max_threads < 2)
    {
        std::cerr << "omp_max_threads is less than 2" << std::endl;
        return false;
    }

    // vector with thread responses (1 = success, 0 = failure)
    std::vector<int> thread_responses(omp_max_threads);

#pragma omp parallel for
    for (int i = 0; i < omp_max_threads; ++i)
    {
        std::cerr << "Hello from thread " << omp_get_thread_num() << std::endl;
        thread_responses.at(omp_get_thread_num()) = 1;
        std::chrono::milliseconds duration(100);
        std::this_thread::sleep_for(duration);
    }

    for (int i = 0; i < omp_max_threads; ++i)
    {
        if (thread_responses.at(i) != 1)
        {
            std::cerr << "thread_responses.at(" << i << ") != 1" << std::endl;
            return false;
        }
    }

    return true;
}
TEST_CASE("openmp_parallel_pragma_is_working_in_header")
{
    REQUIRE(openmp_parallel_pragma_is_working());
}

}
