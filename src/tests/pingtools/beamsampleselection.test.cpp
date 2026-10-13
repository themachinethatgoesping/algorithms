// SPDX-FileCopyrightText: 2022 - 2025 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <xtensor/containers/xtensor.hpp>

#include <themachinethatgoesping/algorithms/pingtools/beamsampleselection.hpp>

// using namespace testing;
using namespace std;
using namespace themachinethatgoesping::algorithms::pingtools;
#define TESTTAG "[pingtools]"

TEST_CASE("BeamSampleSelection should support common functions", TESTTAG)
{
    // initialize class structure
    auto obj = BeamSampleSelection(2);
    CHECK(obj.empty());

    // add some beams
    obj.add_beam(0, 1, 10);
    obj.add_beam(1, 3, 11);
    obj.add_beam(2, 2, 10);
    obj.add_beam(4, 1, 9);
    CHECK(obj.empty() == false);

    // test variables
    CHECK(obj.get_sample_step_ensemble() == 2);
    CHECK(obj.get_first_sample_number_ensemble() == 1);
    CHECK(obj.get_last_sample_number_ensemble() == 11);
    CHECK(obj.get_number_of_beams() == 4);
    CHECK(obj.get_number_of_samples_ensemble() == 6);

    CHECK(obj.get_beam_numbers() == std::vector<uint32_t>{ 0, 1, 2, 4 });
    CHECK(obj.get_first_sample_number_per_beam() == std::vector<uint32_t>{ 1, 3, 2, 1 });
    CHECK(obj.get_last_sample_number_per_beam() == std::vector<uint32_t>{ 10, 11, 10, 9 });

    // test alternative intialization
    xt::xtensor<uint32_t, 1> start_range_sample_numbers{ 1, 3, 2 };
    xt::xtensor<uint32_t, 1> number_of_samples_per_beam{ 10, 9, 9 };
    auto last_sample_numbers = start_range_sample_numbers + number_of_samples_per_beam - 1;
    auto obj2                = BeamSampleSelection(
        std::vector<uint32_t>(start_range_sample_numbers.begin(), start_range_sample_numbers.end()),
        std::vector<uint32_t>(last_sample_numbers.begin(), last_sample_numbers.end()),
        3);

    // test variables alternative
    CHECK(obj2.get_sample_step_ensemble() == 3);
    CHECK(obj2.get_first_sample_number_ensemble() == 1);
    CHECK(obj2.get_last_sample_number_ensemble() == 11);
    CHECK(obj2.get_number_of_beams() == 3);
    CHECK(obj2.get_number_of_samples_ensemble() == 4);

    CHECK(obj2.get_beam_numbers() == std::vector<uint32_t>{ 0, 1, 2 });
    CHECK(obj2.get_first_sample_number_per_beam() == std::vector<uint32_t>{ 1, 3, 2 });
    CHECK(obj2.get_last_sample_number_per_beam() == std::vector<uint32_t>{ 10, 11, 10 });

    CHECK(obj != obj2);
    obj2.add_beam(4, 1, 9);
    obj2.set_sample_step_ensemble(2);
    CHECK(obj == obj2);

    // test inequality
    CHECK(obj != BeamSampleSelection());

    // test copy
    CHECK(obj == BeamSampleSelection(obj));

    // test binary
    CHECK(obj == BeamSampleSelection(obj.from_binary(obj.to_binary())));

    // test stream
    std::stringstream buffer;
    obj.to_stream(buffer);
    CHECK(obj == BeamSampleSelection(obj.from_stream(buffer)));

    // test print does not crash
    CHECK(obj.info_string().size() != 0);

    //--- Class concept ---
}

TEST_CASE("BeamSampleSelection should create correct readsampleselection", TESTTAG)
{
    // initialize class structure
    auto bss = BeamSampleSelection(2);

    // add some beams
    bss.add_beam(0, 1, 10);
    bss.add_beam(1, 3, 11);
    bss.add_beam(2, 2, 10);
    bss.add_beam(4, 1, 9);

    // test readsamplerange beam 1
    auto rsr = bss.get_read_sample_range(0, 0, 20);

    CHECK(rsr.get_first_sample_to_read() == 1);
    // CHECK(rsr.get_number_of_samples_to_read() == 10);
    CHECK(rsr.get_first_read_sample_offset() == 1);
    //    CHECK(rsr.get_last_read_sample_offset() == 10);

    // test readsamplerange beam 2
    rsr = bss.get_read_sample_range(1, 5, 20);

    CHECK(rsr.get_first_sample_to_read() == 0);
    //    CHECK(rsr.get_number_of_samples_to_read() == 7);
    CHECK(rsr.get_first_read_sample_offset() == 5);
    CHECK(rsr.get_last_read_sample_offset() == 11);

    // test readsamplerange beam 3
    rsr = bss.get_read_sample_range(2, 1, 5);

    CHECK(rsr.get_first_sample_to_read() == 1);
    //    CHECK(rsr.get_number_of_samples_to_read() == 4);
    CHECK(rsr.get_first_read_sample_offset() == 2);
    //    CHECK(rsr.get_last_read_sample_offset() == 5);

    // test readsamplerange beam 4
    rsr = bss.get_read_sample_range(3, 3, 5);

    CHECK(rsr.get_first_sample_to_read() == 0);
    // CHECK(rsr.get_number_of_samples_to_read() == 5);
    CHECK(rsr.get_first_read_sample_offset() == 3);
    CHECK(rsr.get_last_read_sample_offset() == 7);

    // test including ensemble offset
    bss.set_first_sample_number_ensemble(5);
    rsr = bss.get_read_sample_range(3, 3, 5);

    CHECK(rsr.get_first_sample_to_read() == 2);
    //    CHECK(rsr.get_number_of_samples_to_read() == 3);
    CHECK(rsr.get_first_read_sample_offset() == 5);
    CHECK(rsr.get_last_read_sample_offset() == 7);

    bss.set_last_sample_number_ensemble(7);
    rsr = bss.get_read_sample_range(3, 3, 5);
    rsr.print(std::cerr);

    CHECK(rsr.get_first_sample_to_read() == 2);
    // CHECK(rsr.get_number_of_samples_to_read() == 3);
    CHECK(rsr.get_first_read_sample_offset() == 5);
    CHECK(rsr.get_last_read_sample_offset() == 7);
}
