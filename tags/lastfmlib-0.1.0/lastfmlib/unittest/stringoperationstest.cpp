#include <unittest++/UnitTest++.h>
#include <string>
#include <vector>
#include <iostream>

#include "lastfmlib/utils/stringoperations.h"

using std::string;
using std::wstring;
using std::vector;

using namespace StringOperations;

SUITE(StringOperationsTest)
{
    TEST(TestLowerCase)
    {
        string testString = "TESTSTRING";
        lowercase(testString);
        CHECK_EQUAL("teststring", testString);
        
        testString = "teststring";
        lowercase(testString);
        CHECK_EQUAL("teststring", testString);

        testString = "~!@#$%^&*()_1234567890-";
        lowercase(testString);
        CHECK_EQUAL("~!@#$%^&*()_1234567890-", testString);
        
        testString = "H_ell_O";
        lowercase(testString);
        CHECK_EQUAL("h_ell_o", testString);
    }
    
    TEST(TestDos2Unix)
    {
        string testString = "abcde\r\nfgs\r\r\n";
        dos2unix(testString);
        CHECK_EQUAL("abcde\nfgs\r\n", testString);
        
        testString = "\r\n";
        dos2unix(testString);
        CHECK_EQUAL("\n", testString);
        
        testString = "teststring";
        dos2unix(testString);
        CHECK_EQUAL("teststring", testString);
    }
    
    TEST(TestReplace)
    {
        string testString = "abcaabbabbab";
        replace(testString, "ab", "a");
        CHECK_EQUAL("acaababa", testString);
        
        testString = "stringstringstring";
        replace(testString, "stringstring", "string");
        CHECK_EQUAL("stringstring", testString);
    }
    
    TEST(TestTokenize)
    {
        string testString = "A-B-C";
        vector<string> tokenized;
        tokenized = tokenize(testString, "-");
        CHECK_EQUAL(3, tokenized.size());
        CHECK_EQUAL("A", tokenized[0]);
        CHECK_EQUAL("B", tokenized[1]);
        CHECK_EQUAL("C", tokenized[2]);
                
        testString = "A_*_B_*_C";
        tokenized = tokenize(testString, "_*_");
        CHECK_EQUAL(3, tokenized.size());
        CHECK_EQUAL("A", tokenized[0]);
        CHECK_EQUAL("B", tokenized[1]);
        CHECK_EQUAL("C", tokenized[2]);
        
        testString = "A_*_B_*_C";
        tokenized = tokenize(testString, "_**_");
        CHECK_EQUAL(1, tokenized.size());
        CHECK_EQUAL("A_*_B_*_C", tokenized[0]);
    }
    
    TEST(TestToNumeric)
    {
        int integer;
        string numericString = "42";
        toNumeric(numericString, integer);
        CHECK_EQUAL(42, integer);
        
        numericString = "-42";
        toNumeric(numericString, integer);
        CHECK_EQUAL(-42, integer);
        
        float floatingPoint;
        numericString = "42.0001";
        toNumeric(numericString, floatingPoint);
        CHECK_CLOSE(42.0001f, floatingPoint, 0.00001);
        
        numericString = "-42.0001";
        toNumeric(numericString, floatingPoint);
        CHECK_CLOSE(-42.0001f, floatingPoint, 0.00001);
    }

    TEST(TestConvertToUtf8)
    {
        string utf8String;
        wideCharToUtf8(L"Teststring", utf8String);
        CHECK_EQUAL("Teststring", utf8String);

        wideCharToUtf8(L"Trentemøller", utf8String);
        CHECK_EQUAL("Trentemøller", utf8String);
    }

    TEST(TestConvertToWidechar)
    {
        wstring wideString;
        utf8ToWideChar("Teststring", wideString);
        CHECK(wstring(L"Teststring") == wideString);

        utf8ToWideChar("Trentemøller", wideString);
        CHECK(wstring(L"Trentemøller") == wideString);
    }

    TEST(TestUrlEncode)
    {
        CHECK_EQUAL("!%40%23%24%25%5e%26*()fsdkjh+", urlEncode("!@#$%^&*()fsdkjh "));
        CHECK_EQUAL("Trentem%c3%b8ller", urlEncode("Trentemøller"));
    }
}
