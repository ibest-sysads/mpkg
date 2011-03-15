#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"

/* Pointer to the file used by the tests. */
/*static FILE* temp_file = NULL;*/
#include "test-config.h"
#include "config.h"

/* The suite initialization function.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite(void)
{
	return 0;
}

/* The suite cleanup function.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite(void)
{
	return 0;
}


/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("test_config", init_suite, clean_suite);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "test of testConfigParseVars()",  testConfigParseVars))
	|| (NULL == CU_add_test(pSuite, "test of testConfigParseEnvironment()",  testConfigParseEnvironment))
	)
   {
      printf("Failed to add test?\n");
      CU_cleanup_registry();
      return CU_get_error();
   }


   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

