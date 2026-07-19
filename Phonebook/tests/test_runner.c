#include <check.h>
#include <stdio.h>
#include <stdlib.h>

// Объявления функций создания наборов тестов
Suite* contact_suite(void);
Suite* phonebook_suite(void);
Suite* validator_suite(void);
Suite* utiles_suite(void);

int main(void) {
    int number_failed = 0;
    SRunner *sr = srunner_create(NULL);
    
    // Добавляем все наборы тестов
    srunner_add_suite(sr, contact_suite());
    srunner_add_suite(sr, phonebook_suite());
    srunner_add_suite(sr, validator_suite());
    srunner_add_suite(sr, utiles_suite());
    
    // Запускаем тесты
    printf("\n════════════════════════════════════════════\n");
    printf("        TELEPHONE BOOK UNIT TESTS\n");
    printf("════════════════════════════════════════════\n\n");
    
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    
    printf("\n════════════════════════════════════════════\n");
    if (number_failed == 0) {
        printf(" ALL TESTS PASSED SUCCESSFULLY!\n");
    } else {
        printf(" SOME TESTS FAILED (%d failures)\n", number_failed);
    }
    printf("════════════════════════════════════════════\n");
    
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}