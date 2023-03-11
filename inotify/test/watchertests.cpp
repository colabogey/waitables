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
#include "watcher.h"
#include "watcherobserver.h"

using namespace std;

void watcherobserver::Update(const std::string& m)
	{ m_the_update_message = m; } 

// The fixture for testing 
class WatcherTest : public ::testing::Test
{

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	WatcherTest()
		: m_dirname("/tmp")
		, m_filename("z")
	{
		// You can do set-up work for each test here.
	}

	virtual ~WatcherTest() 
	{
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void modifyfile()
	{
		sprintf(m_cmd, "/usr/bin/echo zzz >> %s/%s", 
			m_dirname.c_str(), m_filename.c_str());
		system(m_cmd);
	}

	virtual void removefile()
	{
		sprintf(m_cmd, "/bin/rm %s/%s", m_dirname.c_str(), m_filename.c_str());
		system(m_cmd);
	}

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

	string	m_dirname;
	string	m_filename;
	watcher*	m_w;
	watcherobserver* m_observer;
	char m_cmd[1024];

	//
};

TEST_F(WatcherTest, ChangeExistingFile_GetUpdate)
{
	// arrange
	int key = -1;
	modifyfile();
	m_w = new watcher(m_dirname, m_filename);
	m_observer = new watcherobserver();
	key = m_w->attach(m_observer);
	m_w->watch();
	// act
	modifyfile();
	string s1= m_filename;
	string s2= m_observer->getmessage();
	// assert
	ASSERT_STREQ(s1.c_str(), s2.c_str());
	m_w->detach(key);
	m_w->unwatch();
	delete m_w;
	delete m_observer;
}

TEST_F(WatcherTest, ChangeExistingFile_GetUpdate_WithKeyEqual1)
{
	// arrange
	int key = -1;
	modifyfile();
	m_w = new watcher(m_dirname, m_filename);
	m_observer = new watcherobserver();
	key = m_w->attach(m_observer);
	m_w->watch();
	// act
	modifyfile();
	string s1= m_filename;
	string s2= m_observer->getmessage();
	// assert
	ASSERT_EQ(1, key);
	m_w->detach(key);
	m_w->unwatch();
	delete m_w;
	delete m_observer;
}

TEST_F(WatcherTest, ChangeExistingFile_GetUpdate_DetachOneKey)
{
	// arrange
	int key = -1;
	modifyfile();
	m_w = new watcher(m_dirname, m_filename);
	m_observer = new watcherobserver();
	key = m_w->attach(m_observer);
	m_w->watch();
	// act
	modifyfile();
	string s1= m_filename;
	string s2= m_observer->getmessage();
	// assert
	int num_detached = m_w->detach(key);
	ASSERT_EQ(1, num_detached);
	m_w->unwatch();
	delete m_w;
	delete m_observer;
}

TEST_F(WatcherTest, CreateFileAfterWatch_GetUpdate)
{
	// arrange
	modifyfile();
	removefile();
	m_w = new watcher(m_dirname, m_filename);
	m_observer = new watcherobserver();
	m_w->attach(m_observer);
	m_w->watch();
	// act
	modifyfile();
	modifyfile();
	string s1= m_filename;
	string s2= m_observer->getmessage();
	// assert
	ASSERT_STREQ(s1.c_str(), s2.c_str());
	m_w->unwatch();
	delete m_w;
	delete m_observer;
}

TEST_F(WatcherTest, FileDoesNotExist_NoNotification)
{
	// arrange
	modifyfile();
	removefile();
	m_w = new watcher(m_dirname, m_filename);
	m_observer = new watcherobserver();
	m_w->attach(m_observer);
	m_w->watch();
	// act
	string s1= "";
	string s2= m_observer->getmessage();
	// assert
	ASSERT_STREQ(s1.c_str(), s2.c_str());
	m_w->unwatch();
	delete m_w;
	delete m_observer;
}

