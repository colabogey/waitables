/* Copyright 2022-Present Steve Yackey

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <functional>
#include <sys/eventfd.h>
#include <waitable_event.h>
#include <waitable_thread.h>
#include <eventIds.h>
#include <wfmo.h>
#include <wfmoReturnVal.h>

using namespace std;

class NetworkTestClass {
public:
    NetworkTestClass() = default;
    virtual ~NetworkTestClass() = default;

};

// The fixture for testing
class NetworkTest : public ::testing::Test {

  protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    NetworkTest() {
        // You can do set-up work for each test here.
    }

    virtual ~NetworkTest() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
        //
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Objects declared here can be used by all tests.
    //
};

TEST_F(NetworkTest, NetworkTestPlaceHolder) {
    // arrange
    // act
    // assert
}

