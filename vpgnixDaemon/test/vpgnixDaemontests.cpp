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

#include <cstdint>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// The fixture for testing 
class VpgnixDaemonTest : public ::testing::Test
{

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	VpgnixDaemonTest()
	{
		// You can do set-up work for each test here.
	}

	virtual ~VpgnixDaemonTest() 
	{
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() 
	{
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

};

TEST_F(VpgnixDaemonTest, AintGotNoStinkingTests)
{
	// arrange
	// act
	std::string s1= "zz";
	std::string s2= "zz";
	// assert
	ASSERT_STREQ(s1.c_str(), s2.c_str());
}

