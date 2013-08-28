#include <stdio.h>
#include <sol/solar.h>
#include <sol/runtime.h>

SOLAR_DEFINITION(Object_is) {
    SolObject type = arguments->first->value;
    do {
        if (self == type) return sol_obj_retain((SolObject) sol_bool_create(true));
    } while ((self = self->parent));
    return sol_obj_retain((SolObject) sol_bool_create(false));
}
