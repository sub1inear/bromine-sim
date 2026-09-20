#include "system/hot.h"
#include "kapi.h"
#include "v5_api.h"

struct hot_table* const HOT_TABLE = NULL;

// this expands to a bunch of:
// extern void autonomous();
#define FUNC(F) void F();
#include "system/user_functions/list.h"
#undef FUNC
