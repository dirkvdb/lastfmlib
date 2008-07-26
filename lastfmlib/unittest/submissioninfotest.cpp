#include <unittest++/UnitTest++.h>

#include "lastfmlib/submissioninfo.h"

#include <iostream>
#include <stdexcept>

using std::string;
using std::logic_error;

SUITE(SubmissionInfoTest)
{
    TEST(TestgetPostDataUtf8)
    {
        SubmissionInfo info("The Artist", "Trackname", 100);
        info.setAlbum("An Album");
        info.setTrackLength(42);
        info.setTrackNr(4);
        info.setSource(UserChosen);

        string expected = "&a[0]=The+Artist&t[0]=Trackname&i[0]=100"
                          "&o[0]=P&r[0]=&l[0]=42&b[0]=An+Album&n[0]=4&m[0]=";
        CHECK_EQUAL(expected, info.getPostData());

        info.setSource(NonPersonalizedBroadCast);
        info.setRating(Love);
        expected = "&a[0]=The+Artist&t[0]=Trackname&i[0]=100"
                   "&o[0]=R&r[0]=L&l[0]=42&b[0]=An+Album&n[0]=4&m[0]=";
        CHECK_EQUAL(expected, info.getPostData());

        info.setSource(PersonalizedRecommendation);
        info.setRating(Ban);
        expected = "&a[0]=The+Artist&t[0]=Trackname&i[0]=100"
                   "&o[0]=E&r[0]=B&l[0]=42&b[0]=An+Album&n[0]=4&m[0]=";
        CHECK_EQUAL(expected, info.getPostData());

        info.setSource(Lastfm, "12345");
        info.setRating(Skip);
        expected = "&a[0]=The+Artist&t[0]=Trackname&i[0]=100"
                   "&o[0]=L12345&r[0]=S&l[0]=42&b[0]=An+Album&n[0]=4&m[0]=";
        CHECK_EQUAL(expected, info.getPostData());

        info.setSource(Unknown);
        info.setRating(Skip);
        expected = "&a[1]=The+Artist&t[1]=Trackname&i[1]=100"
                   "&o[1]=U&r[1]=S&l[1]=42&b[1]=An+Album&n[1]=4&m[1]=";
        CHECK_EQUAL(expected, info.getPostData(1));
    }
    
    TEST(TestgetPostDataWidechar)
    {
        SubmissionInfo info(L"The Artist", L"Trackname", 100);
        info.setAlbum(L"An Album");
        info.setTrackLength(42);
        info.setTrackNr(4);
        info.setSource(UserChosen);

        string expected = "&a[0]=The+Artist&t[0]=Trackname&i[0]=100"
                          "&o[0]=P&r[0]=&l[0]=42&b[0]=An+Album&n[0]=4&m[0]=";
        CHECK_EQUAL(expected, info.getPostData());

        info.setSource(NonPersonalizedBroadCast);
        info.setRating(Love);
        expected = "&a[0]=The+Artist&t[0]=Trackname&i[0]=100"
                   "&o[0]=R&r[0]=L&l[0]=42&b[0]=An+Album&n[0]=4&m[0]=";
        CHECK_EQUAL(expected, info.getPostData());

        info.setSource(PersonalizedRecommendation);
        info.setRating(Ban);
        expected = "&a[0]=The+Artist&t[0]=Trackname&i[0]=100"
                   "&o[0]=E&r[0]=B&l[0]=42&b[0]=An+Album&n[0]=4&m[0]=";
        CHECK_EQUAL(expected, info.getPostData());

        info.setSource(Lastfm, "12345");
        info.setRating(Skip);
        expected = "&a[0]=The+Artist&t[0]=Trackname&i[0]=100"
                   "&o[0]=L12345&r[0]=S&l[0]=42&b[0]=An+Album&n[0]=4&m[0]=";
        CHECK_EQUAL(expected, info.getPostData());

        info.setSource(Unknown);
        info.setRating(Skip);
        expected = "&a[1]=The+Artist&t[1]=Trackname&i[1]=100"
                   "&o[1]=U&r[1]=S&l[1]=42&b[1]=An+Album&n[1]=4&m[1]=";
        CHECK_EQUAL(expected, info.getPostData(1));
    }

    TEST(TestSubmitWithouthTrackLength)
    {
        SubmissionInfo info(L"The Artist", L"Trackname", 100);
        info.setAlbum(L"An Album");
        info.setTrackNr(4);
        info.setSource(UserChosen);

        CHECK_THROW(info.getPostData(), logic_error);
    }
}

