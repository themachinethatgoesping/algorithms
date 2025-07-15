// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <fmt/core.h>
#include <xtensor/misc/xsort.hpp>
#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/algorithms/imageprocessing/functions/find_local_maxima.hpp>


// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::imageprocessing::functions;

#define TESTTAG "[location]"

TEST_CASE("find_local_maxima functions should reproduce previously computed results", TESTTAG)
{
    using Catch::Approx;

    // Set random seed for reproducibility
    std::srand(42);

    // Create 50x50x50 array with background value 1
    xt::xtensor<float, 3> data = xt::empty<float>({ 50, 50, 50 });

    // Apply random noise < 2.f to data
    std::generate(data.begin(), data.end(), []() {
        return static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.99f));
    });

    // Create 5 random peaks
    struct Peak
    {
        int64_t x, y, z;
        float   value;
    };
    std::vector<Peak> peaks = { { 10, 20, 30, 6.5f },
                                { 25, 25, 25, 7.2f },
                                { 30, 15, 35, 8.8f },
                                { 35, 35, 20, 9.1f },
                                { 40, 40, 45, 10.5f } };

    // Set peaks in data
    for (const auto& peak : peaks)
    {
        data(peak.x, peak.y, peak.z) += peak.value;
    }

    // Find local maxima
    auto maxima = find_local_maxima(data, 2.0f);

    // Create a formatted string with maxima information
    std::string maxima_info;
    for (size_t i = 0; i < std::get<0>(maxima).size(); ++i)
    {
        maxima_info += fmt::format("Peak {}: x = {}, y = {}, z = {}, value = {}\n",
                                   i,
                                   std::get<0>(maxima)[i],
                                   std::get<1>(maxima)[i],
                                   std::get<2>(maxima)[i],
                                   std::get<3>(maxima)[i]);
    }
    // Print maxima information using Catch2 INFO
    INFO(maxima_info);

    // Test results
    REQUIRE(std::get<0>(maxima).size() == peaks.size());
    REQUIRE(std::get<1>(maxima).size() == peaks.size());
    REQUIRE(std::get<2>(maxima).size() == peaks.size());
    REQUIRE(std::get<3>(maxima).size() == peaks.size());
    

    // Sort maxima by x for consistent comparison
    std::vector<size_t> sorted_indices(std::get<0>(maxima).size());
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](size_t a, size_t b) {
        return std::get<0>(maxima)[a] < std::get<0>(maxima)[b];
    });

    // Check if peaks are found near original positions (allowing for slight shift due to smoothing)
    for (const auto i: sorted_indices)
    {
        const auto& peak  = peaks[i];

        // Check if peak is the original position/value
        REQUIRE(std::get<0>(maxima)[i] == peak.x);
        REQUIRE(std::get<1>(maxima)[i] == peak.y);
        REQUIRE(std::get<2>(maxima)[i] == peak.z);
        REQUIRE(std::get<3>(maxima)[i] == Approx(peak.value).margin(2.0)); // margin because of noise
    }
}

// Ai generated/copied test cases below

TEST_CASE("find_local_maxima functions should reproduce previously computed results in 2D", TESTTAG)
{
    using Catch::Approx;

    // Set random seed for reproducibility
    std::srand(42);

    // Create 50x50 array with background value 1
    xt::xtensor<float, 2> data = xt::empty<float>({ 50, 50 });

    // Apply random noise < 2.f to data
    std::generate(data.begin(), data.end(), []() {
        return static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.99f));
    });

    // Create 5 random peaks
    struct Peak
    {
        int64_t x, y;
        float   value;
    };
    std::vector<Peak> peaks = { { 10, 20, 6.5f },
                                { 25, 25, 7.2f },
                                { 30, 15, 8.8f },
                                { 35, 35, 9.1f },
                                { 40, 40, 10.5f } };

    // Set peaks in data
    for (const auto& peak : peaks)
    {
        data(peak.x, peak.y) += peak.value;
    }

    // Find local maxima
    auto maxima = find_local_maxima(data, 2.0f);

    // Create a formatted string with maxima information
    std::string maxima_info;
    for (size_t i = 0; i < std::get<0>(maxima).size(); ++i)
    {
        maxima_info += fmt::format("Peak {}: x = {}, y = {}, value = {}\n",
                                   i,
                                   std::get<0>(maxima)[i],
                                   std::get<1>(maxima)[i],
                                   std::get<2>(maxima)[i]);
    }
    // Print maxima information using Catch2 INFO
    INFO(maxima_info);

    // Test results
    REQUIRE(std::get<0>(maxima).size() == peaks.size());
    REQUIRE(std::get<1>(maxima).size() == peaks.size());
    REQUIRE(std::get<2>(maxima).size() == peaks.size());

    // Sort maxima by x for consistent comparison
    std::vector<size_t> sorted_indices(std::get<0>(maxima).size());
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](size_t a, size_t b) {
        return std::get<0>(maxima)[a] < std::get<0>(maxima)[b];
    });

    // Check if peaks are found near original positions (allowing for slight shift due to smoothing)
    for (const auto i: sorted_indices)
    {
        const auto& peak  = peaks[i];

        // Check if peak is the original position/value
        REQUIRE(std::get<0>(maxima)[i] == peak.x);
        REQUIRE(std::get<1>(maxima)[i] == peak.y);
        REQUIRE(std::get<2>(maxima)[i] == Approx(peak.value).margin(2.0)); // margin because of noise
    }
}

TEST_CASE("find_local_maxima functions should reproduce previously computed results in 1D", TESTTAG)
{
    using Catch::Approx;

    // Set random seed for reproducibility
    std::srand(42);

    // Create 50 array with background value 1
    xt::xtensor<float, 1> data = xt::empty<float>({ 50 });

    // Apply random noise < 2.f to data
    std::generate(data.begin(), data.end(), []() {
        return static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.99f));
    });

    // Create 5 random peaks
    struct Peak
    {
        int64_t x;
        float   value;
    };
    std::vector<Peak> peaks = { { 10, 6.5f },
                                { 25, 7.2f },
                                { 30, 8.8f },
                                { 35, 9.1f },
                                { 40, 10.5f } };

    // Set peaks in data
    for (const auto& peak : peaks)
    {
        data(peak.x) += peak.value;
    }

    // Find local maxima
    auto maxima = find_local_maxima(data, 2.0f);

    // Create a formatted string with maxima information
    std::string maxima_info;
    for (size_t i = 0; i < std::get<0>(maxima).size(); ++i)
    {
        maxima_info += fmt::format("Peak {}: x = {}, value = {}\n",
                                   i,
                                   std::get<0>(maxima)[i],
                                   std::get<1>(maxima)[i]);
    }
    // Print maxima information using Catch2 INFO
    INFO(maxima_info);

    // Test results
    REQUIRE(std::get<0>(maxima).size() == peaks.size());
    REQUIRE(std::get<1>(maxima).size() == peaks.size());

    // Sort maxima by x for consistent comparison
    std::vector<size_t> sorted_indices(std::get<0>(maxima).size());
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](size_t a, size_t b) {
        return std::get<0>(maxima)[a] < std::get<0>(maxima)[b];
    });

    // Check if peaks are found near original positions (allowing for slight shift due to smoothing)
    for (const auto i: sorted_indices)
    {
        const auto& peak  = peaks[i];

        // Check if peak is the original position/value
        REQUIRE(std::get<0>(maxima)[i] == peak.x);
        REQUIRE(std::get<1>(maxima)[i] == Approx(peak.value).margin(2.0)); // margin because of noise
    }
}
