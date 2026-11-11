#include "library.h"

int main()
{
    Library lib = {0};
    lib.next_book_id = 1;
    lib.next_user_id = 1;
    lib.next_record_id = 1;

    load_library(&lib);
    run_menu(&lib);
    save_library(&lib);

    return 0;
}