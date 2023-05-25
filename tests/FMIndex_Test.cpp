#include <catch2/catch_all.hpp>

#include "../include/FMIndex.hpp"

SCENARIO("Test count function with empty pattern.") {
    GIVEN("FM index with string and empty pattern") {
        string given_string = "abaaba";
        FMIndex given_FM_index(given_string);
        string given_empty_pattern = "";

        WHEN("Count query is run") {
            THEN("Count throws invalid_argument") {
                REQUIRE_THROWS_AS(given_FM_index.count(given_empty_pattern), invalid_argument);
            }
        }
    }
}

SCENARIO("Test count function with short pattern.") {
    GIVEN("FM index with string and short pattern") {
        string given_string = "abaaba";
        FMIndex given_FM_index(given_string);
        string given_short_pattern = "aba";

        WHEN("Count query is run") {
            auto actual_output = given_FM_index.count(given_short_pattern);

            THEN("Output is equal to 2") {
                REQUIRE(actual_output == 2);
            }
        }
    }
}

SCENARIO("Test count function with long pattern.") {
    GIVEN("FM index with string and long pattern") {
        string given_string = "abaaba";
        FMIndex given_FM_index(given_string);
        string given_long_pattern = "abaaba";

        WHEN("Count query is run") {
            auto actual_output = given_FM_index.count(given_long_pattern);

            THEN("Output is equal to 1") {
                REQUIRE(actual_output == 1);
            }
        }
    }
}

SCENARIO("Test locate function with empty pattern.") {
    GIVEN("FM index with string and empty pattern") {
        string given_string = "abaaba";
        FMIndex given_FM_index(given_string);
        string given_empty_pattern = "";

        WHEN("Locate query is run") {
            THEN("Locate throws invalid_argument") {
                REQUIRE_THROWS_AS(given_FM_index.locate(given_empty_pattern), invalid_argument);
            }
        }
    }
}

SCENARIO("Test locate function with short pattern.") {
    GIVEN("FM index with string and short pattern") {
        string given_string = "abaaba";
        FMIndex given_FM_index(given_string);
        string given_short_pattern = "aba";

        WHEN("Count query is run") {
            auto actual_output = given_FM_index.locate(given_short_pattern);

            vector<int> expected_output = {0, 3};
            THEN("Output is equal to vector<int> {0, 3}") {
                REQUIRE(actual_output == expected_output);
            }
        }
    }
}

SCENARIO("Test locate function with long pattern.") {
    GIVEN("FM index with string and long pattern") {
        string given_string = "abaaba";
        FMIndex given_FM_index(given_string);
        string given_long_pattern = "abaaba";

        WHEN("Count query is run") {
            auto actual_output = given_FM_index.locate(given_long_pattern);

            vector<int> expected_output = {0};
            THEN("Output is equal to vector<int> {0}") {
                REQUIRE(actual_output == expected_output);
            }
        }
    }
}