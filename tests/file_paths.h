/**
 * @brief Contains test suite file paths.
 */

#ifndef TESTS_FILE_PATHS_H_
#define TESTS_FILE_PATHS_H_

/**
 * @brief Fills dirname with the path to the fixture directory.
 * 
 * @param dirname A character array of length MAX_PATH to fill with the path.
 * @return 0 on success, 1 on failure.
 */
int get_fixture_directory(char *dirname);

#endif  // TESTS_FILE_PATHS_H_
