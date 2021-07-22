/**
  * Author: Rohan
 */
#include <gtest/gtest.h>
#include <stdio.h>
#include <string>

// Include Software Parameters
extern "C" {
#include "load_software_parameters.h"
}

#include "gtest_globals.h"
#include "software_parameters.h"

class LoadSoftwareParameters_fixture : public ::testing::Test {
protected:
    void SetUp() override
    {
        // Reset Software Parameters
        PUMPDOWN_TIMEOUT = 0;
        MAXIMUM_RUN_TIME = 0;
        RUN_RETRO_COUNT = 0;
        BRAKING_CHECK_PRESSURE_TIMEOUT = 0;
        BRAKING_CHECK_RMS_TIMEOUT = 0;
        BRAKING_TRANSITION_STOPPED_TIMEOUT = 0;
        CRAWL_RETRO_COUNT = 0;
        MAXIMUM_CRAWL_TIME = 0;
    }

    void TearDown() override
    {
        // Nothing to do here
    }
};

/**
 * Test nominal parameter loading
 * This is what's called by the actual production code
*/
TEST_F(LoadSoftwareParameters_fixture, VerifyNominalParameters_ExpectSuccess)
{
    int status = loadParameters(executable_path, ACTIVE_RUN_PROFILE);

    // Verify successful status
    EXPECT_EQ(status, 0);

    // Verify expected values
    EXPECT_EQ(PUMPDOWN_TIMEOUT, 300000000);
    EXPECT_EQ(MAXIMUM_RUN_TIME, 30000000);
    EXPECT_EQ(RUN_RETRO_COUNT, 3);
    EXPECT_EQ(BRAKING_CHECK_PRESSURE_TIMEOUT, 5000000);
    EXPECT_EQ(BRAKING_CHECK_RMS_TIMEOUT, 10000000);
    EXPECT_EQ(BRAKING_TRANSITION_STOPPED_TIMEOUT, 15000000);
    EXPECT_EQ(CRAWL_RETRO_COUNT, 2);
    EXPECT_EQ(MAXIMUM_CRAWL_TIME, 5000000);
}

/**
 * Test parameter loading against a constant static file case
*/
TEST_F(LoadSoftwareParameters_fixture, VerifyKnownParameters_ExpectSuccess)
{
    int status = loadParameters(executable_path, (char*)"../Test/run_profiles/Test_profiles/nominal_profile.txt");

    // Verify successful status
    EXPECT_EQ(status, 0);

    // Verify expected values
    EXPECT_EQ(PUMPDOWN_TIMEOUT, 300000000);
    EXPECT_EQ(MAXIMUM_RUN_TIME, 30000000);
    EXPECT_EQ(RUN_RETRO_COUNT, 3);
    EXPECT_EQ(BRAKING_CHECK_PRESSURE_TIMEOUT, 5000000);
    EXPECT_EQ(BRAKING_CHECK_RMS_TIMEOUT, 10000000);
    EXPECT_EQ(BRAKING_TRANSITION_STOPPED_TIMEOUT, 15000000);
    EXPECT_EQ(CRAWL_RETRO_COUNT, 2);
    EXPECT_EQ(MAXIMUM_CRAWL_TIME, 5000000);
}

/**
 * Test failure condition where an invalid filepath is provided
*/
TEST_F(LoadSoftwareParameters_fixture, VerifyNoFile_ExpectFailure)
{
    int status = loadParameters(executable_path, (char*)"../Test/run_profiles/Test_profiles/does_not_exist.txt");

    // Verify failure status
    EXPECT_EQ(status, 1);

    // Verify expected values
    EXPECT_EQ(PUMPDOWN_TIMEOUT, 0);
    EXPECT_EQ(MAXIMUM_RUN_TIME, 0);
    EXPECT_EQ(RUN_RETRO_COUNT, 0);
    EXPECT_EQ(BRAKING_CHECK_PRESSURE_TIMEOUT, 0);
    EXPECT_EQ(BRAKING_CHECK_RMS_TIMEOUT, 0);
    EXPECT_EQ(BRAKING_TRANSITION_STOPPED_TIMEOUT, 0);
    EXPECT_EQ(CRAWL_RETRO_COUNT, 0);
    EXPECT_EQ(MAXIMUM_CRAWL_TIME, 0);
}

/**
 * Test condition where an invalid value is in the profile file
 * 
 * Additional sanity checking for parameter values post-load may be necessary
*/
TEST_F(LoadSoftwareParameters_fixture, VerifyInvalidValue_ExpectSuccess)
{
    int status = loadParameters(executable_path, (char*)"../Test/run_profiles/Test_profiles/bad_values_profile.txt");

    // Verify successful status
    EXPECT_EQ(status, 0);

    // Verify expected values
    EXPECT_EQ(PUMPDOWN_TIMEOUT, 300000000);
    EXPECT_EQ(MAXIMUM_RUN_TIME, 30000000);
    EXPECT_EQ(RUN_RETRO_COUNT, 3);
    EXPECT_EQ(BRAKING_CHECK_PRESSURE_TIMEOUT, 5000000);
    EXPECT_EQ(BRAKING_TRANSITION_STOPPED_TIMEOUT, 15000000);
    EXPECT_EQ(CRAWL_RETRO_COUNT, 2);
    EXPECT_EQ(MAXIMUM_CRAWL_TIME, 5000000);

    // Verify expected 0 for invalid value
    EXPECT_EQ(BRAKING_CHECK_RMS_TIMEOUT, 0);
}

TEST_F(LoadSoftwareParameters_fixture, VerifyInvalidFormat_ExpectFailure)
{
    int status = loadParameters(executable_path, (char*)"../Test/run_profiles/Test_profiles/invalid_format_profile.txt");

    // Verify failure status
    EXPECT_EQ(status, 1);

    // All other values would be garbage
}