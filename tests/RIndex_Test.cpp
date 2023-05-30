#include <catch2/catch_all.hpp>

#include "../include/RIndex.hpp"

SCENARIO("Test locate function.") {
    GIVEN("R index with short string and non containing patter.") {
        string given_string = "mississippi";
        RIndex given_R_index(given_string);
        string given_short_pattern = "misisisis";

        WHEN("Locate query is run.") {
            auto actual_indexes = given_R_index.locate(given_short_pattern);
            THEN("The locate should return empty vector.") {
                REQUIRE(actual_indexes.empty());
            }
        }
    }

    GIVEN("R index with short string and start letter.") {
        string given_string = "mississippi";
        RIndex given_R_index(given_string);
        string given_short_pattern = "m";

        WHEN("Locate query is run.") {
            auto actual_indexes = given_R_index.locate(given_short_pattern);
            THEN("The locate should return start index.") {
                REQUIRE(actual_indexes[0] == 0);
            }
        }
    }

    GIVEN("R index with short string and middle letter.") {
        string given_string = "mississippi";
        RIndex given_R_index(given_string);
        string given_short_pattern = "p";

        WHEN("Locate query is run.") {
            auto actual_indexes = given_R_index.locate(given_short_pattern);
            THEN("The locate should return 2 indexes.") {
                REQUIRE(actual_indexes[0] == 8);
                REQUIRE(actual_indexes[1] == 9);
            }
        }
    }
    GIVEN("R index with short string and short pattern.") {
        string given_string = "tessstteeer";
        RIndex given_R_index(given_string);
        string given_short_pattern = "tee";

        WHEN("Locate query is run.") {
            auto actual_indexes = given_R_index.locate(given_short_pattern);
            THEN("The locate should return 1 indexes.") {
                REQUIRE(actual_indexes[0] == 6);
            }
        }
    }

    GIVEN("R index with short string and whole pattern.") {
        string given_string = "wholeString";
        RIndex given_R_index(given_string);
        string given_short_pattern = "wholeString";

        WHEN("Locate query is run.") {
            auto actual_indexes = given_R_index.locate(given_short_pattern);
            THEN("The locate should index 0.") {
                REQUIRE(actual_indexes[0] == 0);
            }
        }
    }
}