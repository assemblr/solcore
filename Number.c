#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sol/solar.h>
#include <sol/runtime.h>

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#    define M_E 2.7182818284590452354
#endif

int solar_init(SolObject exports, SolObject dependencies) {
    sol_obj_set_prop((SolObject) Number, "PI", (SolObject) sol_num_create(M_PI));
    sol_obj_set_prop((SolObject) Number, "TAU", (SolObject) sol_num_create(2 * M_PI));
    sol_obj_set_prop((SolObject) Number, "E", (SolObject) sol_num_create(M_E));
    return 0;
}

#define MATH_DECL(func) MATH_DECL_L(func, func)
#define MATH_DECL_L(name, func) \
    SOLAR_DEFINITION(Number_ ## name) { \
        return sol_obj_retain((SolObject) sol_num_create(func(((SolNumber) self)->value))); \
    }

SOLAR_DEFINITION(Number_random) {
    return sol_obj_retain((SolObject) sol_num_create(rand() / (double) RAND_MAX));
}

MATH_DECL(floor);
MATH_DECL(ceil);
MATH_DECL(round);
MATH_DECL(sqrt);

MATH_DECL(sin);
MATH_DECL(cos);
MATH_DECL(tan);
MATH_DECL(asin);
MATH_DECL(acos);
MATH_DECL(atan);
MATH_DECL(sinh);
MATH_DECL(cosh);
MATH_DECL(tanh);

MATH_DECL_L(abs, fabs);
MATH_DECL_L(ln, log);

SOLAR_DEFINITION(Number_pow) {
    return sol_obj_retain((SolObject) sol_num_create(pow(((SolNumber) self)->value, ((SolNumber) arguments->first->value)->value)));
}
