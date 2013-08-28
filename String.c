#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sol/solar.h>
#include <sol/runtime.h>

SOLAR_DEFINITION(String_el) {
    if (arguments->length < 1)
        SOL_THROW_MISSING_ARGUMENT("index");
    SOL_REQUIRE_DATATYPE(arguments->first->value, DATA_TYPE_NUM);
    
    char* str = ((SolString) self)->value;
    int len = strlen(str);
    
    int i = ((SolNumber) arguments->first->value)->value;
    if (i < 0) i = len + i;
    SOL_REQUIRE_BOUNDS(i, 0, len - 1);
    
    char* el = malloc(sizeof(*el) * 2);
    el[0] = str[i];
    el[1] = '\0';
    SolString ret = (SolString) sol_obj_retain((SolObject) sol_string_create(el));
    free(el);
    return (SolObject) ret;
}

SOLAR_DEFINITION(String_index_of) {
    if (arguments->length < 1)
        SOL_THROW_MISSING_ARGUMENT("search");
    SOL_REQUIRE_DATATYPE(arguments->first->value, DATA_TYPE_STR);
    char* this = ((SolString) self)->value;
    char* search = ((SolString) arguments->first->value)->value;
    char* result = strstr(this, search);
    return result
        ? sol_obj_retain((SolObject) sol_num_create(result - this))
        : sol_obj_retain((SolObject) sol_bool_create(false));
}

SOLAR_DEFINITION(String_length) {
    return sol_obj_retain((SolObject) sol_num_create(strlen(((SolString) self)->value)));
}

SOLAR_DEFINITION(String_slice) {
    if (arguments->length < 1)
        SOL_THROW_MISSING_ARGUMENT("start");
    SOL_REQUIRE_DATATYPE(arguments->first->value, DATA_TYPE_NUM);
    if (arguments->length >= 2)
        SOL_REQUIRE_DATATYPE(arguments->first->next->value, DATA_TYPE_NUM);
    
    char* str = ((SolString) self)->value;
    int len = strlen(str);
    
    int start = ((SolNumber) arguments->first->value)->value;
    int end = arguments->first->next ? ((SolNumber) arguments->first->next->value)->value : len;
    if (start < 0) {
        start = len + start;
        if (end == 0) end = len;
    }
    if (end < 0) end = len + end;
    
    // swap start/end if necessary
    if (start > end) {
        int tmp = start;
        start = end;
        end = tmp;
    }
    
    SOL_REQUIRE_BOUNDS(start, 0, len - 1);
    SOL_REQUIRE_BOUNDS(end, 0, len);
    
    int newlen = end - start;
    char* ret = malloc(newlen + 1);
    memcpy(ret, str + start, newlen);
    ret[newlen] = '\0';
    SolString ret_value = (SolString) sol_obj_retain((SolObject) sol_string_create(ret));
    free(ret);
    return (SolObject) ret_value;
}

SOLAR_DEFINITION(String_split) {
    char* splitter = ((SolString) arguments->first->value)->value;
    char* str = strdup(((SolString) self)->value);
    SolList split = (SolList) sol_obj_retain((SolObject) sol_list_create(false));
    
    for (char* el = strtok(str, splitter); el != NULL; el = strtok(NULL, splitter)) {
        sol_list_add_obj(split, (SolObject) sol_string_create(el));
    }
    
    free(str);
    return (SolObject) split;
}

SOLAR_DEFINITION(String_to_number) {
    char* this = ((SolString) self)->value;
    
    // validate format
    char* str = this;
    if (*str == '-') str++;
    while (isdigit(*str)) str++;
    if (*str == '.') str++;
    while (isdigit(*str)) str++;
    
    // return false on failure
    if (*str != '\0') return sol_obj_retain((SolObject) sol_bool_create(false));
    
    double value;
    sscanf(this, "%lf", &value);
    return sol_obj_retain((SolObject) sol_num_create(value));
}

SOLAR_DEFINITION(String_to_token) {
    SOL_REQUIRE_DATATYPE(self, DATA_TYPE_STR);
    return sol_obj_retain((SolObject) sol_token_create(((SolString) self)->value));
}

SOLAR_DEFINITION(String_trim) {
    char* this = ((SolString) self)->value;
    int len = strlen(this);
    char* end = this + len - 1;
    
    while (isspace(*this)) this++;
    while (end > this && isspace(*end)) end--;
    int newlen = end - this + 1;
    
    char* result = memcpy(malloc(newlen + 1), this, newlen);
    result[newlen] = '\0';
    SolString ret = (SolString) sol_obj_retain((SolObject) sol_string_create(result));
    free(result);
    return (SolObject) ret;
}

SOLAR_DEFINITION(String_is_digit) {
    char* str = ((SolString) self)->value;
    while (*str != '\0' && isdigit(*str)) str++;
    return sol_obj_retain((SolObject) sol_bool_create(*str == '\0'));
}

SOLAR_DEFINITION(String_is_whitespace) {
    char* str = ((SolString) self)->value;
    while (*str != '\0' && isspace(*str)) str++;
    return sol_obj_retain((SolObject) sol_bool_create(*str == '\0'));
}
