#include <stdio.h>
#include <sol/solar.h>
#include <sol/runtime.h>

SOLAR_DEFINITION(Function_apply) {
    SolObject self_arg = arguments->first->value;
    SolList args = arguments->first->next ? (SolList) arguments->first->next->value : (SolList) nil;
    return sol_func_execute((SolFunction) self, args, self_arg);
}

SOLAR_DEFINITION(Function_call) {
    SolObject self_arg = arguments->first->value;
    SolList args = sol_list_slice_s(arguments, 1);
    SolObject ret = sol_func_execute((SolFunction) self, args, self_arg);
    sol_obj_release((SolObject) args);
    return ret;
}
