#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

TEST(CleanToken, PrefixAndSuffixCleaning) {
  ASSERT_THAT(cleanToken("...hello..."),StrEq("hello"));
  ASSERT_THAT(cleanToken("*./hello&^%"),StrEq("hello"));
  ASSERT_THAT(cleanToken(".<help()"),StrEq("help"));
  ASSERT_THAT(cleanToken("@hi_"),StrEq("hi"));
}
TEST(CleanToken, MiddleCleaning) {
  ASSERT_THAT(cleanToken("he...llo"),StrEq("he...llo"));
  ASSERT_THAT(cleanToken("sum(**)ary"),StrEq("sum(**)ary"));
  ASSERT_THAT(cleanToken("...he..llo"),StrEq("he..llo"));
  ASSERT_THAT(cleanToken("he..llo..."),StrEq("he..llo"));
  ASSERT_THAT(cleanToken("...he..llo..."),StrEq("he..llo"));
}
TEST(CleanToken, NumbersCleaning) {
  ASSERT_THAT(cleanToken("1.hello.1"),StrEq("1.hello.1"));
  ASSERT_THAT(cleanToken("123"),StrEq(""));
  ASSERT_THAT(cleanToken("..1hello1.."),StrEq("1hello1"));
  ASSERT_THAT(cleanToken("mine11craft"),StrEq("mine11craft"));
  ASSERT_THAT(cleanToken("11hello"),StrEq("11hello"));
  ASSERT_THAT(cleanToken("hello11"),StrEq("hello11"));
}
TEST(CleanToken, AllClean) {
  ASSERT_THAT(cleanToken("...12He..ll21..."),StrEq("12he..ll21"));
  ASSERT_THAT(cleanToken("12He..ll21.."),StrEq("12he..ll21"));
  ASSERT_THAT(cleanToken("..12He..ll21"),StrEq("12he..ll21"));
}

TEST(GatherTokens, LeadingSpace) {
  string text = " Hello my name is Greg1.";
  set<string> expected = {"hello","my","name","is","greg1"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, TrailingSpace) {
  string text = "Wh-at. .The. Sigma. ";
  set<string> expected = {"wh-at","the","sigma"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, MulptipleSpace) {
  string text = "Hey,   I'm  Ryan. :)";
  set<string> expected = {"hey","i'm","ryan"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, JustSpace) {
  string text = "   ";
  set<string> expected = {};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, Empty) {
  string text = "";
  set<string> expected = {};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(GatherTokens, NoTokens) {
  string text = "*12> () **+ ";
  set<string> expected = {};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}
TEST(BuildIndex, TinyTxt) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedIndex = {
      {"eggs", {"www.shoppinglist.com"}},
      {"milk", {"www.shoppinglist.com"}},
      {"fish", {"www.shoppinglist.com","www.dr.seuss.net"}},
      {"bread", {"www.shoppinglist.com"}},
      {"cheese", {"www.shoppinglist.com"}},
      {"red", {"www.rainbow.org","www.dr.seuss.net"}},
      {"gre-en", {"www.rainbow.org"}},
      {"orange", {"www.rainbow.org"}},
      {"yellow", {"www.rainbow.org"}},
      {"blue", {"www.rainbow.org","www.dr.seuss.net"}},
      {"indigo", {"www.rainbow.org"}},
      {"violet", {"www.rainbow.org"}},
      {"one", {"www.dr.seuss.net"}},
      {"two", {"www.dr.seuss.net"}},
      {"i'm", {"www.bigbadwolf.com"}},
      {"not", {"www.bigbadwolf.com"}},
      {"trying", {"www.bigbadwolf.com"}},
      {"to", {"www.bigbadwolf.com"}},
      {"eat", {"www.bigbadwolf.com"}},
      {"you", {"www.bigbadwolf.com"}}
  };
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(4)) << retTestFeedback;
}

TEST(BuildIndex, FileNotFound) {
  string filename = "data/silly.txt";
  map<string, set<string>> expectedIndex = {};
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(0)) << retTestFeedback;
}

map<string, set<string>> INDEX = {
    {"hello", {"example.com", "uic.edu"}},
    {"there", {"example.com"}},
    {"according", {"uic.edu"}},
    {"to", {"uic.edu"}},
    {"all", {"example.com", "uic.edu", "random.org"}},
    {"known", {"uic.edu"}},
    {"laws", {"random.org"}},
    {"of", {"random.org"}},
    {"aviation", {"random.org"}},
    {"a", {"uic.edu", "random.org"}},
};

TEST(FindQueryMatches, NoFirstTerm) {
  set<string> expected;

  expected = {"random.org","uic.edu"};
  EXPECT_THAT(findQueryMatches(INDEX, "God of known laws."), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "Bob"), ContainerEq(expected));
  EXPECT_THAT(findQueryMatches(INDEX, "bob"), ContainerEq(expected));

}

TEST(FindQueryMatches, NoLaterTerm) {
  set<string> expected;

  expected = {"example.com", "uic.edu"};
  EXPECT_THAT(findQueryMatches(INDEX, "Hello, God"), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "Hello, +God"), ContainerEq(expected));

  expected = {"example.com", "uic.edu"};
  EXPECT_THAT(findQueryMatches(INDEX, "Hello, -God"), ContainerEq(expected));
}