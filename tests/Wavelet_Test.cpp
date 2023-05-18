#include <catch2/catch_all.hpp>

#include "../include/Wavelet.hpp"

SCENARIO("Test small string access function.") {
    GIVEN("Wavelet tree with small string") {
        string givenSmallString = "mississippi";
        Wavelet givenWaveletTree(givenSmallString);

        string output;
        WHEN("Access queries are run") {
            for (int i = 0; i < givenSmallString.length(); ++i) {
                output += givenWaveletTree.access(i);
            }

            THEN("Output is equal to givenSmallString") {
                REQUIRE(output == givenSmallString);
            }
        }
    }
}