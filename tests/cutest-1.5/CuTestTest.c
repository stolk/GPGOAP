#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CuTest.h"

/*-------------------------------------------------------------------------*
 * Helper functions
 *-------------------------------------------------------------------------*/

#define CompareAsserts(tc, message, expected, actual)  X_CompareAsserts((tc), __FILE__, __LINE__, (message), (expected), (actual))

static void X_CompareAsserts(CuTest* tc, const char *file, int line, const char* message, const char* expected, const char* actual)
{
	int mismatch;
	if (expected == NULL || actual == NULL) {
		mismatch = (expected != NULL || actual != NULL);
	} else {
		const char *front = __FILE__ ":";
		const size_t frontLen = strlen(front);
		const size_t expectedLen = strlen(expected);

		const char *matchStr = actual;

		mismatch = (strncmp(matchStr, front, frontLen) != 0);
		if (!mismatch) {
			matchStr = strchr(matchStr + frontLen, ':');
			mismatch |= (matchStr == NULL || strncmp(matchStr, ": ", 2));
			if (!mismatch) {
				matchStr += 2;
				mismatch |= (strncmp(matchStr, expected, expectedLen) != 0);
			}
		}
	}

	CuAssert_Line(tc, file, line, message, !mismatch);
}

/*-------------------------------------------------------------------------*
 * CuString Test
 *-------------------------------------------------------------------------*/

void TestCuStringNew(CuTest* tc)
{
	CuString* str = CuStringNew();
	CuAssertTrue(tc, 0 == str->length);
	CuAssertTrue(tc, 0 != str->size);
	CuAssertStrEquals(tc, "", str->buffer);
}


void TestCuStringAppend(CuTest* tc)
{
	CuString* str = CuStringNew();
	CuStringAppend(str, "hello");
	CuAssertIntEquals(tc, 5, str->length);
	CuAssertStrEquals(tc, "hello", str->buffer);
	CuStringAppend(str, " world");
	CuAssertIntEquals(tc, 11, str->length);
	CuAssertStrEquals(tc, "hello world", str->buffer);
}


void TestCuStringAppendNULL(CuTest* tc)
{
	CuString* str = CuStringNew();
	CuStringAppend(str, NULL);
	CuAssertIntEquals(tc, 4, str->length);
	CuAssertStrEquals(tc, "NULL", str->buffer);
}


void TestCuStringAppendChar(CuTest* tc)
{
	CuString* str = CuStringNew();
	CuStringAppendChar(str, 'a');
	CuStringAppendChar(str, 'b');
	CuStringAppendChar(str, 'c');
	CuStringAppendChar(str, 'd');
	CuAssertIntEquals(tc, 4, str->length);
	CuAssertStrEquals(tc, "abcd", str->buffer);
}


void TestCuStringInserts(CuTest* tc)
{
	CuString* str = CuStringNew();
	CuStringAppend(str, "world");
	CuAssertIntEquals(tc, 5, str->length);
	CuAssertStrEquals(tc, "world", str->buffer);
	CuStringInsert(str, "hell", 0);
	CuAssertIntEquals(tc, 9, str->length);
	CuAssertStrEquals(tc, "hellworld", str->buffer);
	CuStringInsert(str, "o ", 4);
	CuAssertIntEquals(tc, 11, str->length);
	CuAssertStrEquals(tc, "hello world", str->buffer);
	CuStringInsert(str, "!", 11);
	CuAssertIntEquals(tc, 12, str->length);
	CuAssertStrEquals(tc, "hello world!", str->buffer);
}


void TestCuStringResizes(CuTest* tc)
{
	CuString* str = CuStringNew();
	int i;
	for(i = 0 ; i < STRING_MAX ; ++i)
	{
		CuStringAppend(str, "aa");
	}
	CuAssertTrue(tc, STRING_MAX * 2 == str->length);
	CuAssertTrue(tc, STRING_MAX * 2 <= str->size);
}

CuSuite* CuStringGetSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestCuStringNew);
	SUITE_ADD_TEST(suite, TestCuStringAppend);
	SUITE_ADD_TEST(suite, TestCuStringAppendNULL);
	SUITE_ADD_TEST(suite, TestCuStringAppendChar);
	SUITE_ADD_TEST(suite, TestCuStringInserts);
	SUITE_ADD_TEST(suite, TestCuStringResizes);

	return suite;
}

/*-------------------------------------------------------------------------*
 * CuTest Test
 *-------------------------------------------------------------------------*/

void TestPasses(CuTest* tc)
{
	CuAssert(tc, "test should pass", 1 == 0 + 1);
}

void zTestFails(CuTest* tc)
{
	CuAssert(tc, "test should fail", 1 == 1 + 1);
}


void TestCuTestNew(CuTest* tc)
{
	CuTest* tc2 = CuTestNew("MyTest", TestPasses);
	CuAssertStrEquals(tc, "MyTest", tc2->name);
	CuAssertTrue(tc, !tc2->failed);
	CuAssertTrue(tc, tc2->message == NULL);
	CuAssertTrue(tc, tc2->function == TestPasses);
	CuAssertTrue(tc, tc2->ran == 0);
	CuAssertTrue(tc, tc2->jumpBuf == NULL);
}


void TestCuTestInit(CuTest *tc)
{
	CuTest tc2;
	CuTestInit(&tc2, "MyTest", TestPasses);
	CuAssertStrEquals(tc, "MyTest", tc2.name);
	CuAssertTrue(tc, !tc2.failed);
	CuAssertTrue(tc, tc2.message == NULL);
	CuAssertTrue(tc, tc2.function == TestPasses);
	CuAssertTrue(tc, tc2.ran == 0);
	CuAssertTrue(tc, tc2.jumpBuf == NULL);
}

void TestCuAssert(CuTest* tc)
{
	CuTest tc2;
	CuTestInit(&tc2, "MyTest", TestPasses);

	CuAssert(&tc2, "test 1", 5 == 4 + 1);
	CuAssertTrue(tc, !tc2.failed);
	CuAssertTrue(tc, tc2.message == NULL);

	CuAssert(&tc2, "test 2", 0);
	CuAssertTrue(tc, tc2.failed);
	CompareAsserts(tc, "CuAssert didn't fail", "test 2", tc2.message);

	CuAssert(&tc2, "test 3", 1);
	CuAssertTrue(tc, tc2.failed);
	CompareAsserts(tc, "CuAssert didn't fail", "test 2", tc2.message);

	CuAssert(&tc2, "test 4", 0);
	CuAssertTrue(tc, tc2.failed);
	CompareAsserts(tc, "CuAssert didn't fail", "test 4", tc2.message);

}

void TestCuAssertPtrEquals_Success(CuTest* tc)
{
	CuTest tc2;
	int x;

	CuTestInit(&tc2, "MyTest", TestPasses);

	/* test success case */
	CuAssertPtrEquals(&tc2, &x, &x);
	CuAssertTrue(tc, ! tc2.failed);
	CuAssertTrue(tc, NULL == tc2.message);
}

void TestCuAssertPtrEquals_Failure(CuTest* tc)
{
	CuTest tc2;
	int x;
	int* nullPtr = NULL;
	char expected_message[STRING_MAX];

	CuTestInit(&tc2, "MyTest", TestPasses);

	/* test failing case */
	sprintf(expected_message, "expected pointer <0x%p> but was <0x%p>", nullPtr, &x);
	CuAssertPtrEquals(&tc2, NULL, &x);
	CuAssertTrue(tc, tc2.failed);
	CompareAsserts(tc, "CuAssertPtrEquals failed", expected_message, tc2.message);
}

void TestCuAssertPtrNotNull_Success(CuTest* tc)
{
	CuTest tc2;
	int x;

	CuTestInit(&tc2, "MyTest", TestPasses);

	/* test success case */
	CuAssertPtrNotNull(&tc2, &x);
	CuAssertTrue(tc, ! tc2.failed);
	CuAssertTrue(tc, NULL == tc2.message);
}

void TestCuAssertPtrNotNull_Failure(CuTest* tc)
{
	CuTest tc2;

	CuTestInit(&tc2, "MyTest", TestPasses);

	/* test failing case */
	CuAssertPtrNotNull(&tc2, NULL);
	CuAssertTrue(tc, tc2.failed);
	CompareAsserts(tc, "CuAssertPtrNotNull failed", "null pointer unexpected", tc2.message);
}

void TestCuTestRun(CuTest* tc)
{
	CuTest tc2;
	CuTestInit(&tc2, "MyTest", zTestFails);
	CuTestRun(&tc2);

	CuAssertStrEquals(tc, "MyTest", tc2.name);
	CuAssertTrue(tc, tc2.failed);
	CuAssertTrue(tc, tc2.ran);
	CompareAsserts(tc, "TestRun failed", "test should fail", tc2.message);
}

/*-------------------------------------------------------------------------*
 * CuSuite Test
 *-------------------------------------------------------------------------*/

void TestCuSuiteInit(CuTest* tc)
{
	CuSuite ts;
	CuSuiteInit(&ts);
	CuAssertTrue(tc, ts.count == 0);
	CuAssertTrue(tc, ts.failCount == 0);
}

void TestCuSuiteNew(CuTest* tc)
{
	CuSuite* ts = CuSuiteNew();
	CuAssertTrue(tc, ts->count == 0);
	CuAssertTrue(tc, ts->failCount == 0);
}

void TestCuSuiteAddTest(CuTest* tc)
{
	CuSuite ts;
	CuTest tc2;

	CuSuiteInit(&ts);
	CuTestInit(&tc2, "MyTest", zTestFails);

	CuSuiteAdd(&ts, &tc2);
	CuAssertTrue(tc, ts.count == 1);

	CuAssertStrEquals(tc, "MyTest", ts.list[0]->name);
}

void TestCuSuiteAddSuite(CuTest* tc)
{
	CuSuite* ts1 = CuSuiteNew();
	CuSuite* ts2 = CuSuiteNew();

	CuSuiteAdd(ts1, CuTestNew("TestFails1", zTestFails));
	CuSuiteAdd(ts1, CuTestNew("TestFails2", zTestFails));

	CuSuiteAdd(ts2, CuTestNew("TestFails3", zTestFails));
	CuSuiteAdd(ts2, CuTestNew("TestFails4", zTestFails));

	CuSuiteAddSuite(ts1, ts2);
	CuAssertIntEquals(tc, 4, ts1->count);

	CuAssertStrEquals(tc, "TestFails1", ts1->list[0]->name);
	CuAssertStrEquals(tc, "TestFails2", ts1->list[1]->name);
	CuAssertStrEquals(tc, "TestFails3", ts1->list[2]->name);
	CuAssertStrEquals(tc, "TestFails4", ts1->list[3]->name);
}

void TestCuSuiteRun(CuTest* tc)
{
	CuSuite ts;
	CuTest tc1, tc2, tc3, tc4;

	CuSuiteInit(&ts);
	CuTestInit(&tc1, "TestPasses", TestPasses);
	CuTestInit(&tc2, "TestPasses", TestPasses);
	CuTestInit(&tc3, "TestFails",  zTestFails);
	CuTestInit(&tc4, "TestFails",  zTestFails);

	CuSuiteAdd(&ts, &tc1);
	CuSuiteAdd(&ts, &tc2);
	CuSuiteAdd(&ts, &tc3);
	CuSuiteAdd(&ts, &tc4);
	CuAssertTrue(tc, ts.count == 4);

	CuSuiteRun(&ts);
	CuAssertTrue(tc, ts.count - ts.failCount == 2);
	CuAssertTrue(tc, ts.failCount == 2);
}

void TestCuSuiteSummary(CuTest* tc)
{
	CuSuite ts;
	CuTest tc1, tc2;
	CuString summary;

	CuSuiteInit(&ts);
	CuTestInit(&tc1, "TestPasses", TestPasses);
	CuTestInit(&tc2, "TestFails",  zTestFails);
	CuStringInit(&summary);

	CuSuiteAdd(&ts, &tc1);
	CuSuiteAdd(&ts, &tc2);
	CuSuiteRun(&ts);

	CuSuiteSummary(&ts, &summary);

	CuAssertTrue(tc, ts.count == 2);
	CuAssertTrue(tc, ts.failCount == 1);
	CuAssertStrEquals(tc, ".F\n\n", summary.buffer);
}


void TestCuSuiteDetails_SingleFail(CuTest* tc)
{
	CuSuite ts;
	CuTest tc1, tc2;
	CuString details;
	const char* front;
	const char* back;

	CuSuiteInit(&ts);
	CuTestInit(&tc1, "TestPasses", TestPasses);
	CuTestInit(&tc2, "TestFails",  zTestFails);
	CuStringInit(&details);

	CuSuiteAdd(&ts, &tc1);
	CuSuiteAdd(&ts, &tc2);
	CuSuiteRun(&ts);

	CuSuiteDetails(&ts, &details);

	CuAssertTrue(tc, ts.count == 2);
	CuAssertTrue(tc, ts.failCount == 1);

	front = "There was 1 failure:\n"
		"1) TestFails: ";
	back =  "test should fail\n"
		"\n!!!FAILURES!!!\n"
		"Runs: 2 Passes: 1 Fails: 1\n";

	CuAssertStrEquals(tc, back, details.buffer + strlen(details.buffer) - strlen(back));
	details.buffer[strlen(front)] = 0;
	CuAssertStrEquals(tc, front, details.buffer);
}


void TestCuSuiteDetails_SinglePass(CuTest* tc)
{
	CuSuite ts;
	CuTest tc1;
	CuString details;
	const char* expected;

	CuSuiteInit(&ts);
	CuTestInit(&tc1, "TestPasses", TestPasses);
	CuStringInit(&details);

	CuSuiteAdd(&ts, &tc1);
	CuSuiteRun(&ts);

	CuSuiteDetails(&ts, &details);

	CuAssertTrue(tc, ts.count == 1);
	CuAssertTrue(tc, ts.failCount == 0);

	expected =
		"OK (1 test)\n";

	CuAssertStrEquals(tc, expected, details.buffer);
}

void TestCuSuiteDetails_MultiplePasses(CuTest* tc)
{
	CuSuite ts;
	CuTest tc1, tc2;
	CuString details;
	const char* expected;

	CuSuiteInit(&ts);
	CuTestInit(&tc1, "TestPasses", TestPasses);
	CuTestInit(&tc2, "TestPasses", TestPasses);
	CuStringInit(&details);

	CuSuiteAdd(&ts, &tc1);
	CuSuiteAdd(&ts, &tc2);
	CuSuiteRun(&ts);

	CuSuiteDetails(&ts, &details);

	CuAssertTrue(tc, ts.count == 2);
	CuAssertTrue(tc, ts.failCount == 0);

	expected =
		"OK (2 tests)\n";

	CuAssertStrEquals(tc, expected, details.buffer);
}

void TestCuSuiteDetails_MultipleFails(CuTest* tc)
{
	CuSuite ts;
	CuTest tc1, tc2;
	CuString details;
	const char* front;
	const char* mid;
	const char* back;

	CuSuiteInit(&ts);
	CuTestInit(&tc1, "TestFails1", zTestFails);
	CuTestInit(&tc2, "TestFails2", zTestFails);
	CuStringInit(&details);

	CuSuiteAdd(&ts, &tc1);
	CuSuiteAdd(&ts, &tc2);
	CuSuiteRun(&ts);

	CuSuiteDetails(&ts, &details);

	CuAssertTrue(tc, ts.count == 2);
	CuAssertTrue(tc, ts.failCount == 2);

	front =
		"There were 2 failures:\n"
		"1) TestFails1: ";
	mid =   "test should fail\n"
		"2) TestFails2: ";
	back =  "test should fail\n"
		"\n!!!FAILURES!!!\n"
		"Runs: 2 Passes: 0 Fails: 2\n";

	CuAssertStrEquals(tc, back, details.buffer + strlen(details.buffer) - strlen(back));
	CuAssert(tc, "Couldn't find middle", strstr(details.buffer, mid) != NULL);
	details.buffer[strlen(front)] = 0;
	CuAssertStrEquals(tc, front, details.buffer);
}


/*-------------------------------------------------------------------------*
 * Misc Test
 *-------------------------------------------------------------------------*/

void TestCuStrCopy(CuTest* tc)
{
	const char* old = "hello world";
	const char* newStr = CuStrCopy(old);
	CuAssert(tc, "old is new", strcmp(old, newStr) == 0);
}


void TestCuStringAppendFormat(CuTest* tc)
{
	int i;
	char* text = CuStrAlloc(301);		/* long string */
	CuString* str = CuStringNew();
	for (i = 0 ; i < 300 ; ++i)
		text[i] = 'a';
	text[300] = '\0';
	CuStringAppendFormat(str, "%s", text);

	/* buffer limit raised to HUGE_STRING_LEN so no overflow */

	CuAssert(tc, "length of str->buffer is 300", 300 == strlen(str->buffer));
}

void TestFail(CuTest* tc)
{
	jmp_buf buf;
	int pointReached = 0;
	CuTest* tc2 = CuTestNew("TestFails", zTestFails);
	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuFail(tc2, "hello world");
		pointReached = 1;
	}
	CuAssert(tc, "point was not reached", pointReached == 0);
}

void TestAssertStrEquals(CuTest* tc)
{
	jmp_buf buf;
	CuTest *tc2 = CuTestNew("TestAssertStrEquals", zTestFails);

	const char* expected = "expected <hello> but was <world>";
	const char *expectedMsg = "some text: expected <hello> but was <world>";

	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals(tc2, "hello", "world");
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals failed", expected, tc2->message);
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals_Msg(tc2, "some text", "hello", "world");
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals failed", expectedMsg, tc2->message);
}

void TestAssertStrEquals_NULL(CuTest* tc)
{
	jmp_buf buf;
	CuTest *tc2 = CuTestNew("TestAssertStrEquals_NULL", zTestFails);

	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals(tc2, NULL, NULL);
	}
	CuAssertTrue(tc, !tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_NULL failed", NULL, tc2->message);
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals_Msg(tc2, "some text", NULL, NULL);
	}
	CuAssertTrue(tc, !tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_NULL failed", NULL, tc2->message);
}

void TestAssertStrEquals_FailNULLStr(CuTest* tc)
{
	jmp_buf buf;
	CuTest *tc2 = CuTestNew("TestAssertStrEquals_FailNULLStr", zTestFails);

	const char* expected = "expected <hello> but was <NULL>";
	const char *expectedMsg = "some text: expected <hello> but was <NULL>";

	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals(tc2, "hello", NULL);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_FailNULLStr failed", expected, tc2->message);
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals_Msg(tc2, "some text", "hello", NULL);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_FailNULLStr failed", expectedMsg, tc2->message);
}

void TestAssertStrEquals_FailStrNULL(CuTest* tc)
{
	jmp_buf buf;
	CuTest *tc2 = CuTestNew("TestAssertStrEquals_FailStrNULL", zTestFails);

	const char* expected = "expected <NULL> but was <hello>";
	const char *expectedMsg = "some text: expected <NULL> but was <hello>";

	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals(tc2, NULL, "hello");
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_FailStrNULL failed", expected, tc2->message);
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals_Msg(tc2, "some text", NULL, "hello");
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_FailStrNULL failed", expectedMsg, tc2->message);
}

void TestAssertIntEquals(CuTest* tc)
{
	jmp_buf buf;
	CuTest *tc2 = CuTestNew("TestAssertIntEquals", zTestFails);
	const char* expected = "expected <42> but was <32>";
	const char* expectedMsg = "some text: expected <42> but was <32>";
	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertIntEquals(tc2, 42, 32);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertIntEquals failed", expected, tc2->message);
	if (setjmp(buf) == 0)
	{
		CuAssertIntEquals_Msg(tc2, "some text", 42, 32);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals failed", expectedMsg, tc2->message);
}

void TestAssertDblEquals(CuTest* tc)
{
	jmp_buf buf;
	double x = 3.33;
	double y = 10.0 / 3.0;
	CuTest *tc2 = CuTestNew("TestAssertDblEquals", zTestFails);
	char expected[STRING_MAX];
	char expectedMsg[STRING_MAX];
	sprintf(expected, "expected <%lf> but was <%lf>", x, y);
	sprintf(expectedMsg, "some text: expected <%lf> but was <%lf>", x, y);

	CuTestInit(tc2, "TestAssertDblEquals", TestPasses);

	CuAssertDblEquals(tc2, x, x, 0.0);
	CuAssertTrue(tc, ! tc2->failed);
	CuAssertTrue(tc, NULL == tc2->message);

	CuAssertDblEquals(tc2, x, y, 0.01);
	CuAssertTrue(tc, ! tc2->failed);
	CuAssertTrue(tc, NULL == tc2->message);

	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertDblEquals(tc2, x, y, 0.001);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertDblEquals failed", expected, tc2->message);
	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertDblEquals_Msg(tc2, "some text", x, y, 0.001);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertDblEquals failed", expectedMsg, tc2->message);
}

/*-------------------------------------------------------------------------*
 * main
 *-------------------------------------------------------------------------*/

CuSuite* CuGetSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestCuStringAppendFormat);
	SUITE_ADD_TEST(suite, TestCuStrCopy);
	SUITE_ADD_TEST(suite, TestFail);
	SUITE_ADD_TEST(suite, TestAssertStrEquals);
	SUITE_ADD_TEST(suite, TestAssertStrEquals_NULL);
	SUITE_ADD_TEST(suite, TestAssertStrEquals_FailStrNULL);
	SUITE_ADD_TEST(suite, TestAssertStrEquals_FailNULLStr);
	SUITE_ADD_TEST(suite, TestAssertIntEquals);
	SUITE_ADD_TEST(suite, TestAssertDblEquals);

	SUITE_ADD_TEST(suite, TestCuTestNew);
	SUITE_ADD_TEST(suite, TestCuTestInit);
	SUITE_ADD_TEST(suite, TestCuAssert);
	SUITE_ADD_TEST(suite, TestCuAssertPtrEquals_Success);
	SUITE_ADD_TEST(suite, TestCuAssertPtrEquals_Failure);
	SUITE_ADD_TEST(suite, TestCuAssertPtrNotNull_Success);
	SUITE_ADD_TEST(suite, TestCuAssertPtrNotNull_Failure);
	SUITE_ADD_TEST(suite, TestCuTestRun);

	SUITE_ADD_TEST(suite, TestCuSuiteInit);
	SUITE_ADD_TEST(suite, TestCuSuiteNew);
	SUITE_ADD_TEST(suite, TestCuSuiteAddTest);
	SUITE_ADD_TEST(suite, TestCuSuiteAddSuite);
	SUITE_ADD_TEST(suite, TestCuSuiteRun);
	SUITE_ADD_TEST(suite, TestCuSuiteSummary);
	SUITE_ADD_TEST(suite, TestCuSuiteDetails_SingleFail);
	SUITE_ADD_TEST(suite, TestCuSuiteDetails_SinglePass);
	SUITE_ADD_TEST(suite, TestCuSuiteDetails_MultiplePasses);
	SUITE_ADD_TEST(suite, TestCuSuiteDetails_MultipleFails);

	return suite;
}
