#include "variant.h"

int main() {
    std::cout << mpl::index_of<int, double, long, long>() << std::endl;

    Variant<int, std::string> v("hello");
    //std::cout << v.get<std::string>();



    Variant<int, std::string> v2(v);
    std::cout << "v2" <<  v2.get<std::string>() << std::endl;
    v2 = Variant<int, std::string>(5);
    std::cout << "v2" <<  v2.get<int>() << std::endl;

    Variant<int, std::string> v3(std::move(v2));
    std::cout << "v3: " <<  v3.get<int>() << std::endl;


    struct Visitor {
        void operator()(const std::string& s) {
            std::cout << "i am string " << s;
        }

        void operator()(int i) {
            std::cout << "i am int" << i;
        }
    };

    v.visit(Visitor());
}