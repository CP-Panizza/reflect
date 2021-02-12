
#ifndef REFLEX_HPP
#define REFLEX_HPP
#include <map>

class Method{
public:
    Method(std::string name, void * func){

    }
};

class MethodsManager{

};

template <class Type>
class FiledMannager{
public:
    FiledMannager(std::string class_name, Type  fileds) {

    }
};



class ClassInfo;
std::map<std::string, void *> *classMethodMap = NULL;
std::map< std::string, ClassInfo*> *classInfoMap = NULL;
class Object
{
public:
    Object(){}
    virtual ~Object(){}
    static bool Register(ClassInfo* ci);          //注册传入一个classInfo(类信息),将这个类的信息注册到映射中
    static Object* CreateObject(std::string name);     //工厂生产对象的接口
};

typedef Object* (*ObjectConstructorFn)(void);
class ClassInfo
{
public:
    ClassInfo(const std::string className, ObjectConstructorFn ctor)
            :m_className(className), m_objectConstructor(ctor)
    {
        Object::Register(this);             //classInfo的构造函数是传入类名和类对应的new函数然后自动注册进map中。
    }
    virtual ~ClassInfo(){}
    Object* CreateObject() const { return m_objectConstructor ? (*m_objectConstructor)() : 0; }
    bool IsDynamic()const { return NULL != m_objectConstructor; }
    const std::string GetClassName()const { return m_className; }
    ObjectConstructorFn GetConstructor()const{ return m_objectConstructor; }

public:
    std::string m_className;
    ObjectConstructorFn m_objectConstructor;
};


bool Object::Register(ClassInfo* ci)
{
	if (!classInfoMap)   {
		classInfoMap = new std::map< std::string, ClassInfo*>();      //这里我们是通过map来存储这个映射的。
        classMethodMap = new std::map< std::string, void *>();
	}
	if (ci)  {
		if (classInfoMap->find(ci->m_className) == classInfoMap->end()){
			classInfoMap->insert(std::map<std::string, ClassInfo*>::value_type(ci->m_className, ci)); // 类名 <-> classInfo
		}
	}
	return true;
}

Object* Object::CreateObject(std::string name)
{
	std::map< std::string, ClassInfo*>::const_iterator iter = classInfoMap->find(name);
	if (classInfoMap->end() != iter)  {
		return iter->second->CreateObject();         //当传入字符串name后,通过name找到info,然后调用对应的CreatObject()即可
	}
	return NULL;
}


#define DECLARE_CLASS(name)             \
    public:                          \
        static ClassInfo ms_classinfo;  \
        virtual ClassInfo* GetClassInfo() const; \
        static void * CreateObject();

//实现CreatObject 和 GetClassInfo 的两个方法
#define IMPLEMENT_CLASS_COMMON(name,func, ...) \
    ClassInfo name::ms_classinfo((#name), \
             (ObjectConstructorFn) func); \
                                            \
    ClassInfo *name::GetClassInfo() const {return &name::ms_classinfo;} \



    //classInfo 属性的初始化
#define END_DECLARE_CLASS(name)                          \
    IMPLEMENT_CLASS_COMMON(name, name::CreateObject)    \
    void* name::CreateObject()                          \
            { return new name;}                         \



template <class Class>
ClassInfo reflex_get_classinfo(){
        return Class::ms_classinfo;
}


ClassInfo reflex_get_classinfo(std::string class_name){
    return (*classInfoMap).count(class_name) ? *(*classInfoMap)[class_name] : ClassInfo("", nullptr);
}

template <class Class>
void reflex_get_methods(){

}

#endif //REFLEX_HPP