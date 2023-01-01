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

class WaitableTestClass {
public:
    WaitableTestClass() = default;
    virtual ~WaitableTestClass() = default;

    void runWaitableThreadSignalThreadExit(int fd, std::string& myThreadName) {
        m_pwt->signalthreadexit();
    };

    void runWaitableEventSetReset(int fd, std::string& myThreadName) {
        m_pwe->set_event();
        m_pwt->signalthreadexit();
    };

    void runWaitForAnyObject_1(int fd, std::string& myThreadName) {
        m_pwe_1->set_event();
        m_pwt_1->signalthreadexit();
    };

    void runWaitForAnyObject_2(int fd, std::string& myThreadName) {
        m_pwe_2->set_event();
        m_pwt_2->signalthreadexit();
    };

    void runWaitForAnyObject_signaler(int fd, std::string& myThreadName) {
        m_pwe_signaler->set_event();
        m_pwe_1->reset_event();
        m_pwe_2->reset_event();
        m_pwt_signaler->signalthreadexit();
    };

    std::unique_ptr<waitable_thread> m_pwt;
    std::unique_ptr<waitable_event> m_pwe;
    std::unique_ptr<waitable_thread> m_pwt_1;
    std::unique_ptr<waitable_event> m_pwe_1;
    std::unique_ptr<waitable_thread> m_pwt_2;
    std::unique_ptr<waitable_event> m_pwe_2;
    std::unique_ptr<waitable_thread> m_pwt_signaler;
    std::unique_ptr<waitable_event> m_pwe_signaler;
};

// The fixture for testing
class WaitableTest : public ::testing::Test {

  protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    WaitableTest() {
        // You can do set-up work for each test here.
    }

    virtual ~WaitableTest() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
        //
        m_wtc = std::make_shared<WaitableTestClass>();
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Objects declared here can be used by all tests.
    //
    std::shared_ptr<WaitableTestClass> m_wtc;

};

TEST_F(WaitableTest, WaitableThreadGetName) {
    // arrange
    uint64_t u = 6;
    std::string myThreadName = "WaitableThreadGetName";
    waitableThreadFunc_t f = 
                std::bind(&WaitableTestClass::runWaitableThreadSignalThreadExit, 
                m_wtc, 
                std::placeholders::_1, std::placeholders::_2);
    m_wtc->m_pwt = std::make_unique<waitable_thread>(myThreadName, 0, f);
    // act
    std::string name = m_wtc->m_pwt->getName();
    // assert
    ASSERT_STREQ(myThreadName.c_str(), name.c_str());
}

TEST_F(WaitableTest, WaitableThreadSignalThreadExit) {
    // arrange
    uint64_t u = 6;
    std::string myThreadName = "WaitableThreadSignalThreadExit";
    waitableThreadFunc_t f = 
                std::bind(&WaitableTestClass::runWaitableThreadSignalThreadExit, 
                m_wtc, 
                std::placeholders::_1, std::placeholders::_2);
    m_wtc->m_pwt = std::make_unique<waitable_thread>(myThreadName, 0, f);
    // act
    m_wtc->m_pwt->func();
    ssize_t s = read(m_wtc->m_pwt->getFd(), &u, sizeof(uint64_t));
    // assert
    ASSERT_EQ(u, 1);
}

TEST_F(WaitableTest, WaitableEventSetReset) {
    // arrange
    uint64_t u = 6;
    int id = 12;
    m_wtc->m_pwe = std::make_unique<waitable_event>();
    m_wtc->m_pwe->create_event(id);
    std::string myThreadName = "WaitableEventSetReset";
    waitableThreadFunc_t f = 
                std::bind(&WaitableTestClass::runWaitableEventSetReset, 
                m_wtc, 
                std::placeholders::_1, std::placeholders::_2);
    m_wtc->m_pwt = std::make_unique<waitable_thread>(myThreadName, 0, f);
    // act
    m_wtc->m_pwt->func();
    m_wtc->m_pwe->reset_event();
    ssize_t s = read(m_wtc->m_pwt->getFd(), &u, sizeof(uint64_t));
    // if the thread returns, its goodness, but we'll check the thread fd for fun
    // assert
    ASSERT_EQ(u, 1);
}

TEST_F(WaitableTest, WaitableEventGetId) {
    // arrange
    int id = 12;
    // act
    m_wtc->m_pwe = std::make_unique<waitable_event>();
    m_wtc->m_pwe->create_event(id);
    // assert
    ASSERT_EQ(id, m_wtc->m_pwe->getId());
}

TEST_F(WaitableTest, WaitableEventGetFd) {
    // arrange
    int id = 12;
    int fd = -1;
    // act
    m_wtc->m_pwe = std::make_unique<waitable_event>();
    m_wtc->m_pwe->create_event(id);
    // assert
    ASSERT_NE(fd, m_wtc->m_pwe->getFd());
}

TEST_F(WaitableTest, WaitForAnyObject) {
    //arrange
    uint64_t u = 6;
    m_wtc->m_pwe_1 = std::make_unique<waitable_event>();
    m_wtc->m_pwe_1->create_event(1100);
    std::string myThreadName_1 = "runWaitForAnyObject_1";
    waitableThreadFunc_t f_1 = 
                std::bind(&WaitableTestClass::runWaitForAnyObject_1, 
                m_wtc, 
                std::placeholders::_1, std::placeholders::_2);
    m_wtc->m_pwt_1 = std::make_unique<waitable_thread>(myThreadName_1, 1111, f_1);
    //
    m_wtc->m_pwe_2 = std::make_unique<waitable_event>();
    m_wtc->m_pwe_2->create_event(2200);
    std::string myThreadName_2 = "runWaitForAnyObject_2";
    waitableThreadFunc_t f_2 = 
                std::bind(&WaitableTestClass::runWaitForAnyObject_2, 
                m_wtc, 
                std::placeholders::_1, std::placeholders::_2);
    m_wtc->m_pwt_2 = std::make_unique<waitable_thread>(myThreadName_2, 2222, f_2);
    //
    m_wtc->m_pwe_signaler = std::make_unique<waitable_event>();
    m_wtc->m_pwe_signaler->create_event(3300);
    std::string myThreadName_signaler = "runWaitForAnyObject_signaler";
    waitableThreadFunc_t f_signaler = 
                std::bind(&WaitableTestClass::runWaitForAnyObject_signaler, 
                m_wtc, 
                std::placeholders::_1, std::placeholders::_2);
    m_wtc->m_pwt_signaler = std::make_unique<waitable_thread>(myThreadName_signaler, 3333, f_signaler);
    // act
    waitableMap_t myWaitableMap;
    myWaitableMap.insert(std::pair<int, int>(m_wtc->m_pwt_1->getFd(), m_wtc->m_pwt_1->getId()));
    myWaitableMap.insert(std::pair<int, int>(m_wtc->m_pwt_2->getFd(), m_wtc->m_pwt_2->getId()));
    auto my_wfmo = std::make_shared<wfmo>(myWaitableMap);
    my_wfmo->setup();
    m_wtc->m_pwt_1->func();
    m_wtc->m_pwt_2->func();
    m_wtc->m_pwt_signaler->func();
    m_wtc->m_pwe_signaler->reset_event();
    bool brunning = true;
    int n = 0;
    while(brunning) {
        wfmoReturnVal_t rv = my_wfmo->waitforobject(5000);
        if (rv.m_id == m_wtc->m_pwt_1->getId()) {
            my_wfmo->clearlastevent(rv.m_fd);
            n++;
            brunning = false;
        } else if (rv.m_id == m_wtc->m_pwt_2->getId()) {
            my_wfmo->clearlastevent(rv.m_fd);
            n++;
            brunning = false;
        } else if (rv.m_id == commonEventIds::timeoutEventId) {
            n = 0;
            brunning = false;
        }
    }
    my_wfmo->teardown();
    // assert
    ASSERT_TRUE( n == 1);
}

TEST_F(WaitableTest, WaitForAll) {
    //arrange
    uint64_t u = 6;
    m_wtc->m_pwe_1 = std::make_unique<waitable_event>();
    m_wtc->m_pwe_1->create_event(1100);
    std::string myThreadName_1 = "runWaitForAnyObject_1";
    waitableThreadFunc_t f_1 = 
                std::bind(&WaitableTestClass::runWaitForAnyObject_1, 
                m_wtc, 
                std::placeholders::_1, std::placeholders::_2);
    m_wtc->m_pwt_1 = std::make_unique<waitable_thread>(myThreadName_1, 1111, f_1);
    //
    m_wtc->m_pwe_2 = std::make_unique<waitable_event>();
    m_wtc->m_pwe_2->create_event(2200);
    std::string myThreadName_2 = "runWaitForAnyObject_2";
    waitableThreadFunc_t f_2 = 
                std::bind(&WaitableTestClass::runWaitForAnyObject_2, 
                m_wtc, 
                std::placeholders::_1, std::placeholders::_2);
    m_wtc->m_pwt_2 = std::make_unique<waitable_thread>(myThreadName_2, 2222, f_2);
    //
    m_wtc->m_pwe_signaler = std::make_unique<waitable_event>();
    m_wtc->m_pwe_signaler->create_event(3300);
    std::string myThreadName_signaler = "runWaitForAnyObject_signaler";
    waitableThreadFunc_t f_signaler = 
                std::bind(&WaitableTestClass::runWaitForAnyObject_signaler, 
                m_wtc, 
                std::placeholders::_1, std::placeholders::_2);
    m_wtc->m_pwt_signaler = std::make_unique<waitable_thread>(myThreadName_signaler, 3333, f_signaler);
    // act
    waitableMap_t myWaitableMap;
    myWaitableMap.insert(std::pair<int, int>(m_wtc->m_pwt_1->getFd(), m_wtc->m_pwt_1->getId()));
    myWaitableMap.insert(std::pair<int, int>(m_wtc->m_pwt_2->getFd(), m_wtc->m_pwt_2->getId()));
    auto my_wfmo = std::make_shared<wfmo>(myWaitableMap);
    my_wfmo->setup();
    m_wtc->m_pwt_1->func();
    m_wtc->m_pwt_2->func();
    m_wtc->m_pwt_signaler->func();
    m_wtc->m_pwe_signaler->reset_event();
    bool brunning = true;
    bool bgotall = false;
    while(brunning) {
        wfmoReturnVal_t rv = my_wfmo->waitforobject(5000, true);
        if (rv.m_id == commonEventIds::waitForAllEventId) {
            bgotall = true;
            brunning = false;
        } else if (rv.m_id == commonEventIds::timeoutEventId) {
            brunning = false;
        }
    }
    my_wfmo->clearAlllastevent();
    my_wfmo->teardown();
    // assert
    ASSERT_TRUE( bgotall );
}

