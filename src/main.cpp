#include "op_codes.h"
#include "vm.h"
#include "builder.h"

int main() {
    /*
    int r_fib = r(0); // Arg 1
    int r_n1 = r(1);
    int r_n2 = r(2);
    int r_n = r(3);
    int r_counter = r(4);

    Value constants[] = {
        Value::number(0), // k(0)
        Value::number(1), // k(1)
    };

    instr_t fib_instructions[] = {
        loadk(r_counter, k(0)),

        loadk(r_n2, k(0)),
        loadk(r_n1, k(1)),

        sub(r_fib, r_fib, k(1)), // We start at the 1st fib number

        add(r_n, r_n1, r_n2),
        move(r_n2, r_n1),
        move(r_n1, r_n),
        add(r_counter, r_counter, k(1)),
        lt(r_counter, r_fib),
        jump(-6),

        print(r_n),
        ret()
    };

    Function fib(5, constants, fib_instructions);

    */

    FunctionBuilder builder;

    Register r_n = builder.create_register();
    Register r_fib = builder.create_register();
    Register r_n1 = builder.create_register();
    Register r_n2 = builder.create_register();

    Constant k_fib = builder.self();
    Constant k_1 = builder.create_constant(Value::number(1));
    Constant k_2 = builder.create_constant(Value::number(2));

    instr_t i_fib[] = {
        loadk(r_fib, k_fib),
        lt(r_n, k_2),           // if n < 2
        jump(5),                // jump to ret

        sub(r_n1, r_n, k_1),    // n1 = n - 1
        call(r_fib, r_n1, 1),   // n1 = fib(n1)

        sub(r_n2, r_n, k_2),    // n2 = n - 2
        call(r_fib, r_n2, 1),   // n2 = fib(n2)

        add(r_n, r_n1, r_n2),   // n = n1 + n2

        ret(r_n, 1)             // return n
    };

    Function* fib_recur = builder.build(i_fib);

    Value c_print[] = {
        Value::function(fib_recur)
    };

    instr_t i_print[] = {
        loadk(r(1), k(0)),
        call(r(1), r(0), 1),
        print(r(0)),
        ret()
    };

    Function print(2, c_print, i_print);

    Vm vm(&print);
    vm.execute({Value::number(20)});

    return 0;
}
