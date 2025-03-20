// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <fmt/core.h>
#include <xtensor/xsort.hpp>
#include <xtensor/xtensor.hpp>

#include <themachinethatgoesping/algorithms/imageprocessing/functions/find_local_maxima2.hpp>
#include <themachinethatgoesping/tools/helper.hpp>

using namespace std;
using namespace themachinethatgoesping::algorithms::imageprocessing::functions;

#define TESTTAG "[location]"

TEST_CASE("find_local_maxima2 in 3D", TESTTAG)
{
    using Catch::Approx;
    std::srand(42);
    xt::xtensor<float, 3> data = xt::empty<float>({ 50, 50, 50 });
    std::generate(data.begin(), data.end(), []() {
        return static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.99f));
    });

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
    for (auto& p : peaks)
        data(p.x, p.y, p.z) += p.value;

    auto maxima = find_local_maxima2(data, 2.0f);
    REQUIRE(maxima.size() == peaks.size());

    std::string info_str;
    for (size_t i = 0; i < maxima.size(); ++i)
    {
        auto val = data(maxima[i][0], maxima[i][1], maxima[i][2]);
        info_str += fmt::format("Peak {}: x={}, y={}, z={}, value={}\n",
                                i,
                                maxima[i][0],
                                maxima[i][1],
                                maxima[i][2],
                                val);
    }
    INFO(info_str);

    // Sort maxima by x
    std::vector<size_t> sorted_indices(maxima.size());
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](size_t a, size_t b) {
        return maxima[a][0] < maxima[b][0];
    });

    for (size_t idx = 0; idx < sorted_indices.size(); ++idx)
    {
        const auto& peak = peaks[idx];
        const auto  i    = sorted_indices[idx];
        REQUIRE(maxima[i][0] == peak.x);
        REQUIRE(maxima[i][1] == peak.y);
        REQUIRE(maxima[i][2] == peak.z);
        auto val = data(peak.x, peak.y, peak.z);
        REQUIRE(val == Approx(peak.value).margin(2.0));
    }

    //check that the returned maxima type is compatible with xt::index_view
    auto lm_values = xt::index_view(data, maxima);
    for (size_t idx = 0; idx < sorted_indices.size(); ++idx)
    {
        INFO(fmt::format("lm_values[{}]: {} peaks[{}].value: {}", idx, lm_values[idx], idx, peaks[idx].value));
        REQUIRE(lm_values[idx] == Approx(peaks[idx].value).margin(2.0));
        REQUIRE(lm_values[idx] == Approx(data(peaks[idx].x,peaks[idx].y,peaks[idx].z)).margin(0.1));

    }
}

TEST_CASE("find_local_maxima2 in 2D", TESTTAG)
{
    using Catch::Approx;
    std::srand(42);
    xt::xtensor<float, 2> data = xt::empty<float>({ 50, 50 });
    std::generate(data.begin(), data.end(), []() {
        return static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.99f));
    });

    struct Peak
    {
        int64_t x, y;
        float   value;
    };
    std::vector<Peak> peaks = {
        { 10, 20, 6.5f }, { 25, 25, 7.2f }, { 30, 15, 8.8f }, { 35, 35, 9.1f }, { 40, 40, 10.5f }
    };
    for (auto& p : peaks)
        data(p.x, p.y) += p.value;

    auto maxima = find_local_maxima2(data, 2.0f);
    REQUIRE(maxima.size() == peaks.size());

    std::string info_str;
    for (size_t i = 0; i < maxima.size(); ++i)
    {
        auto val = data(maxima[i][0], maxima[i][1]);
        info_str +=
            fmt::format("Peak {}: x={}, y={}, value={}\n", i, maxima[i][0], maxima[i][1], val);
    }
    INFO(info_str);

    // Sort maxima by x
    std::vector<size_t> sorted_indices(maxima.size());
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](size_t a, size_t b) {
        return maxima[a][0] < maxima[b][0];
    });

    for (size_t idx = 0; idx < sorted_indices.size(); ++idx)
    {
        const auto& peak = peaks[idx];
        const auto  i    = sorted_indices[idx];
        REQUIRE(maxima[i][0] == peak.x);
        REQUIRE(maxima[i][1] == peak.y);
        auto val = data(peak.x, peak.y);
        REQUIRE(val == Approx(peak.value).margin(2.0));
    }

    //check that the returned maxima type is compatible with xt::index_view
    auto lm_values = xt::index_view(data, maxima);
    for (size_t idx = 0; idx < sorted_indices.size(); ++idx)
    {
        INFO(fmt::format("lm_values[{}]: {} peaks[{}].value: {}", idx, lm_values[idx], idx, peaks[idx].value));
        REQUIRE(lm_values[idx] == Approx(peaks[idx].value).margin(2.0));
        REQUIRE(lm_values[idx] == Approx(data(peaks[idx].x,peaks[idx].y)).margin(0.1));

    }
}

TEST_CASE("find_local_maxima2 in 1D", TESTTAG)
{
    using Catch::Approx;
    std::srand(42);
    xt::xtensor<float, 1> data = xt::empty<float>({ 50 });
    std::generate(data.begin(), data.end(), []() {
        return static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.99f));
    });

    struct Peak
    {
        int64_t x;
        float   value;
    };
    std::vector<Peak> peaks = {
        { 10, 6.5f }, { 25, 7.2f }, { 30, 8.8f }, { 35, 9.1f }, { 40, 10.5f }
    };
    for (auto& p : peaks)
        data(p.x) += p.value;

    auto maxima = find_local_maxima2(data, 2.0f);
    REQUIRE(maxima.size() == peaks.size());

    std::string info_str;
    for (size_t i = 0; i < maxima.size(); ++i)
    {
        auto val = data(maxima[i]);
        info_str += fmt::format("Peak {}: x={}, value={}\n", i, maxima[i], val);
    }
    INFO(info_str);

    // Sort maxima by x
    std::vector<size_t> sorted_indices(maxima.size());
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](size_t a, size_t b) {
        return maxima[a] < maxima[b];
    });

    for (size_t idx = 0; idx < sorted_indices.size(); ++idx)
    {
        const auto& peak = peaks[idx];
        const auto  i    = sorted_indices[idx];
        REQUIRE(maxima[i] == peak.x);
        auto val = data(peak.x);
        REQUIRE(val == Approx(peak.value).margin(2.0));
    }

    //check that the returned maxima type is compatible with xt::index_view
    auto lm_values = xt::index_view(data, maxima);
    for (size_t idx = 0; idx < sorted_indices.size(); ++idx)
    {
        INFO(fmt::format("lm_values[{}]: {} peaks[{}].value: {}", idx, lm_values[idx], idx, peaks[idx].value));
        REQUIRE(lm_values[idx] == Approx(peaks[idx].value).margin(2.0));
        REQUIRE(lm_values[idx] == Approx(data(peaks[idx].x)).margin(0.1));

    }
}
