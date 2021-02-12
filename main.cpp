#include "reflection.h"

#define  GetAttrOffSet(CLASSTYPE, Attr)  (size_t)(&((CLASSTYPE*)0x000)->Attr)
REGISTER_TYPE(std::vector<float>, 26)
#pragma pack(1)
template <class Type>
struct Serialize{
    static std::string serialize(Type *obj){
        for (int i = 0; i < get_fields_count<Type>(); ++i) {
            std::cout << get_field_name_by_idx<Type>(i)
                    << " ,type:" << get_field_type_by_idx<Type>(i)
                    << " ,size: " << id_to_size((init<Type>::type_ids)[i])
                    << "\n";
        }
        return "sd";
    }
    static void deserialize(){

    }
    };
#pragma pack()

#pragma pack(1) //让结构体不进行内存对齐
class Data {
public:
    Data(){

    }

    Data(short a, short b, char c, double d, const std::string &name) : a(a), b(b), c(c), d(d), name(name) {}

    ~Data(){

    }

    short a = INIT_FIELD(Data, "age");
    short b = INIT_FIELD(Data, "year");
    char c = INIT_FIELD(Data, "char");
    double d = INIT_FIELD(Data, "price");
    std::string name = INIT_FIELD(Data, "name");
    std::vector<float> v = INIT_FIELD(Data, "vec");
    bool flag = INIT_FIELD(Data, "flag");
};


struct MyData{
    MyData() {}

    MyData(int age, const std::string &name) : age(age), name(name) {}

    int age = INIT_FIELD(MyData, "age");
    std::string name = INIT_FIELD(MyData, "name");
};


#pragma pack()





int main(){
    INIT_TYPE(MyData);
    INIT_TYPE(Data);
    Data data(100, 23, 'a', 1.0, "cmj iii");
    data.v.push_back(1000);
    set_field_value_by_idx<Data, std::string>(data, 4, "121w2w");
    set_field_value_by_idx<Data, bool>(data, 6, true);
    std::cout << get_field_value_by_idx<Data, bool>(data, 6) << "\n";
    std::cout << get_field_value_by_idx<Data, char *>(data, 4) << "\n";
    set_field_value_by_idx<Data, std::vector<int>>(data, 5, std::vector<int>{1998, 2004});
    std::cout << get_field_value_by_idx<Data, std::vector<int>>(data, 5)[1] << "\n";


    Serialize<Data>::serialize(&data);

    MyData myData{100, "cmj"};
    MyData myData1{200, "hhh"};
    set_field_value_by_idx<MyData, std::string>(myData, 1, "wocaonidaya");
    std::cout << get_field_value_by_idx<MyData, std::string>(myData, 1) << "\n";

    std::cout << get_class_info("Data").class_name << "\n";
    std::cout << get_class_info<Data>().fields_info[0].field_type_string << "\n";

    return 0;
}
