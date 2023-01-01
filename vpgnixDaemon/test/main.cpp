#include <stdio.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

int main (int argc, char** argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
