#include "stdafx.h"

#include "..\Form.h"

class FormTests : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(FormTests);
    CPPUNIT_TEST(testConstructFromValidData);
    CPPUNIT_TEST(testConstructWithInvalidMethod);
    CPPUNIT_TEST(testGetAbsoluteActionPath);
    CPPUNIT_TEST(testGetAbsoluteActionPathWithAbsoluteAction);
    CPPUNIT_TEST(testGetAbsoluteActionPathWithEmptyAction);
    CPPUNIT_TEST(testGetAbsoluteActionPathWithExcitingBaseUrl);
    CPPUNIT_TEST(testGetAbsoluteActionPathWithNonRootedActionAndPathLessBaseUrl);
    CPPUNIT_TEST(testEchoMembers);
  CPPUNIT_TEST_SUITE_END();

  void testConstructFromValidData()
  {
    Form get("name", "get", "action");
    Form post("name", "POST", "action");
    CPPUNIT_ASSERT(get.IsMethodValid());
    CPPUNIT_ASSERT(post.IsMethodValid());
  }

  void testConstructWithInvalidMethod()
  {
    Form form("name", "some random method", "action");
    CPPUNIT_ASSERT(!form.IsMethodValid());
  }

  void testGetAbsoluteActionPath()
  {
    Form form("name", "get", "/search");
    std::string actual = form.GetAbsoluteActionPath("http://www.example.com");

    const std::string expected = "http://www.example.com/search";
    CPPUNIT_ASSERT_EQUAL(expected, actual);
  }

  void testGetAbsoluteActionPathWithNonRootedActionAndPathLessBaseUrl()
  {
    Form form("name", "get", "search");
    std::string actual = form.GetAbsoluteActionPath("http://www.example.com");

    const std::string expected = "http://www.example.com/search";
    CPPUNIT_ASSERT_EQUAL(expected, actual);
  }

  void testGetAbsoluteActionPathWithAbsoluteAction()
  {
    const std::string action = "http://www.example.com/target";

    Form form("name", "get", action);
    std::string actual = form.GetAbsoluteActionPath("http://www.example.com/");

    CPPUNIT_ASSERT_EQUAL(action, actual);
  }

  void testGetAbsoluteActionPathWithEmptyAction()
  {
    const std::string baseUrl = "http://www.example.com/";
    Form form("name", "get", "");
    std::string actual = form.GetAbsoluteActionPath(baseUrl);

    CPPUNIT_ASSERT_EQUAL(baseUrl, actual);
  }

  void testGetAbsoluteActionPathWithExcitingBaseUrl()
  {
    const std::string baseUrl = "http://www.example.com/exciting/page.html";
    Form form("name", "get", "posttarget.html");
    std::string actual = form.GetAbsoluteActionPath(baseUrl);

    const std::string expected = "http://www.example.com/exciting/posttarget.html";

    CPPUNIT_ASSERT_EQUAL(expected, actual);
  }

  void testEchoMembers()
  {
    Form form("name", "get", "/action");

    CPPUNIT_ASSERT_EQUAL(std::string("name"), form.Name());
    CPPUNIT_ASSERT_EQUAL(std::string("get"), form.Method());
    CPPUNIT_ASSERT_EQUAL(std::string("/action"), form.Action());

  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(FormTests);