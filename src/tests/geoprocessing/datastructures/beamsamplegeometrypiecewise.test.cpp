// SPDX-FileCopyrightText: 2022 - 2026 Peter Urban, Ghent University
//
// SPDX-License-Identifier: MPL-2.0

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <sstream>

#include "../../../themachinethatgoesping/algorithms/geoprocessing/datastructures/beamsamplegeometrypiecewise.hpp"

using namespace themachinethatgoesping::algorithms::geoprocessing::datastructures;

#define TESTTAG "[piecewise][datastructures]"

TEST_CASE("BeamSampleGeometryPiecewise from_layer_xyz, K=1 single segment", TESTTAG)
{
    // 2 beams, 2 knots (K=1 segment), simple linear ramps
    xt::xtensor<float, 1>        first  = { 0.f, 0.f };
    xt::xtensor<unsigned int, 1> n      = { 100u, 100u };
    xt::xtensor<float, 1>        knots  = { 0.f, 100.f };
    xt::xtensor<float, 3>        xyz = xt::zeros<float>({ size_t(2), size_t(2), size_t(3) });
    // beam 0: x = sample_nr*0.1, y = 0, z = sample_nr*1.0
    xyz(1, 0, 0) = 10.f;  // x@k=1
    xyz(1, 0, 2) = 100.f; // z@k=1
    // beam 1: x = 0, y = sample_nr*0.5, z = sample_nr*1.5
    xyz(1, 1, 1) = 50.f;
    xyz(1, 1, 2) = 150.f;

    auto g = BeamSampleGeometryPiecewise::from_layer_xyz(first, n, knots, xyz);

    REQUIRE(g.get_n_beams() == 2);
    REQUIRE(g.get_n_segments() == 1);
    REQUIRE(g.has_x()); REQUIRE(g.has_y()); REQUIRE(g.has_z());

    // forward eval at sample_nr=0 -> origin
    auto p0 = g.eval_xyz(0, 0.f);
    REQUIRE_THAT(p0[0], Catch::Matchers::WithinAbs(0.f, 1e-4f));
    REQUIRE_THAT(p0[2], Catch::Matchers::WithinAbs(0.f, 1e-4f));
    // at sample_nr=100 -> end of ramp
    auto p1 = g.eval_xyz(0, 100.f);
    REQUIRE_THAT(p1[0], Catch::Matchers::WithinAbs(10.f, 1e-3f));
    REQUIRE_THAT(p1[2], Catch::Matchers::WithinAbs(100.f, 1e-3f));
    // mid sample -> half
    auto pm = g.eval_xyz(1, 50.f);
    REQUIRE_THAT(pm[1], Catch::Matchers::WithinAbs(25.f, 1e-3f));
    REQUIRE_THAT(pm[2], Catch::Matchers::WithinAbs(75.f, 1e-3f));

    REQUIRE(g.is_single_affine());
}

TEST_CASE("BeamSampleGeometryPiecewise from_layer_xyz, K=3 multi segment", TESTTAG)
{
    // 1 beam, 4 knots (K=3 segments). Each segment has different slope.
    xt::xtensor<float, 1>        first = { 0.f };
    xt::xtensor<unsigned int, 1> n     = { 300u };
    xt::xtensor<float, 1>        knots = { 0.f, 100.f, 200.f, 300.f };
    xt::xtensor<float, 3>        xyz   = xt::zeros<float>({ size_t(4), size_t(1), size_t(3) });
    xyz(0, 0, 2) = 0.f;
    xyz(1, 0, 2) = 100.f;
    xyz(2, 0, 2) = 150.f; // slope drops from 1.0 to 0.5
    xyz(3, 0, 2) = 200.f; // slope picks back up to 0.5

    auto g = BeamSampleGeometryPiecewise::from_layer_xyz(first, n, knots, xyz);
    REQUIRE(g.get_n_segments() == 3);

    // exact at knots
    REQUIRE_THAT(g.eval_xyz(0, 0.f)[2],   Catch::Matchers::WithinAbs(0.f,   1e-3f));
    REQUIRE_THAT(g.eval_xyz(0, 100.f)[2], Catch::Matchers::WithinAbs(100.f, 1e-3f));
    REQUIRE_THAT(g.eval_xyz(0, 200.f)[2], Catch::Matchers::WithinAbs(150.f, 1e-3f));
    REQUIRE_THAT(g.eval_xyz(0, 300.f)[2], Catch::Matchers::WithinAbs(200.f, 1e-3f));

    // mid-segment
    REQUIRE_THAT(g.eval_xyz(0, 150.f)[2], Catch::Matchers::WithinAbs(125.f, 1e-3f));
    REQUIRE_THAT(g.eval_xyz(0, 250.f)[2], Catch::Matchers::WithinAbs(175.f, 1e-3f));

    // not single affine (slopes differ across segments)
    REQUIRE_FALSE(g.is_single_affine(1e-3f));
}

TEST_CASE("BeamSampleGeometryPiecewise auto-collapse to single affine", TESTTAG)
{
    // K=3 segments but all collinear (same slope)
    xt::xtensor<float, 1>        first = { 0.f };
    xt::xtensor<unsigned int, 1> n     = { 300u };
    xt::xtensor<float, 1>        knots = { 0.f, 100.f, 200.f, 300.f };
    xt::xtensor<float, 3>        xyz   = xt::zeros<float>({ size_t(4), size_t(1), size_t(3) });
    for (int i = 0; i < 4; ++i) xyz(i, 0, 2) = (float)i * 100.f; // slope = 1 everywhere

    auto g = BeamSampleGeometryPiecewise::from_layer_xyz(first, n, knots, xyz);
    REQUIRE(g.is_single_affine(1e-4f));

    auto single = g.to_single_affine();
    REQUIRE(single.get_n_beams() == 1);
}

TEST_CASE("BeamSampleGeometryPiecewise binary roundtrip", TESTTAG)
{
    xt::xtensor<float, 1>        first = { 0.f, 0.f };
    xt::xtensor<unsigned int, 1> n     = { 100u, 100u };
    xt::xtensor<float, 1>        knots = { 0.f, 50.f, 100.f };
    xt::xtensor<float, 3>        xyz   = xt::zeros<float>({ size_t(3), size_t(2), size_t(3) });
    xyz(1, 0, 2) = 50.f;  xyz(2, 0, 2) = 100.f;
    xyz(1, 1, 2) = 75.f;  xyz(2, 1, 2) = 150.f;

    auto g = BeamSampleGeometryPiecewise::from_layer_xyz(first, n, knots, xyz);

    REQUIRE(g == BeamSampleGeometryPiecewise(g)); // copy

    std::stringstream buf;
    g.to_stream(buf);
    auto g2 = BeamSampleGeometryPiecewise::from_stream(buf);
    REQUIRE(g == g2);

    REQUIRE(g.info_string().size() != 0);
}
