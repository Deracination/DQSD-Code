#include "stdafx.h"
#include "..\FormElement.h"

class FormElementTests : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(FormElementTests);
  CPPUNIT_TEST(testConstructFromValidData);
  CPPUNIT_TEST(testTagAndTypeForcedLowerCase);
  CPPUNIT_TEST(testCasePreservedForRemainingAttributes);
  CPPUNIT_TEST(testCheckedDefaultsToFalse);
  CPPUNIT_TEST(testCheckedCanBeOverridden);
  CPPUNIT_TEST_SUITE_END();

  void testConstructFromValidData()
  {
    FormElement element("tag", "type", "name", "value");
    CPPUNIT_ASSERT_EQUAL(std::string("tag"), element.Tag());
    CPPUNIT_ASSERT_EQUAL(std::string("type"), element.Type());
    CPPUNIT_ASSERT_EQUAL(std::string("name"), element.Name());
    CPPUNIT_ASSERT_EQUAL(std::string("value"), element.Value());
  }

  void testTagAndTypeForcedLowerCase()
  {
    FormElement element("INPUT", "CheckBox", "name", "value");
    CPPUNIT_ASSERT_EQUAL(std::string("input"), element.Tag());
    CPPUNIT_ASSERT_EQUAL(std::string("checkbox"), element.Type());
  }

  void testCasePreservedForRemainingAttributes()
  {
    FormElement element("input", "checkbox", "Name", "VaLue");
    CPPUNIT_ASSERT_EQUAL(std::string("Name"), element.Name());
    CPPUNIT_ASSERT_EQUAL(std::string("VaLue"), element.Value());
  }

  void testCheckedDefaultsToFalse()
  {
    FormElement element("input", "checkbox", "Name", "VaLue");
    CPPUNIT_ASSERT(!element.Checked());
  }

  void testCheckedCanBeOverridden()
  {
    FormElement element("input", "checkbox", "Name", "VaLue", true);
    CPPUNIT_ASSERT(element.Checked());
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(FormElementTests);
