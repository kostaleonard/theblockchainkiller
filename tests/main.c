/**
 * @brief Runs the unit test suite.
 */

#include <ftw.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include <cmocka.h>
#include "tests/file_paths.h"
#include "tests/test_linked_list.h"
#include "tests/test_block.h"
#include "tests/test_blockchain.h"
#include "tests/test_transaction.h"
#include "tests/test_base64.h"
#include "tests/test_endian.h"

int unlink_callback(
    const char *fpath,
    const struct stat *sb,
    int typeflag,
    struct FTW *ftwbuf)
{
    int return_value = remove(fpath);
    if (return_value) {
        perror(fpath);
    }
    return return_value;
}

int create_empty_output_directory(char *dirname) {
    int nopenfd = 64;
    int return_value = nftw(
        dirname,
        unlink_callback,
        nopenfd,
        FTW_DEPTH | FTW_PHYS);
    if (0 != return_value) {
        goto end;
    }
    return_value = mkdir(dirname);
end:
    return return_value;
}

int main(int argc, char **argv) {
    // TODO use path to executable?
    int return_value = create_empty_output_directory(TEST_OUTPUT_DIR);
    if (0 != return_value) {
        goto end;
    }
    const struct CMUnitTest tests[] = {
        // test_linked_list.h
        cmocka_unit_test(test_linked_list_create_gives_linked_list),
        cmocka_unit_test(test_linked_list_create_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_create_compare_function_may_be_null),
        cmocka_unit_test(test_linked_list_destroy_empty_list_returns_success),
        cmocka_unit_test(
            test_linked_list_destroy_nonempty_list_returns_success),
        cmocka_unit_test(test_linked_list_destroy_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_prepend_adds_node_to_front),
        cmocka_unit_test(test_linked_list_prepend_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_get_first_fails_on_empty_list),
        cmocka_unit_test(test_linked_list_get_first_gives_head_of_list),
        cmocka_unit_test(test_linked_list_get_first_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_remove_first_fails_on_empty_list),
        cmocka_unit_test(test_linked_list_remove_first_removes_head),
        cmocka_unit_test(test_linked_list_remove_first_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_is_empty_gives_true_if_no_elements),
        cmocka_unit_test(
            test_linked_list_is_empty_gives_false_if_list_has_elements),
        cmocka_unit_test(test_linked_list_is_empty_fails_on_invalid_input),
        cmocka_unit_test(
            test_linked_list_find_succeeds_and_gives_null_if_list_empty),
        cmocka_unit_test(
            test_linked_list_find_succeeds_and_gives_null_if_no_match),
        cmocka_unit_test(test_linked_list_find_gives_first_matching_element),
        cmocka_unit_test(test_linked_list_find_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_find_fails_on_null_compare_function),
        cmocka_unit_test(test_linked_list_append_adds_node_to_back),
        cmocka_unit_test(test_linked_list_append_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_get_last_fails_on_empty_list),
        cmocka_unit_test(test_linked_list_get_last_gives_last_element),
        cmocka_unit_test(test_linked_list_get_last_fails_on_invalid_input),
        cmocka_unit_test(test_linked_list_length_gives_zero_on_empty_list),
        cmocka_unit_test(
            test_linked_list_length_gives_num_elements_on_nonempty_list),
        cmocka_unit_test(test_linked_list_length_fails_on_invalid_input),
        // test_block.h
        cmocka_unit_test(test_block_create_gives_block),
        cmocka_unit_test(test_block_create_fails_on_invalid_input),
        cmocka_unit_test(
            test_create_genesis_block_gives_block_with_genesis_values),
        cmocka_unit_test(test_create_genesis_block_fails_on_invalid_input),
        cmocka_unit_test(test_block_destroy_returns_success),
        cmocka_unit_test(test_block_destroy_fails_on_invalid_input),
        cmocka_unit_test(test_block_hash_gives_nonempty_hash),
        cmocka_unit_test(test_block_hash_same_fields_gives_same_hash),
        cmocka_unit_test(test_block_hash_created_at_included_in_hash),
        cmocka_unit_test(test_block_hash_transactions_included_in_hash),
        cmocka_unit_test(
            test_block_hash_multiple_transactions_included_in_hash),
        cmocka_unit_test(test_block_hash_proof_of_work_included_in_hash),
        cmocka_unit_test(test_block_hash_previous_block_hash_included_in_hash),
        cmocka_unit_test(test_block_hash_fails_on_invalid_input),
        // test_blockchain.h
        cmocka_unit_test(test_blockchain_create_gives_blockchain),
        cmocka_unit_test(test_blockchain_create_fails_on_invalid_input),
        cmocka_unit_test(test_blockchain_destroy_returns_success),
        cmocka_unit_test(test_blockchain_destroy_fails_on_invalid_input),
        cmocka_unit_test(test_blockchain_add_block_appends_block),
        cmocka_unit_test(test_blockchain_add_block_fails_on_invalid_input),
        cmocka_unit_test(
            test_blockchain_is_valid_block_hash_true_on_valid_hash),
        cmocka_unit_test(
            test_blockchain_is_valid_block_hash_false_on_invalid_hash),
        cmocka_unit_test(
            test_blockchain_is_valid_block_hash_fails_on_invalid_input),
        cmocka_unit_test(
            test_blockchain_mine_block_produces_block_with_valid_hash),
        cmocka_unit_test(test_blockchain_mine_block_fails_on_invalid_input),
        // TODO put these in the correct order
        cmocka_unit_test(test_blockchain_serialize_creates_nonempty_buffer),
        cmocka_unit_test(test_blockchain_serialize_fails_on_invalid_arguments),
        cmocka_unit_test(test_blockchain_write_to_file_creates_nonempty_file),
        cmocka_unit_test(test_blockchain_write_to_file_fails_on_invalid_input),
        // test_transaction.h
        cmocka_unit_test(test_transaction_create_gives_transaction),
        cmocka_unit_test(test_transaction_create_fails_on_invalid_input),
        cmocka_unit_test(test_transaction_destroy_returns_success),
        cmocka_unit_test(test_transaction_destroy_fails_on_invalid_input),
        cmocka_unit_test(test_transaction_generate_signature_gives_signature),
        cmocka_unit_test(
            test_transaction_generate_signature_fails_on_invalid_input),
        cmocka_unit_test(
            test_transaction_verify_signature_identifies_valid_signature),
        cmocka_unit_test(
            test_transaction_verify_signature_identifies_invalid_signature),
        cmocka_unit_test(
            test_transaction_verify_signature_fails_on_invalid_input),
        // test_base64.h
        cmocka_unit_test(test_base64_decode_correctly_decodes),
        cmocka_unit_test(test_base64_decode_fails_on_invalid_input),
        // test_endian.h
        cmocka_unit_test(test_htobe64_correctly_encodes_data),
        cmocka_unit_test(test_betoh64_correctly_decodes_data),
    };
    //return_value = cmocka_run_group_tests(tests, NULL, NULL);
end:
    return return_value;
}
