#include "../goap.h"
#include "../astar.h"
#include "CuTest.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

void ap_test_clear(CuTest* tst);
void ap_test_set_basic(CuTest* tst);
void ap_test_set_action_limits(CuTest* test);
void ap_test_set_atom_limits(CuTest* test);

static actionplanner_t ap;

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, ap_test_clear);
	SUITE_ADD_TEST(suite, ap_test_set_basic);
	SUITE_ADD_TEST(suite, ap_test_set_action_limits);
	SUITE_ADD_TEST(suite, ap_test_set_atom_limits);
	
	CuString *output = CuStringNew();
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	return 0;
}

void ap_test_clear(CuTest* tc)
{
	goap_actionplanner_clear(&ap);
	CuAssert(tc, "number of actions is 0", 0 == ap.numactions);
	CuAssert(tc, "number of atoms is 0", 0 == ap.numatoms);
}

void ap_test_set_basic(CuTest* tc)
{
	ap_test_clear(tc);
	
	CuAssert(tc, "able to set precondition", true == goap_set_pre( &ap, "scout", "armedwithgun", true ));
	CuAssert(tc, "pre number of actions is 1", 1 == ap.numactions);
	CuAssert(tc, "pre number of atoms is 1", 1 == ap.numatoms);
	CuAssert(tc, "able to set postconditon", true == goap_set_pst( &ap, "scout", "enemyvisible", true ));
	CuAssert(tc, "post number of actions is 1", 1 == ap.numactions);
	CuAssert(tc, "post number of atoms is 2", 2 == ap.numatoms);

	ap_test_clear(tc);
}

void ap_test_set_action_limits(CuTest* tc)
{
	ap_test_clear(tc);
	
	char buff[20];
	for(int i=0; i<MAXACTIONS; ++i) {
		sprintf(buff, "ACT_%d", i);
		CuAssert(tc, "able to set precondition", true == goap_set_pre( &ap, strdup(buff), "ATOM_0", true ));
	}
	
	CuAssert(tc, "correctly handles too many action condition", false == goap_set_pre( &ap, "ACT_-1", "ATOM_0", true ));
	CuAssert(tc, "post number of actions is correct", MAXACTIONS == ap.numactions);
	CuAssert(tc, "post number of atoms is 1", 1 == ap.numatoms);
	
	ap_test_clear(tc);
}

void ap_test_set_atom_limits(CuTest* tc)
{
	ap_test_clear(tc);
	
	char buff[20];
	for(int i=0; i<MAXATOMS; ++i) {
		sprintf(buff, "ACT_%d", i);
		CuAssert(tc, "able to set precondition", true == goap_set_pre( &ap, "ACT_0", strdup(buff), true ));
	}
	
	CuAssert(tc, "correctly handles too many atoms condition", false == goap_set_pre( &ap,"ACT_0", "ATOM_-1", true ));
	CuAssert(tc, "post number of atoms is correct", MAXATOMS == ap.numatoms);
	CuAssert(tc, "post number of actions is 1", 1 == ap.numactions);
	
	ap_test_clear(tc);
}
