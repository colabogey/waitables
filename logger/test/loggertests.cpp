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

/*
 * MutexTests.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: yackey
 */

#include "logger_interface.h"
#include "loggerConfig.h"
#include "vpgnixlogger.h"
#include <cstdint>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include "cstring.h"
#include <unistd.h>
#include <memory>

using namespace std;

// The fixture for testing
class LoggerTest : public ::testing::Test {

  protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    LoggerTest() : m_logdir("/tmp") {
        // You can do set-up work for each test here.
    }

    virtual ~LoggerTest() {}

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
        // delete m_w;
    }

    // Objects declared here can be used by all tests.

    string m_logdir;
    //
};

TEST_F(LoggerTest, UseLogMethod_FromBase_ExpectLogEntries) {
    // arrange
    std::shared_ptr<loggerConfig> lc = std::make_shared<loggerConfig>();
    std::unique_ptr<vpgnixlogger> l = std::make_unique<vpgnixlogger>();
    l->Initialize(lc);
    // act
    for(int i = 0; i < 3; i++) {
        l->Log(LOGLEVEL_INFO, "wtf");
        usleep(2345);
        l->Log(LOGLEVEL_INFO, "wtf %x", i);
    }
    // assert
    string s1 = "zzz";
    string s2 = "zzz";
    ASSERT_STREQ(s1.c_str(), s2.c_str());
    l->Uninitialize();
}

TEST_F(LoggerTest, UseLogMethod_FromBase_Using_cstring_ExpectLogEntries) {
    // arrange
    std::shared_ptr<loggerConfig> lc = std::make_shared<loggerConfig>();
    std::unique_ptr<vpgnixlogger> l = std::make_unique<vpgnixlogger>();
    l->Initialize(lc);
    // act
    cstring c;
    c.format("%s", "wtf");
    l->Log(LOGLEVEL_INFO, c);

    cstring c3("xyz");
    l->Log(LOGLEVEL_INFO, c3);

    for(int i = 0; i < 3; i++) {
        cstring c2;
        c2.format("%s (%d)", "wtf", i);
        l->Log(LOGLEVEL_INFO, c2);
    }
    // assert
    string s1 = "zzz";
    string s2 = "zzz";
    ASSERT_STREQ(s1.c_str(), s2.c_str());
    l->Uninitialize();
}

TEST_F(LoggerTest, UseLogMethod_FromIf_ExpectLogEntries) {
    // arrange
    std::shared_ptr<loggerConfig> lc = std::make_shared<loggerConfig>();
    std::unique_ptr<vpgnixlogger> l = std::make_unique<vpgnixlogger>();
    l->Initialize(lc);
    for(int i = 0; i < 3; i++) {
        l->Log(LOGLEVEL_DEBUG, "this is what");
        usleep(1234);
        l->Log(LOGLEVEL_DEBUG, "this too %x", i);
    }
    // assert
    string s1 = "zzz";
    string s2 = "zzz";
    ASSERT_STREQ(s1.c_str(), s2.c_str());
    l->Uninitialize();
}
