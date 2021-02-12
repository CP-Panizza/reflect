#ifndef _REFLECTION_H_
#define _REFLECTION_H_

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>


template <class T> struct id{};

std::map<int, std::string> id_classname_map;

class REGISTER_TYPE_TOOL{
public:
    REGISTER_TYPE_TOOL(std::string class_name, int id){
        if(!id_classname_map.count(id)){
            id_classname_map[id] = class_name;
        }
    }
};

#define REGISTER_TYPE(Type, Index)             \
     REGISTER_TYPE_TOOL reg_tool##Index(#Type, Index);\
     int type_to_id(id<Type>)  { return Index; } \


REGISTER_TYPE(unsigned short       , 1)
REGISTER_TYPE(unsigned int          , 2)
REGISTER_TYPE(unsigned long long    , 3)
REGISTER_TYPE(signed char           , 4)
REGISTER_TYPE(short                 , 5)
REGISTER_TYPE(int                   , 6)
REGISTER_TYPE(long long             , 7)
REGISTER_TYPE(unsigned char         , 8)
REGISTER_TYPE(char                  , 9)
REGISTER_TYPE(wchar_t               , 10)
REGISTER_TYPE(long                  , 11)
REGISTER_TYPE(unsigned long         , 12)
REGISTER_TYPE(void*                 , 13)
REGISTER_TYPE(const void*           , 14)
REGISTER_TYPE(char16_t              , 15)
REGISTER_TYPE(char32_t              , 16)
REGISTER_TYPE(float                 , 17)
REGISTER_TYPE(double                , 18)
REGISTER_TYPE(long double           , 19)
REGISTER_TYPE(char *                , 20)
REGISTER_TYPE(int *                 , 21)
REGISTER_TYPE(double *              , 22)
REGISTER_TYPE(std::vector<int>      , 23)
REGISTER_TYPE(bool                  , 24)
REGISTER_TYPE(std::string      , 25)


std::map<int, std::size_t > id_size_map;

std::size_t id_to_size(int id){
    if(id_size_map.count(id)){
        return id_size_map[id];
    }else {
        return 0;
    }
}



struct __field_info__{
    std::string field_name;
    int type_id;
    std::string field_type_string;
    std::size_t field_size;
};

struct __class_info__{
    std::string class_name;
    std::vector<__field_info__> fields_info;
};

std::map<std::string, __class_info__> global_class_info;

template <class T>
class init {
public:

    template <class Type>
    operator Type() {
        if(!is_init){
            if(class_name.empty()){
                class_name = current_class_name;
            }
            if(!global_class_info.count(current_class_name)){
                global_class_info[current_class_name] = __class_info__{current_class_name};
            }
            __field_info__ field_info__;
            field_info__.field_name = current_field_name;
            int type_id = type_to_id(id<Type>{});
            field_info__.type_id = type_id;
            field_info__.field_size = sizeof(Type);
            field_info__.field_type_string = id_classname_map[type_id];
            type_ids.push_back(type_id);
            if(id_size_map.count(type_id) == 0){
                id_size_map[type_id] = sizeof(Type);
            }
            field_names.push_back(current_field_name);
            global_class_info[current_class_name].fields_info.push_back(field_info__);
        }
        return Type{};
    }
    template <class Type>
    static void Init(Type){
        is_init = true;
    }
    std::string current_field_name;
    std::string current_class_name;
    static std::vector<int> type_ids;
    static bool is_init;
    static std::vector<std::string> field_names;
    static std::string class_name;
};

//template <class T>
//std::vector<int> *init<T>::type_ids = new std::vector<int>;
template <class T>
std::vector<int> init<T>::type_ids;

//template <class T>
//std::vector<std::string> *init<T>::field_names = new std::vector<std::string>;
template <class T>
std::vector<std::string> init<T>::field_names;

template <class T>
std::string init<T>::class_name;


template <class T>
bool init<T>::is_init = false;

#define INIT_FIELD(Class, field_name) (init<Class>{field_name, #Class})

#define INIT_TYPE(Class) init<Class>::Init(Class{})

//#define OUT_INIT_FIELD(Type , Class, field_name, idx) Type Class##idx = init<Class>{field_name};


template <class Type>
__class_info__ get_class_info(){
    std::string cn = init<Type>::class_name;
    return global_class_info[cn];
}

__class_info__ get_class_info(std::string class_name){
    if(global_class_info.count(class_name)){
        return global_class_info[class_name];
    }
    return __class_info__{};
}

template <class Type>
int get_fields_count(){
    return static_cast<int>(init<Type>::type_ids.size());
}


template <class Type, class Res>
Res get_field_value_by_idx(Type &type, int idx){
    if(idx == 0){
        return *((Res*)((size_t)&type));
    }

    int offset = 0;
    for (int i = 0; i < idx; ++i) {
        int s = static_cast<int>(id_to_size((init<Type>::type_ids)[i]));
        for (int j = 0; j < s; ++j) {
            offset++;
        }
    }
    return *((Res*)((size_t)&type + (size_t)offset));
}

template <class Type>
std::string get_field_type_by_idx(int idx){
    return id_classname_map[(init<Type>::type_ids)[idx]];
}

template <class Type>
std::string get_field_name_by_idx(int idx){
    return (init<Type>::field_names)[idx];
}


template <class Type, class ValType>
void set_field_value_by_idx(Type &type, int idx, ValType val){
    if(idx == 0){
        *((ValType*)((size_t)&type)) = val;
    }

    int offset = 0;
    for (int i = 0; i < idx; ++i) {
        auto s = static_cast<int>(id_to_size((init<Type>::type_ids)[i]));
        for (int j = 0; j < s; ++j) {
            offset++;
        }
    }
    *((ValType*)((size_t)&type + (size_t)offset)) = val;
}






#endif //_REFLECTION_H_