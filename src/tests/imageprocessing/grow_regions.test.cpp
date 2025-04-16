// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <fmt/core.h>
#include <xtensor/misc/xsort.hpp>
#include <xtensor/containers/xtensor.hpp>


#include <themachinethatgoesping/algorithms/imageprocessing/functions/grow_regions.hpp>
#include <themachinethatgoesping/tools/helper.hpp>

using namespace std;
using namespace themachinethatgoesping::algorithms::imageprocessing::functions;

#define TESTTAG "[imageprocessing]"

TEST_CASE("grow_regions in 3D", TESTTAG)
{
    using Catch::Approx;
    std::srand(42);
    
    // Create test data
    xt::xtensor<float, 3> data = xt::empty<float>({ 50, 50, 50 });
    std::generate(data.begin(), data.end(), []() {
        return static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.99f));
    });

    // Define peaks in the data
    struct Peak
    {
        int64_t x, y, z;
        float   value;
        int     region_id;
    };
    
    std::vector<Peak> peaks = { 
        { 10, 20, 30, 6.5f, 1 },
        { 25, 25, 25, 7.2f, 2 },
        { 30, 15, 35, 8.8f, 3 },
        { 35, 35, 20, 9.1f, 4 },
        { 40, 40, 45, 10.5f, 5 } 
    };
    
    // Add peaks to data
    for (auto& p : peaks)
        data(p.x, p.y, p.z) += p.value;

    // Create initial regions tensor with null_region = 0
    xt::xtensor<int, 3> regions = xt::zeros<int>({ 50, 50, 50 });
    const int null_region = 0;
    
    // Set initial region seeds at peak locations
    for (auto& p : peaks)
        regions(p.x, p.y, p.z) = p.region_id;
    
    // Count initial non-null regions
    size_t initial_region_count = 0;
    for (auto r : regions)
        if (r != null_region)
            initial_region_count++;
    
    REQUIRE(initial_region_count == peaks.size());
    
    // Test growing regions
    float threshold = .5f;
    bool changes = false;
    
    // Run grow_regions until no more changes
    int iterations = 0;
    const int max_iterations = 20;
    
    do {
        changes = grow_regions(regions, data, null_region, threshold);
        iterations++;
    } while (changes && iterations < max_iterations);
    
    INFO(fmt::format("Regions grown in {} iterations", iterations));
    
    // Count final non-null regions
    size_t final_region_count = 0;
    for (auto r : regions)
        if (r != null_region)
            final_region_count++;
    
    INFO(fmt::format("Initial region count: {}, Final region count: {}", initial_region_count, final_region_count));
    
    // Verify growth happened
    REQUIRE(final_region_count > initial_region_count);
    
    // Verify each peak is in the correct region
    for (auto& p : peaks) {
        INFO(fmt::format("Checking peak at ({},{},{}) with region_id {}", p.x, p.y, p.z, p.region_id));
        REQUIRE(regions(p.x, p.y, p.z) == p.region_id);
    }
    
    // Verify some properties of the grown regions
    std::vector<size_t> region_sizes(peaks.size() + 1, 0);
    
    for (auto r : regions) {
        if (r != null_region && r <= int64_t(peaks.size())) {
            region_sizes[r]++;
        }
    }
    
    // Verify each region has grown
    for (size_t i = 1; i <= peaks.size(); i++) {
        INFO(fmt::format("Region {} size: {}", i, region_sizes[i]));
        REQUIRE(region_sizes[i] > 1);
    }
}
TEST_CASE("grow_regions in 2D", TESTTAG)
{
    using Catch::Approx;
    std::srand(43);
    
    // Create test data
    xt::xtensor<float, 2> data = xt::empty<float>({ 50, 50 });
    std::generate(data.begin(), data.end(), []() {
        return static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.99f));
    });

    // Define peaks in the data
    struct Peak
    {
        int64_t x, y;
        float   value;
        int     region_id;
    };
    
    std::vector<Peak> peaks = { 
        { 10, 20, 6.5f, 1 },
        { 25, 25, 7.2f, 2 },
        { 30, 15, 8.8f, 3 },
        { 35, 35, 9.1f, 4 },
        { 40, 40, 10.5f, 5 } 
    };
    
    // Add peaks to data
    for (auto& p : peaks)
        data(p.x, p.y) += p.value;

    // Create initial regions tensor with null_region = 0
    xt::xtensor<int, 2> regions = xt::zeros<int>({ 50, 50 });
    const int null_region = 0;
    
    // Set initial region seeds at peak locations
    for (auto& p : peaks)
        regions(p.x, p.y) = p.region_id;
    
    // Count initial non-null regions
    size_t initial_region_count = 0;
    for (auto r : regions)
        if (r != null_region)
            initial_region_count++;
    
    REQUIRE(initial_region_count == peaks.size());
    
    // Test growing regions
    float threshold = .5f;
    bool changes = false;
    
    // Run grow_regions until no more changes
    int iterations = 0;
    const int max_iterations = 20;
    
    do {
        changes = grow_regions(regions, data, null_region, threshold);
        iterations++;
    } while (changes && iterations < max_iterations);
    
    INFO(fmt::format("2D Regions grown in {} iterations", iterations));
    
    // Count final non-null regions
    size_t final_region_count = 0;
    for (auto r : regions)
        if (r != null_region)
            final_region_count++;
    
    INFO(fmt::format("2D Initial region count: {}, Final region count: {}", initial_region_count, final_region_count));
    
    // Verify growth happened
    REQUIRE(final_region_count > initial_region_count);
    
    // Verify each peak is in the correct region
    for (auto& p : peaks) {
        INFO(fmt::format("Checking 2D peak at ({},{}) with region_id {}", p.x, p.y, p.region_id));
        REQUIRE(regions(p.x, p.y) == p.region_id);
    }
    
    // Verify some properties of the grown regions
    std::vector<size_t> region_sizes(peaks.size() + 1, 0);
    
    for (auto r : regions) {
        if (r != null_region && r <= int64_t(peaks.size())) {
            region_sizes[r]++;
        }
    }
    
    // Verify each region has grown
    for (size_t i = 1; i <= peaks.size(); i++) {
        INFO(fmt::format("2D Region {} size: {}", i, region_sizes[i]));
        REQUIRE(region_sizes[i] > 1);
    }
}

TEST_CASE("grow_regions in 1D", TESTTAG)
{
    using Catch::Approx;
    std::srand(44);
    
    // Create test data
    xt::xtensor<float, 1> data = xt::empty<float>({ 100 });
    std::generate(data.begin(), data.end(), []() {
        return static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.99f));
    });

    // Define peaks in the data
    struct Peak
    {
        int64_t x;
        float   value;
        int     region_id;
    };
    
    std::vector<Peak> peaks = { 
        { 10, 6.5f, 1 },
        { 30, 7.2f, 2 },
        { 50, 8.8f, 3 },
        { 70, 9.1f, 4 },
        { 90, 10.5f, 5 } 
    };
    
    // Add peaks to data
    for (auto& p : peaks)
        data(p.x) += p.value;

    // Create initial regions tensor with null_region = 0
    xt::xtensor<int, 1> regions = xt::zeros<int>({ 100 });
    const int null_region = 0;
    
    // Set initial region seeds at peak locations
    for (auto& p : peaks)
        regions(p.x) = p.region_id;
    
    // Count initial non-null regions
    size_t initial_region_count = 0;
    for (auto r : regions)
        if (r != null_region)
            initial_region_count++;
    
    REQUIRE(initial_region_count == peaks.size());
    
    // Test growing regions
    bool changes = false;
    
    // Run grow_regions until no more changes
    int iterations = 0;
    const int max_iterations = 20;
    
    do {
        changes = grow_regions(regions, data, null_region);
        iterations++;
    } while (changes && iterations < max_iterations);
    
    INFO(fmt::format("1D Regions grown in {} iterations", iterations));
    
    // Count final non-null regions
    size_t final_region_count = 0;
    for (auto r : regions)
        if (r != null_region)
            final_region_count++;
    
    INFO(fmt::format("1D Initial region count: {}, Final region count: {}", initial_region_count, final_region_count));
    
    // Verify growth happened
    REQUIRE(final_region_count > initial_region_count);
    
    // Verify each peak is in the correct region
    for (auto& p : peaks) {
        INFO(fmt::format("Checking 1D peak at ({}) with region_id {}", p.x, p.region_id));
        REQUIRE(regions(p.x) == p.region_id);
    }
    
    // Verify some properties of the grown regions
    std::vector<size_t> region_sizes(peaks.size() + 1, 0);
    
    for (auto r : regions) {
        if (r != null_region && r <= int64_t(peaks.size())) {
            region_sizes[r]++;
        }
    }
    
    // Verify each region has grown
    for (size_t i = 1; i <= peaks.size(); i++) {
        INFO(fmt::format("1D Region {} size: {}", i, region_sizes[i]));
        REQUIRE(region_sizes[i] > 1);
    }
}
