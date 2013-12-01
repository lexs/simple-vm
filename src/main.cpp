#include <memory>

#include "op_codes.h"
#include "vm.h"
#include "builder.h"
#include "array.h"
#include "native_types.h"

Object number(number_t value) { return NumberType::create(value); }
Object function(Function* function) { return FunctionType::create(function); }
Object array(const std::shared_ptr<Array>& array) { return ArrayType::create(array); }

int main() {
    FunctionBuilder builder;

    Register r_array = builder.create_register();
    Register r_len= builder.create_register();
    Register r_range = builder.create_register();
    Register r_i = builder.create_register();
    Register r_index = builder.create_register();
    Register r_temp = builder.create_register();
    Register r_temp2 = builder.create_register();

    Constant k_0 = builder.create_constant(number(0));
    Constant k_1 = builder.create_constant(number(1));
    Constant k_2 = builder.create_constant(number(2));
    Constant k_fib = builder.self();

    instr_t i_reverse[] = {
        //new_array(r_temp),
        len(r_len, r_array),
        sub(r_len, r_len, k_1),
        div(r_range, r_len, k_2),
        loadk(r_i, k_0),
        le(r_range, r_i),
        jump(6),

        get(r_temp, r_array, r_i), // temp = array[i]
        sub(r_index, r_len, r_i),
        get(r_temp2, r_array, r_index),
        set(r_array, r_i, r_temp2),
        set(r_array, r_index, r_temp),
        add(r_i, r_i, k_1),


        ret(r_array, 1)
    };

    Function* reverse = builder.build(i_reverse);

    Object c_print[] = {
        function(reverse)
    };

    instr_t i_print[] = {
        loadk(r(1), k(0)),
        call(r(1), r(0), 1),
        print(r(0)),
        ret()
    };

    Function print(2, c_print, i_print);

    Vm vm(&print);
    vm.execute({array(std::shared_ptr<Array>(new Array {number(5), number(3), number(7)}))});

    return 0;
}

int main2() {
    /*
    int r_fib = r(0); // Arg 1
    int r_n1 = r(1);
    int r_n2 = r(2);
    int r_n = r(3);
    int r_counter = r(4);

    Value constants[] = {
        number(0), // k(0)
        number(1), // k(1)
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
    Constant k_1 = builder.create_constant(number(1));
    Constant k_2 = builder.create_constant(number(2));

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

    Object c_print[] = {
        function(fib_recur)
    };

    instr_t i_print[] = {
        loadk(r(1), k(0)),
        call(r(1), r(0), 1),
        print(r(0)),
        ret()
    };

    Function print(2, c_print, i_print);

    Vm vm(&print);
    vm.execute({number(20)});

    return 0;
}
