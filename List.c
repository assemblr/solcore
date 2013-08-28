#include <stdio.h>
#include <sol/solar.h>
#include <sol/runtime.h>

SOLAR_DEFINITION(List_el) {
    return sol_list_get_obj((SolList) self, ((SolNumber) arguments->first->value)->value);
}

SOLAR_DEFINITION(List_length) {
    return sol_obj_retain((SolObject) sol_num_create(((SolList) self)->length));
}

SOLAR_DEFINITION(List_add) {
    if (arguments->length > 1) {
        sol_list_insert_object((SolList) self, arguments->first->value, (int) ((SolNumber) arguments->first->next->value)->value);
    } else {
        sol_list_add_obj((SolList) self, arguments->first->value);
    }
    return sol_obj_retain(arguments->first->value);
}

SOLAR_DEFINITION(List_remove) {
    return arguments->length == 0 ? sol_list_remove_obj(((SolList) self), 0) : sol_list_remove_obj(((SolList) self), (int) ((SolNumber) arguments->first->value)->value);
}

SOLAR_DEFINITION(List_index) {
    int index = sol_list_index_of(((SolList) self), arguments->first->value);
    return index != -1 ? sol_obj_retain((SolObject) sol_num_create(index)) : nil;
}

SOLAR_DEFINITION(List_slice) {
    int start = ((SolNumber) arguments->first->value)->value;
    int end = arguments->length >= 2 ? ((SolNumber) arguments->first->next->value)->value : ((SolList) self)->length;
    return (SolObject) sol_list_slice((SolList) self, start, end);
}

SOLAR_DEFINITION(List_iterate) {
    SolFunction func = (SolFunction) arguments->first->value;
    SolList list = (SolList) self;
    SolObject result = nil;
    SOL_LIST_ITR(list, current, i) {
        SolList args = (SolList) sol_obj_retain((SolObject) sol_list_create(false));
        sol_list_add_obj(args, current->value);
        sol_list_add_obj(args, (SolObject) sol_num_create(i));
        sol_obj_release(result);
        result = sol_func_execute(func, args, nil);
        sol_obj_release((SolObject) args);
    }
    return result;
}
