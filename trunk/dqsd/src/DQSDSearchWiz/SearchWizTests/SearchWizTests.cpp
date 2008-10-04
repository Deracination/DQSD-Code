// SearchWizTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, _TCHAR* argv[])
{
  CPPUNIT_NS::TextUi::TestRunner runner;
  runner.setOutputter(new CPPUNIT_NS::CompilerOutputter(&runner.result(), std::cerr));

  CPPUNIT_NS::Test* suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();
  runner.addTest(suite);

  runner.run();

  return runner.result().testFailuresTotal();
}
