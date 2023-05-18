#include <catch2/catch_all.hpp>

#include "../include/FMIndex.hpp"

SCENARIO("Test count function with empty pattern.") {
    GIVEN("FM index with string and empty pattern") {
        string givenString = "abaaba";
        FMIndex givenFMIndex(givenString);
        string givenEmptyPattern = "";

        WHEN("Count query is run") {
            THEN("Count throws invalid_argument") {
                REQUIRE_THROWS_AS(givenFMIndex.count(givenEmptyPattern), invalid_argument);
            }
        }
    }
}

SCENARIO("Test count function with short pattern.") {
    GIVEN("FM index with string and short pattern") {
        string givenString = "abaaba";
        FMIndex givenFMIndex(givenString);
        string givenShortPattern = "aba";

        WHEN("Count query is run") {
            auto actualOutput = givenFMIndex.count(givenShortPattern);

            THEN("Output is equal to 2") {
                REQUIRE(actualOutput == 2);
            }
        }
    }
}

SCENARIO("Test count function with long pattern.") {
    GIVEN("FM index with string and long pattern") {
        string givenString = "abaaba";
        FMIndex givenFMIndex(givenString);
        string givenLongPattern = "abaaba";

        WHEN("Count query is run") {
            auto actualOutput = givenFMIndex.count(givenLongPattern);

            THEN("Output is equal to 1") {
                REQUIRE(actualOutput == 1);
            }
        }
    }
}

SCENARIO("Test locate function with empty pattern.") {
    GIVEN("FM index with string and empty pattern") {
        string givenString = "abaaba";
        FMIndex givenFMIndex(givenString);
        string givenEmptyPattern = "";

        WHEN("Locate query is run") {
            THEN("Locate throws invalid_argument") {
                REQUIRE_THROWS_AS(givenFMIndex.locate(givenEmptyPattern), invalid_argument);
            }
        }
    }
}

SCENARIO("Test locate function with short pattern.") {
    GIVEN("FM index with string and short pattern") {
        string givenString = "abaaba";
        FMIndex givenFMIndex(givenString);
        string givenShortPattern = "aba";

        WHEN("Count query is run") {
            auto actualOutput = givenFMIndex.locate(givenShortPattern);

            vector<int> expectedOutput = {0, 3};
            THEN("Output is equal to vector<int> {0, 3}") {
                REQUIRE(actualOutput == expectedOutput);
            }
        }
    }
}

SCENARIO("Test locate function with long pattern.") {
    GIVEN("FM index with string and long pattern") {
        string givenString = "abaaba";
        FMIndex givenFMIndex(givenString);
        string givenLongPattern = "abaaba";

        WHEN("Count query is run") {
            auto actualOutput = givenFMIndex.locate(givenLongPattern);

            vector<int> expectedOutput = {0};
            THEN("Output is equal to to vector<int> {0}") {
                REQUIRE(actualOutput == expectedOutput);
            }
        }
    }
}