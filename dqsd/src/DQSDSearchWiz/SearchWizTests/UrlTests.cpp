#include "stdafx.h"

#include "..\Url.h"

class UrlTests : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(UrlTests);
    CPPUNIT_TEST(testParseValid);
    CPPUNIT_TEST(testParseNonStandardPort);
    CPPUNIT_TEST(testEndPoint);
    CPPUNIT_TEST(testDirectoryOfBlankPath);
    CPPUNIT_TEST(testDirectoryOfDirectoryPath);
    CPPUNIT_TEST(testDirectoryOfFullPath);
  CPPUNIT_TEST_SUITE_END();

  void testParseValid()
  {
    Url url = Url::Parse("http://www.example.com/foo/page.html");

    CPPUNIT_ASSERT_EQUAL(std::string("http"), url.Scheme());
    CPPUNIT_ASSERT_EQUAL(std::string("www.example.com"), url.Host());
    CPPUNIT_ASSERT_EQUAL(std::string("/foo/page.html"), url.Path());
    CPPUNIT_ASSERT_EQUAL(0u, url.Port());
  }

  void testParseNonStandardPort()
  {
    Url url = Url::Parse("http://www.example.com:77/foo/page.html");

    CPPUNIT_ASSERT_EQUAL(std::string("http"), url.Scheme());
    CPPUNIT_ASSERT_EQUAL(77u, url.Port());
    CPPUNIT_ASSERT_EQUAL(std::string("www.example.com"), url.Host());
    CPPUNIT_ASSERT_EQUAL(std::string("/foo/page.html"), url.Path());
  }

  void testEndPoint()
  {
    Url noport = Url::Parse("http://www.example.com/foo/page.html");
    CPPUNIT_ASSERT_EQUAL(std::string("www.example.com"), noport.EndPoint());

    Url port = Url::Parse("http://www.example.com:8080/foo/page.html");
    CPPUNIT_ASSERT_EQUAL(std::string("www.example.com:8080"), port.EndPoint());
  }

  void testDirectoryOfBlankPath()
  {
    Url url = Url::Parse("http://www.example.com");
    CPPUNIT_ASSERT_EQUAL(std::string("/"), url.Directory());
  }

  void testDirectoryOfDirectoryPath()
  {
    Url url = Url::Parse("http://www.example.com/fuzz/boot/");
    CPPUNIT_ASSERT_EQUAL(std::string("/fuzz/boot/"), url.Directory());
  }

  void testDirectoryOfFullPath()
  {
    Url url = Url::Parse("http://www.example.com/foo/bar/page.html");
    CPPUNIT_ASSERT_EQUAL(std::string("/foo/bar/"), url.Directory());
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(UrlTests);
