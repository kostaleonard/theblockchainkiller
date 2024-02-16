/**
 * @brief Contains test suite file paths.
 */

#ifndef TESTS_FILE_PATHS_H_
#define TESTS_FILE_PATHS_H_
#include "include/return_codes.h"
// Windows has a MAX_PATH of 260. Many versions of Unix have a PATH_MAX of 4096.
// We want the test suite to run on many systems, so we will use 260.
#define TESTS_MAX_PATH 260

/**
 * @brief Fills dirname with the path to the fixture directory.
 * 
 * The fixture directory is located at tests/fixtures. This function assumes
 * that the executable is located in the build directory.
 * 
 * @param dirname A character array of length TESTS_MAX_PATH to fill with the
 * path.
 * @return A return code indicating success or failure.
 */
return_code_t get_fixture_directory(char *dirname);

/**
 * @brief Fills dirname with the path to the output directory.
 * 
 * The output directory is located at tests/output. This function assumes
 * that the executable is located in the build directory.
 * 
 * @param dirname A character array of length TESTS_MAX_PATH to fill with the
 * path.
 * @return A return code indicating success or failure.
 */
return_code_t get_output_directory(char *dirname);

#endif  // TESTS_FILE_PATHS_H_
