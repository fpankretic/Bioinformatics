#include <catch2/catch_all.hpp>

#include "../include/Wavelet.hpp"

string givenEscherichiaColi() {
    std::ifstream input("../tests/resources/ecoli.txt");
    std::string ecoli;

    for (std::string line; std::getline(input, line);) {
        ecoli += line;
    }

    return ecoli;
}

SCENARIO("Test Wavelet creation with wrong input") {
    GIVEN("An empty string") {
        string givenEmptyString;
        WHEN("Creating wavelet tree with empty string") {
            THEN("Wavelet throws") {
                REQUIRE_THROWS_AS(Wavelet(givenEmptyString), invalid_argument);
            }
        }
    }

    GIVEN("One different character") {
        string givenOneDifferentCharacter = "aa";
        WHEN("Creating wavelet tree with one character") {
            THEN("Wavelet throws") {
                REQUIRE_THROWS_AS(Wavelet(givenOneDifferentCharacter), invalid_argument);
            }
        }
    }
}

SCENARIO("Test Wavelet creation with correct input") {
    GIVEN("A small string") {
        string givenSmallString = "ab";
        WHEN("Creating wavelet tree with empty string") {
            THEN("Wavelet doesn't throw") {
                REQUIRE_NOTHROW(Wavelet(givenSmallString));
            }
        }
    }

    GIVEN("E. Coli") {
        string givenEColi = givenEscherichiaColi();
        WHEN("Creating wavelet tree with one character") {
            THEN("Wavelet doesn't throw") {
                REQUIRE_NOTHROW(Wavelet(givenEColi));
            }
        }
    }
}

SCENARIO("Test access function") {
    GIVEN("Wavelet tree with small string") {
        string givenSmallString = "mississippi";
        Wavelet givenWaveletTree(givenSmallString);

        string actual_output;
        WHEN("Access queries are run") {
            for (int i = 0; i < givenSmallString.length(); ++i) {
                actual_output += givenWaveletTree.access(i);
            }

            THEN("Output is equal to givenSmallString") {
                REQUIRE(actual_output == givenSmallString);
            }
        }
    }

    GIVEN("Wavelet tree with large string") {
        string givenLargeString = givenEscherichiaColi();
        Wavelet givenWaveletTree(givenLargeString);

        string actual_output;
        WHEN("Access queries are run") {
            for (int i = 0; i < givenLargeString.length(); ++i) {
                actual_output += givenWaveletTree.access(i);
            }

            THEN("Output is equal to givenLargeString") {
                REQUIRE(actual_output == givenLargeString);
            }
        }
    }
}

SCENARIO("Test rank function") {
    GIVEN("Wavelet tree with small string") {
        string givenSmallString = "ab";
        Wavelet givenWaveletTree(givenSmallString);

        WHEN("Rank is ran in expected range") {
            auto actualAmount = givenWaveletTree.rank('a',0);
            THEN("ActualAmount is equal to 0") {
                REQUIRE(actualAmount == 0);
            }
        }

        WHEN("Rank is ran in expected range") {
            auto actualAmount = givenWaveletTree.rank('a',2);
            THEN("ActualAmount is equal to 1") {
                REQUIRE(actualAmount == 1);
            }
        }

        WHEN("Rank is ran in expected range") {
            auto actualAmount = givenWaveletTree.rank('b',1);
            THEN("ActualAmount is equal to 0") {
                REQUIRE(actualAmount == 0);
            }
        }

        WHEN("Rank is ran in out of expected range") {
            THEN("Rank should trow") {
                REQUIRE_THROWS_AS(givenWaveletTree.rank('a',3), invalid_argument);
            }
        }
    }

    GIVEN("Wavelet tree with medium string") {
        string givenMediumString = "mississippi";
        Wavelet givenWaveletTree(givenMediumString);

        WHEN("Rank is given non existing letter") {
            THEN("Rank should throw") {
                REQUIRE_THROWS_AS(givenWaveletTree.rank('a',givenMediumString.length()) , invalid_argument);
            }
        }

        WHEN("Rank is given starting letter at index after") {
            auto actualAmount = givenWaveletTree.rank('m',1);
            THEN("ActualAmount is equal to 1") {
                REQUIRE(actualAmount == 1);
            }
        }

        WHEN("Rank is given middle letter right at its 3rd index") {
            auto actualAmount = givenWaveletTree.rank('s',5);
            THEN("ActualAmount is equal to 2") {
                REQUIRE(actualAmount == 2);
            }
        }
    }

    // TODO: Add tests for large if fixed
}