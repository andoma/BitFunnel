// The MIT License (MIT)

// Copyright (c) 2016, Microsoft

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#include "gtest/gtest.h"

#include <cmath>
#include <iostream>

#include "OptimalTermTreatments.h"
#include "TermTreatments.h"


namespace BitFunnel
{
    TEST(OptimalTermTreatmentsTest, Placeholder)
    {
        // This test exists solely as a demonstration of the optimal term
        // treatment algorithm. It is currently disabled so that it doesn't
        // slow down the unit test suite.
        // OptimalTermTreatments();
    }


    double ProbNotZero(double density)
    {
        return 1.0 - pow(1 - density, 64);
    }


    TEST(OptimalTermTreatmentsTest, SingleRank0)
    {
        const double c_density = 0.1;
        const double c_signal = 0.05;
        std::vector<int> rows = {1};
        auto metrics0 = AnalyzeAlternate(rows, c_density, c_signal);
        size_t rowConfig = SizeTFromRowVector(rows);
        auto metrics1 = Analyze(rowConfig, c_density, c_signal, false);

        double snr0 = metrics0.GetSNR();
        double snr1 = metrics1.second.GetSNR();

        ASSERT_FALSE(std::isinf(snr0));
        ASSERT_FALSE(std::isinf(snr1));
        ASSERT_FALSE(std::isnan(snr0));
        ASSERT_FALSE(std::isnan(snr1));
        EXPECT_LE(std::abs(snr0-1.0), 0.00000001);
        EXPECT_LE(std::abs(snr1-1.0), 0.00000001);

        double bits0 = metrics0.GetBits();
        double bits1 = metrics1.second.GetBits();
        const double c_expectedBits = c_signal / c_density;

        ASSERT_FALSE(std::isinf(bits0));
        ASSERT_FALSE(std::isinf(bits1));
        ASSERT_FALSE(std::isnan(bits0));
        ASSERT_FALSE(std::isnan(bits1));
        EXPECT_LE(std::abs(bits0-c_expectedBits), 0.00000001);
        EXPECT_LE(std::abs(bits1-c_expectedBits), 0.00000001);
    }


    TEST(OptimalTermTreatmentsTest, TwoRank0)
    {
        const double c_density = 0.1;
        const double c_signal = 0.05;
        std::vector<int> rows = {2};
        auto metrics0 = AnalyzeAlternate(rows, c_density, c_signal);
        size_t rowConfig = SizeTFromRowVector(rows);
        auto metrics1 = Analyze(rowConfig, c_density, c_signal, false);

        double snr0 = metrics0.GetSNR();
        double snr1 = metrics1.second.GetSNR();

        double singleRowNoise = (c_density - c_signal);
        double expectedNoise = singleRowNoise * singleRowNoise;
        const double c_expectedSNR = c_signal / expectedNoise;

        ASSERT_FALSE(std::isinf(snr0));
        ASSERT_FALSE(std::isinf(snr1));
        ASSERT_FALSE(std::isnan(snr0));
        ASSERT_FALSE(std::isnan(snr1));
        EXPECT_LE(std::abs(snr0-c_expectedSNR), 0.00000001);
        EXPECT_LE(std::abs(snr1-c_expectedSNR), 0.00000001);

        double bits0 = metrics0.GetBits();
        double bits1 = metrics1.second.GetBits();
        const double c_expectedBits = 2.0 * c_signal / c_density;

        ASSERT_FALSE(std::isinf(bits0));
        ASSERT_FALSE(std::isinf(bits1));
        ASSERT_FALSE(std::isnan(bits0));
        ASSERT_FALSE(std::isnan(bits1));
        EXPECT_LE(std::abs(bits0-c_expectedBits), 0.00000001);
        EXPECT_LE(std::abs(bits1-c_expectedBits), 0.00000001);
    }


    TEST(OptimalTermTreatmentsTest, PrivateRank0)
    {
        const double c_density = 0.1;
        const double c_signal = 0.8;
        std::vector<int> rows = {1};
        auto metrics0 = AnalyzeAlternate(rows, c_density, c_signal);
        size_t rowConfig = SizeTFromRowVector(rows);
        auto metrics1 = Analyze(rowConfig, c_density, c_signal, false);

        double snr0 = metrics0.GetSNR();
        double snr1 = metrics1.second.GetSNR();

        ASSERT_TRUE(std::isinf(snr0));
        ASSERT_TRUE(std::isinf(snr1));

        double bits0 = metrics0.GetBits();
        double bits1 = metrics1.second.GetBits();
        const double c_expectedBits = 1.0;

        ASSERT_FALSE(std::isinf(bits0));
        ASSERT_FALSE(std::isinf(bits1));
        ASSERT_FALSE(std::isnan(bits0));
        ASSERT_FALSE(std::isnan(bits1));
        EXPECT_LE(std::abs(bits0-c_expectedBits), 0.00000001);
        EXPECT_LE(std::abs(bits1-c_expectedBits), 0.00000001);
    }

    // Can't work right now because Analyze converts a row configuration into a size_t.
    // TEST(OptimalTermTreatmentsTest, SNRManyRank0)
    // {
    //     const double c_density = 0.1;
    //     const double c_signal = 0.05;
    //     std::vector<int> rows = {10000000};
    //     auto metrics0 = AnalyzeAlternate(rows, c_density, c_signal);
    //     size_t rowConfig = SizeTFromRowVector(rows);
    //     auto metrics1 = Analyze(rowConfig, c_density, c_signal, false);

    //     double c0 = metrics0.GetSNR();
    //     double c1 = metrics1.second.GetSNR();

    //     ASSERT_GT(c0, 1000.0);
    //     ASSERT_GT(c1, 1000.0);
    // }


    TEST(OptimalTermTreatmentsTest, PrivateRank6)
    {
        const double c_density = 0.1;
        const double c_signal = 0.1;
        std::vector<int> rows = {0, 0, 0, 0, 0, 0, 1};
        auto metrics0 = AnalyzeAlternate(rows, c_density, c_signal);
        size_t rowConfig = SizeTFromRowVector(rows);
        auto metrics1 = Analyze(rowConfig, c_density, c_signal, false);

        const double c_signalAt6 = Term::FrequencyAtRank(c_signal, 6);
        ASSERT_LE(c_signalAt6, 1.0);
        const double c_noise = c_signalAt6 - c_signal;
        const double c_expectedSNR = c_signal / c_noise;

        double snr0 = metrics0.GetSNR();
        double snr1 = metrics1.second.GetSNR();

        EXPECT_LE(std::abs(snr0-c_expectedSNR), 0.00000001);
        EXPECT_LE(std::abs(snr1-c_expectedSNR), 0.00000001);

        double bits0 = metrics0.GetBits();
        double bits1 = metrics1.second.GetBits();
        const double c_expectedBits = 1.0 / 64;

        ASSERT_FALSE(std::isinf(bits0));
        ASSERT_FALSE(std::isinf(bits1));
        ASSERT_FALSE(std::isnan(bits0));
        ASSERT_FALSE(std::isnan(bits1));
        EXPECT_LE(std::abs(bits0-c_expectedBits), 0.00000001);
        EXPECT_LE(std::abs(bits1-c_expectedBits), 0.00000001);
    }


    // TEST(OptimalTermTreatmentsTest, SNRSingleRank6)
    // {
    //     const double c_density = 0.1;
    //     const double c_signal = 0.00001;
    //     std::vector<int> rows = {0, 0, 0, 0, 0, 0, 1000};
    //     auto metrics0 = AnalyzeAlternate(rows, c_density, c_signal);
    //     size_t rowConfig = SizeTFromRowVector(rows);
    //     auto metrics1 = Analyze(rowConfig, c_density, c_signal, false);

    //     double c0 = metrics0.GetSNR();
    //     double c1 = metrics1.second.GetSNR();

    //     ASSERT_FALSE(std::isinf(c0));
    //     ASSERT_FALSE(std::isinf(c1));
    //     ASSERT_FALSE(std::isnan(c0));
    //     ASSERT_FALSE(std::isnan(c1));
    //     EXPECT_LE(std::abs(c0-1.0), 0.00000001);
    //     EXPECT_LE(std::abs(c1-1.0), 0.00000001);
    // }


    TEST(OptimalTermTreatmentsTest, Analyzer)
    {
        const double c_density = 0.1;
        const double c_signal = 0.00125893;
        std::vector<int> rows = {2, 0, 0, 0, 0, 1};
        auto metrics0 = AnalyzeAlternate(rows, c_density, c_signal);
        size_t rowConfig = SizeTFromRowVector(rows);
        auto metrics1 = Analyze(rowConfig, c_density, c_signal, false);

        double c0 = metrics0.GetQuadwords();
        double c1 = metrics1.second.GetQuadwords();


        // // Manual computation for AnalyzeAlternate formulation.
        // const double c_debug_signal = Term::FrequencyAtRank(c_signal, 5);
        // double initialNoise = c_density - c_debug_signal;
        // double noiseAtZero = c_density - c_signal;

        // double rankDownSignalDelta = c_debug_signal - c_signal;
        // double noiseAfterR5R0 = (rankDownSignalDelta + initialNoise) * noiseAtZero;
        // double noiseAfterR5R0R0 = noiseAfterR5R0 * noiseAtZero;

        // double initialDensity = c_density;
        // double densityAfterR5R0 = c_signal + noiseAfterR5R0;
        // double densityAfterR5R0R0 = c_signal + noiseAfterR5R0R0;

        // double initialPNonZero = ProbNotZero(initialDensity);
        // double PNonZeroAfterR5R0 = ProbNotZero(densityAfterR5R0);
        // double PNonZeroAfterR5R0R0 = ProbNotZero(densityAfterR5R0R0);

        // std::cout << "==============================" << std::endl;
        // std::cout << "signal:debug_signal " << c_signal << ":" << c_debug_signal << std::endl;
        // std::cout << "qword0:qword1 " << c0 << ":" << c1 << std::endl;
        // std::cout << "noise0:noise1:noise2 "
        //           << initialNoise << ":"
        //           << noiseAfterR5R0 << ":"
        //           << noiseAfterR5R0R0 << std::endl;

        // std::cout << "density0:density1:density2 "
        //           << initialDensity << ":"
        //           << densityAfterR5R0 << ":"
        //           << densityAfterR5R0R0 << std::endl;

        // std::cout << "P0:P1:P2 "
        //           << initialPNonZero << ":"
        //           << PNonZeroAfterR5R0 << ":"
        //           << PNonZeroAfterR5R0R0 << std::endl;


        // std::cout << "ExpectedQwords: " << (1.0 / (1 << 5))
        //     + initialPNonZero + PNonZeroAfterR5R0 << std::endl;


        ASSERT_FALSE(std::isinf(c0));
        ASSERT_FALSE(std::isinf(c1));
        ASSERT_FALSE(std::isnan(c0));
        ASSERT_FALSE(std::isnan(c1));
        ASSERT_LE(std::abs(c0-c1), 0.00000001);
    }


    TEST(OptimalTermTreatmentsTest, Analyze)
    {
        // double density = 0.1;
        // double snr = 10;

        // std::cout << "================================================================ Rank0" << std::endl;
        // TreatmentPrivateSharedRank0 t0(density, snr);
        // AnalyzeTermTreatment(t0, density);

        // std::cout << "================================================================ Rank0And3" << std::endl;
        // TreatmentPrivateSharedRank0And3 t1(density, snr);
        // AnalyzeTermTreatment(t1, density);

        // std::cout << "================================================================ Experimental" << std::endl;
        // TreatmentExperimental t2(density, snr);
        // AnalyzeTermTreatment(t2, density);

        // std::cout << "================================================================ Optimal" << std::endl;
        // TreatmentOptimal t3(density, snr);
        // AnalyzeTermTreatment(t3, density);
    }
}
