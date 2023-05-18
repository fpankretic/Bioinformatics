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

        int output;
        WHEN("Count query is run") {
            output = givenFMIndex.count(givenShortPattern);

            THEN("Output is equal to 2") {
                REQUIRE(output == 2);
            }
        }
    }
}

SCENARIO("Test count function with long pattern.") {
    GIVEN("FM index with string and long pattern") {
        string givenString = "abaaba";
        FMIndex givenFMIndex(givenString);

        string givenLongPattern = "abaaba";

        int output;
        WHEN("Count query is run") {
            output = givenFMIndex.count(givenLongPattern);

            THEN("Output is equal to 1") {
                REQUIRE(output == 1);
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

        vector<int> output;
        vector<int> expected = {0, 3};
        WHEN("Count query is run") {
            output = givenFMIndex.locate(givenShortPattern);

            THEN("Output is equal to vector<int> {0, 3}") {
                REQUIRE(output == expected);
            }
        }
    }
}

SCENARIO("Test locate function with long pattern.") {
    GIVEN("FM index with string and long pattern") {
        string givenString = "abaaba";
        FMIndex givenFMIndex(givenString);

        string givenLongPattern = "abaaba";

        vector<int> output;
        vector<int> expected = {0};
        WHEN("Count query is run") {
            output = givenFMIndex.locate(givenLongPattern);

            THEN("Output is equal to to vector<int> {0}") {
                REQUIRE(output == expected);
            }
        }
    }
}