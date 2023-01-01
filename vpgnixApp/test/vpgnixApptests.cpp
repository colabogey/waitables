/*
 * MutexTests.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: yackey
 */

#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vpgnix.h>
#include <runnableApp_interface.h>
#include <vpgnixlogger.h>
#include <vpgnixApp.h>
#include <wfmo.h>

using namespace std;

// The fixture for testing 
class VpgnixAppTest : public ::testing::Test
{

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	VpgnixAppTest()
	{
		// You can do set-up work for each test here.
	}

	virtual ~VpgnixAppTest() 
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

TEST_F(VpgnixAppTest, AintGotNoStinkingTests)
{
	// arrange
    auto sp_logger = std::shared_ptr<ILogger>(new vpgnixlogger);
    sp_logger->Initialize("/tmp");
    auto sp_App = std::shared_ptr<IRunnableApp>(new vpgnixApp(sp_logger));
	// act
	std::string s1 = "zz";
	std::string s2 = "zz";
	// assert
	ASSERT_STREQ(s1.c_str(), s2.c_str());
}

