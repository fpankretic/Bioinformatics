#include <catch2/catch_all.hpp>

#include "../include/Wavelet.hpp"

string given_Escherichia_Coli() {
    std::ifstream input("../tests/resources/ecoli.txt");
    std::string ecoli;

    for (std::string line; std::getline(input, line);) {
        ecoli += line;
    }

    return ecoli;
}

SCENARIO("Test Wavelet creation with wrong input") {
    GIVEN("An empty string") {
        string given_empty_string;
        WHEN("Creating wavelet tree with empty string") {
            THEN("Wavelet throws") {
                REQUIRE_THROWS_AS(Wavelet(given_empty_string), invalid_argument);
            }
        }
    }

    GIVEN("One different character") {
        string given_one_different_character = "aa";
        WHEN("Creating wavelet tree with one character") {
            THEN("Wavelet throws") {
                REQUIRE_THROWS_AS(Wavelet(given_one_different_character), invalid_argument);
            }
        }
    }
}

SCENARIO("Test Wavelet creation with correct input") {
    GIVEN("A small string") {
        string given_small_string = "ab";
        WHEN("Creating wavelet tree with empty string") {
            THEN("Wavelet doesn't throw") {
                REQUIRE_NOTHROW(Wavelet(given_small_string));
            }
        }
    }

    GIVEN("E. Coli") {
        string given_E_Coli = given_Escherichia_Coli();
        WHEN("Creating wavelet tree with more than one character") {
            THEN("Wavelet doesn't throw") {
                REQUIRE_NOTHROW(Wavelet(given_E_Coli));
            }
        }
    }
}

SCENARIO("Test access function") {
    GIVEN("Wavelet tree with small string") {
        string given_small_string = "mississippi";
        Wavelet given_wavelet_tree(given_small_string);

        string actual_output;
        WHEN("Access queries are run") {
            for (int i = 0; i < given_small_string.length(); ++i) {
                actual_output += given_wavelet_tree.access(i);
            }

            THEN("Output is equal to givenSmallString") {
                REQUIRE(actual_output == given_small_string);
            }
        }
    }

    GIVEN("Wavelet tree with large string") {
        string given_large_string = given_Escherichia_Coli();
        Wavelet given_wavelet_tree(given_large_string);

        string actual_output;
        WHEN("Access queries are run") {
            for (int i = 0; i < given_large_string.length(); ++i) {
                actual_output += given_wavelet_tree.access(i);
            }

            THEN("Output is equal to givenLargeString") {
                REQUIRE(actual_output == given_large_string);
            }
        }
    }
}

SCENARIO("Test rank function") {
    GIVEN("Wavelet tree with small string") {
        string given_small_string = "ab";
        Wavelet given_wavelet_tree(given_small_string);

        WHEN("Rank is ran in expected range") {
            auto actual_amount = given_wavelet_tree.rank('a',0);
            THEN("Actual amount is equal to 0") {
                REQUIRE(actual_amount == 0);
            }
        }

        WHEN("Rank is ran in expected range") {
            auto actual_amount = given_wavelet_tree.rank('a',1);
            THEN("Actual amount is equal to 1") {
                REQUIRE(actual_amount == 1);
            }
        }

        WHEN("Rank is ran in expected range") {
            auto actual_amount = given_wavelet_tree.rank('b',1);
            THEN("Actual amount is equal to 0") {
                REQUIRE(actual_amount == 0);
            }
        }

        WHEN("Rank is ran in out of expected range") {
            THEN("Rank should trow") {
                REQUIRE_THROWS_AS(given_wavelet_tree.rank('a',3), invalid_argument);
            }
        }
    }

    GIVEN("Wavelet tree with medium string") {
        string given_medium_string = "mississippi";
        Wavelet given_wavelet_tree(given_medium_string);

        WHEN("Rank is given non existing letter") {
            THEN("Rank should throw") {
                REQUIRE_THROWS_AS(given_wavelet_tree.rank('a',given_medium_string.length()) , invalid_argument);
            }
        }

        WHEN("Rank is given starting letter at index after") {
            auto actual_amount = given_wavelet_tree.rank('m',1);
            THEN("Actual amount is equal to 1") {
                REQUIRE(actual_amount == 1);
            }
        }

        WHEN("Rank is given middle letter right at its 3rd index") {
            auto actual_amount = given_wavelet_tree.rank('s',5);
            THEN("Actual amount is equal to 2") {
                REQUIRE(actual_amount == 2);
            }
        }
    }
}

SCENARIO("Test select function") {
    GIVEN("Wavelet tree with small string") {
        string given_small_string = "ab";
        Wavelet given_wavelet_tree(given_small_string);

        WHEN("Select is ran in expected range") {
            auto actual_amount = given_wavelet_tree.select('a',0);
            THEN("Actual amount is equal to 0") {
                REQUIRE(actual_amount == 0);
            }
        }

        WHEN("Select is ran in expected range") {
            auto actual_amount = given_wavelet_tree.select('a',1);
            THEN("Actual amount is equal to 1") {
                REQUIRE(actual_amount == 1);
            }
        }

        WHEN("Select is ran in expected range") {
            auto actual_amount = given_wavelet_tree.select('b',0);
            THEN("Actual amount is equal to 1") {
                REQUIRE(actual_amount == 1);
            }
        }

        WHEN("Select is ran in expected range") {
            auto actual_amount = given_wavelet_tree.select('b',1);
            THEN("Actual amount is equal to 1") {
                REQUIRE(actual_amount == 1);
            }
        }

        WHEN("Select is ran in out of expected range") {
            THEN("Select should trow") {
                REQUIRE_THROWS_AS(given_wavelet_tree.select('a',3), invalid_argument);
            }
        }
    }

    GIVEN("Wavelet tree with medium string") {
        string given_medium_string = "mississippi";
        Wavelet given_wavelet_tree(given_medium_string);

        WHEN("Select is given non existing letter") {
            THEN("Select should throw") {
                REQUIRE_THROWS_AS(given_wavelet_tree.select('a',given_medium_string.length()) , invalid_argument);
            }
        }

        WHEN("Select is given starting letter and rank 0") {
            auto actual_amount = given_wavelet_tree.select('m',0);
            THEN("Actual amount is equal to 0") {
                REQUIRE(actual_amount == 0);
            }
        }

        WHEN("Select is given starting letter and rank 1") {
            auto actual_amount = given_wavelet_tree.select('m',1);
            THEN("Actual amount is equal to given_medium_string length - 1") {
                REQUIRE(actual_amount == given_medium_string.length() - 1);
            }
        }

        WHEN("Select is given middle letter and rank 3") {
            auto actual_amount = given_wavelet_tree.select('s',3);
            THEN("Actual amount is equal to 6") {
                REQUIRE(actual_amount == 6);
            }
        }
    }
}