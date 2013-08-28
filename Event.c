
#include <sol/runtime.h>

SOLAR_DEFINITION(Event_initWithType) {
    sol_obj_set_prop(self, "type", arguments->first->value);
    return sol_obj_retain(self);
}
