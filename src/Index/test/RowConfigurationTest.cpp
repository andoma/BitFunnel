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

// For std::equal() on Windows.
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include <algorithm>        // std::equal
#include <deque>

#include "BitFunnel/Exceptions.h"
#include "gtest/gtest.h"
#include "TermTreatments.h"


namespace BitFunnel
{
    namespace RowConfigurationTest
    {
        // Create RowConfiguration::Entry and get fields.
        TEST(Entry, Construct)
        {
            RowConfiguration::Entry e1(1u, 2u);
            EXPECT_EQ(e1.GetRank(), 1u);
            EXPECT_EQ(e1.GetRowCount(), 2u);

            RowConfiguration::Entry e(6u, 5u);
            EXPECT_EQ(e.GetRank(), 6u);
            EXPECT_EQ(e.GetRowCount(), 5u);
        }


        // Throw on invalid RowConfiguration::Entry.
        TEST(Entry, Throw)
        {
            // Rank is too large.
            ASSERT_THROW(RowConfiguration::Entry(c_maxRankValue + 1, 1u),
                         RecoverableError);

            // RowCount is too large.
            ASSERT_THROW(RowConfiguration::Entry(0u,
                                                 RowConfiguration::Entry::c_maxRowCount + 1),
                         RecoverableError);

            // RowCount is zero.
            ASSERT_THROW(RowConfiguration::Entry(1u, 0u),
                         RecoverableError);
        }


        // Throw on iterate empty.
        TEST(RowConfiguration, ThrowOnIterateEmpty)
        {
            RowConfiguration c;
            auto it = c.begin();

            // Shouldn't be able to dereference iterator to empty RowConfiguration.
            ASSERT_THROW(*it, RecoverableError);

            // Shouldn't be able to advance iterator to empty RowConfiguration.
            ASSERT_THROW(++it, RecoverableError);
        }


        // Throw on duplicate rank
        TEST(RowConfiguration, Iterate)
        {
            std::deque<RowConfiguration::Entry> expected;
            RowConfiguration observed;

            // push_front, iterate, and verify contents for up to c_maxRankValue + 1 entries.
            for (unsigned i = 0; i <= c_maxRankValue ; ++i)
            {
                RowConfiguration::Entry e(i, i + 1);
                expected.push_front(e);
                observed.push_front(e);

                EXPECT_TRUE(std::equal(observed.begin(), observed.end(), expected.begin()));
                EXPECT_TRUE(std::equal(expected.begin(), expected.end(), observed.begin()));
            }

            // Ensure 9th entry generates an exception, now that RowConfiguration is full.
            RowConfiguration::Entry e2(1, 2);
            ASSERT_THROW(observed.push_front(e2), RecoverableError);
        }


        TEST(RowConfiguration, ThrowOnDuplicateRank)
        {
            RowConfiguration c;
            c.push_front(RowConfiguration::Entry(1, 2));

            // Adding a second entry with the same rank should trigger an exception.
            ASSERT_THROW(c.push_front(RowConfiguration::Entry(1, 3)),
                         RecoverableError);
        }
    }
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif
