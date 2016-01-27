/* AUTOGENERATED FILE. DO NOT EDIT. */

//=======Test Runner Used To Run Each Test Below=====
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.CurrentTestLineNumber = TestLineNum; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      setUp(); \
      TestFunc(); \
  } \
  if (TEST_PROTECT() && !TEST_IS_IGNORED) \
  { \
    tearDown(); \
  } \
  UnityConcludeTest(); \
}

//=======Automagically Detected Files To Include=====
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>
#include "config.h"
#include "sntptest.h"
#include "networking.h"
#include "ntp_stdlib.h"

//=======External Functions This Runner Calls=====
extern void setUp(void);
extern void tearDown(void);
extern void test_TooShortLength(void);
extern void test_LengthNotMultipleOfFour(void);
extern void test_TooShortExtensionFieldLength(void);
extern void test_UnauthenticatedPacketReject(void);
extern void test_CryptoNAKPacketReject(void);
extern void test_AuthenticatedPacketInvalid(void);
extern void test_AuthenticatedPacketUnknownKey(void);
extern void test_ServerVersionTooOld(void);
extern void test_ServerVersionTooNew(void);
extern void test_NonWantedMode(void);
extern void test_KoDRate(void);
extern void test_KoDDeny(void);
extern void test_RejectUnsyncedServer(void);
extern void test_RejectWrongResponseServerMode(void);
extern void test_AcceptNoSentPacketBroadcastMode(void);
extern void test_CorrectUnauthenticatedPacket(void);
extern void test_CorrectAuthenticatedPacketMD5(void);
extern void test_CorrectAuthenticatedPacketSHA1(void);


//=======Test Reset Option=====
void resetTest(void);
void resetTest(void)
{
  tearDown();
  setUp();
}

char const *progname;


//=======MAIN=====
int main(int argc, char *argv[])
{
  progname = argv[0];
  UnityBegin("packetProcessing.c");
  RUN_TEST(test_TooShortLength, 25);
  RUN_TEST(test_LengthNotMultipleOfFour, 26);
  RUN_TEST(test_TooShortExtensionFieldLength, 27);
  RUN_TEST(test_UnauthenticatedPacketReject, 28);
  RUN_TEST(test_CryptoNAKPacketReject, 29);
  RUN_TEST(test_AuthenticatedPacketInvalid, 30);
  RUN_TEST(test_AuthenticatedPacketUnknownKey, 31);
  RUN_TEST(test_ServerVersionTooOld, 32);
  RUN_TEST(test_ServerVersionTooNew, 33);
  RUN_TEST(test_NonWantedMode, 34);
  RUN_TEST(test_KoDRate, 35);
  RUN_TEST(test_KoDDeny, 36);
  RUN_TEST(test_RejectUnsyncedServer, 37);
  RUN_TEST(test_RejectWrongResponseServerMode, 38);
  RUN_TEST(test_AcceptNoSentPacketBroadcastMode, 39);
  RUN_TEST(test_CorrectUnauthenticatedPacket, 40);
  RUN_TEST(test_CorrectAuthenticatedPacketMD5, 41);
  RUN_TEST(test_CorrectAuthenticatedPacketSHA1, 42);

  return (UnityEnd());
}
