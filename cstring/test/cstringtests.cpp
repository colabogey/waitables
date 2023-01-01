/*
 * MutexTests.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: yackey
 */

#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstring.h>

using namespace std;

// The fixture for testing 
class CstringTest : public ::testing::Test
{

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	CstringTest()
	{
		// You can do set-up work for each test here.
	}

	virtual ~CstringTest() 
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
	}

	// Objects declared here can be used by all tests.

};

TEST_F(CstringTest, DefaultConstructor_Size_Is_Zero)
{
	// arrange
    cstring cs;
	// act
	int cs_size = cs.size();
	// assert
	ASSERT_EQ(0, cs_size);
}

TEST_F(CstringTest, ConstructorFromStdString_Size_Check)
{
	// arrange
    std::string str = "12345";
    cstring cs(str);
	// act
	int cs_size = cs.size();
	int str_size = str.size();
	// assert
	ASSERT_EQ(str_size, cs_size);
}

TEST_F(CstringTest, ConstructorFromStdString_Data_Check)
{
	// arrange
    std::string str = "12345";
    cstring cs(str);
	// act
	// assert
	ASSERT_STREQ(str.c_str(), cs.data());
}

TEST_F(CstringTest, ConstructorFromCharArray_Size_Check)
{
	// arrange
    char buf[128] = {'\0'};
    strcat(buf, "12345");
    cstring cs(buf);
	// act
	int cs_size = cs.size();
	int str_size = strlen(buf);
	// assert
	// ASSERT_STREQ(s1.c_str(), s2.c_str());
	ASSERT_EQ(str_size, cs_size);
}

TEST_F(CstringTest, ConstructorFromCharArray_Data_Check)
{
	// arrange
    char buf[128] = {'\0'};
    strcat(buf, "12345");
	// act
    cstring cs(buf);
	// assert
	ASSERT_STREQ(buf, cs.data());
}

TEST_F(CstringTest, CopyConstructor_Size_Check)
{
	// arrange
    std::string str = "12345";
    cstring cs1(str);
	// act
    cstring cs2(cs1);
	// assert
	ASSERT_EQ(cs1.size(), cs2.size());
}

TEST_F(CstringTest, CopyConstructor_Data_Check)
{
	// arrange
    std::string str = "12345";
    cstring cs1(str);
	// act
    cstring cs2(cs1);
	// assert
	ASSERT_STREQ(cs1.data(), cs2.data());
}

TEST_F(CstringTest, OperatorEqual_Size_Check)
{
	// arrange
    std::string str = "12345";
    cstring cs1(str);
    cstring cs2;
	// act
    cs2 = cs1;
	// assert
	ASSERT_EQ(cs1.size(), cs2.size());
}

TEST_F(CstringTest, OperatorEqual_Data_Check)
{
	// arrange
    std::string str = "12345";
    cstring cs1(str);
    cstring cs2;
	// act
    cs2 = cs1;
	// assert
	ASSERT_STREQ(cs1.data(), cs2.data());
}

TEST_F(CstringTest, OperatorPlusEqual_With_Cstring_Size_Check)
{
	// arrange
    std::string str1 = "12345";
    std::string str2 = "abcdfeghi";
    cstring cs1(str1);
    cstring cs2(str2);
	// act
    cs2 += cs1;
	// assert
	int sz = str1.size() + str2.size();
	ASSERT_EQ(sz, cs2.size());
}

TEST_F(CstringTest, OperatorPlusEqual_With_Cstring_Data_Check)
{
	// arrange
    std::string str1 = "12345";
    std::string str2 = "qrstuvwxyz";
    std::string str3 = str1 + str2;
    cstring cs1(str1);
    cstring cs2(str2);
	// act
    cs1 += cs2;
	// assert
	ASSERT_STREQ(cs1.data(), str3.c_str());
}

TEST_F(CstringTest, OperatorPlusEqual_With_String_Size_Check)
{
	// arrange
    std::string str1 = "12345";
    std::string str2 = "abcdfeghi";
    cstring cs1(str1);
    cstring cs2(str2);
	// act
    cs2 += cs1;
	// assert
	int sz = str1.size() + str2.size();
	ASSERT_EQ(sz, cs2.size());
}

TEST_F(CstringTest, OperatorPlusEqual_With_String_Data_Check)
{
	// arrange
    std::string str1 = "12345";
    std::string str2 = "qrstuvwxyz";
    std::string str3 = str1 + str2;
    cstring cs1(str1);
	// act
    cs1 += str2;
	// assert
	ASSERT_STREQ(cs1.data(), str3.c_str());
}

TEST_F(CstringTest, OperatorPlus_With_Cstring_Size_Check)
{
	// arrange
    std::string str1 = "12345";
    std::string str2 = "abcdfeghi";
    cstring cs1(str1);
    cstring cs2(str2);
    cstring cs3;
	// act
    cs3 = cs1 + cs2;
	// assert
	int sz = str1.size() + str2.size();
	ASSERT_EQ(sz, cs3.size());
}

TEST_F(CstringTest, OperatorPlus_With_Cstring_Data_Check)
{
	// arrange
    std::string str1 = "12345";
    std::string str2 = "qrstuvwxyz";
    std::string str3 = str1 + str2;
    cstring cs1(str1);
    cstring cs2(str2);
    cstring cs3;
	// act
    cs3 = cs1 + cs2;
	// assert
	ASSERT_STREQ(cs3.data(), str3.c_str());
}

TEST_F(CstringTest, OperatorPlus_With_String_Size_Check)
{
	// arrange
    std::string str1 = "12345";
    std::string str2 = "abcdfeghi";
    cstring cs1(str1);
    cstring cs3;
	// act
    cs3 = cs1 + str2;
	// assert
	int sz = str1.size() + str2.size();
	ASSERT_EQ(sz, cs3.size());
}

TEST_F(CstringTest, OperatorPlus_With_String_Data_Check)
{
	// arrange
    std::string str1 = "12345";
    std::string str2 = "qrstuvwxyz";
    std::string str3 = str1 + str2;
    cstring cs1(str1);
    cstring cs3;
	// act
    cs3 = cs1 + str2;
	// assert
	ASSERT_STREQ(cs3.data(), str3.c_str());
}

TEST_F(CstringTest, Format_Size_Check)
{
	// arrange
    char buf[128] = {'\0'};
    sprintf(buf, "my dog has (%d) fleas", 47);
    cstring cs1;
	// act
    cs1.format("my dog has (%d) fleas", 47);
	// assert
	ASSERT_EQ(cs1.size(), strlen(buf));
}

TEST_F(CstringTest, Format_Data_Check)
{
	// arrange
    char buf[128] = {'\0'};
    sprintf(buf, "my dog has (%d) fleas", 47);
    cstring cs1;
	// act
    cs1.format("my dog has (%d) fleas", 47);
	// assert
	ASSERT_STREQ(cs1.data(), buf);
}
