#include <unittest++/UnitTest++.h>

#include "lastfmlib/submissioninfocollection.h"

#include <iostream>
#include <stdexcept>

using std::string;
using std::logic_error;

SUITE(SubmissionInfoCollectionTest)
{
    TEST(TestgetPostData)
    {
        SubmissionInfo info1("The Artist1", "Trackname1", 100);
        info1.setAlbum("An Album1");
        info1.setTrackLength(1);
        info1.setTrackNr(1);
        info1.setSource(UserChosen);

        SubmissionInfo info2("The Artist2", "Trackname2", 200);
        info2.setAlbum("An Album2");
        info2.setTrackLength(2);
        info2.setTrackNr(2);
        info2.setSource(Unknown);

        SubmissionInfoCollection collection;
        collection.addInfo(info1);
        collection.addInfo(info2);

        string expected = "&a[0]=The+Artist1&t[0]=Trackname1&i[0]=100"
                          "&o[0]=P&r[0]=&l[0]=1&b[0]=An+Album1&n[0]=1&m[0]="
                          "&a[1]=The+Artist2&t[1]=Trackname2&i[1]=200"
                          "&o[1]=U&r[1]=&l[1]=2&b[1]=An+Album2&n[1]=2&m[1]=";
        CHECK_EQUAL(expected, collection.getPostData());
    }
}

