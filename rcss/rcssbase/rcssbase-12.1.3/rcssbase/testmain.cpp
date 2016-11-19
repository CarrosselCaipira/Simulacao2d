//=============================================================================
/** 
 * \brief XXX: Add brief comment here.
 * *
 * XXX: Add detailed comments here.
 */
//=============================================================================

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include <fstream>

void
printResults( CPPUNIT_NS::TestResultCollector& result, const char* file )
{
	std::fstream ofile( file, std::fstream::out );
	CPPUNIT_NS::XmlOutputter xmloutputter( &result, ofile );
	xmloutputter.write();
	ofile.close();
}

int
main(int argc, char** argv)
{
	/// Create the event manager and test controller
	CPPUNIT_NS::TestResult controller;

	/// Add a listener that colllects test result
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );

	/// Add a listener that print dots as test run.
	CPPUNIT_NS::BriefTestProgressListener progress;
	controller.addListener( &progress );

	/// Add the registered tests to the test runner
	CPPUNIT_NS::TestRunner runner;
	runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );
	runner.run( controller );

	//Print test to xml file
	if( argc > 1 )
	{
		printResults( result, argv[ 1 ] );
	}
	/// Print test in a compiler compatible format.
	CPPUNIT_NS::CompilerOutputter outputter( &result, std::cerr );
	outputter.write();

	return result.wasSuccessful() ? 0 : 1;  
}
