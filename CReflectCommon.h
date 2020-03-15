#ifndef CREFLECTCOMMON_H
#define CREFLECTCOMMON_H



#undef STRUCT_LIST

#undef STRUCT_NAME
#define STRUCT_NAME(type, name)     type name


#define COTENTS(...)                __VA_ARGS__

#define DEFINE_STRUCT(name) struct name{ COTENTS(STRUCT_LIST) };

#define TP(t)                       t*

#define ToStr(str)                  #str
#define CBOOL                       ToStr(bool)
#define CCHAR                       ToStr(char)
#define CSHORT                      ToStr(short)
#define CINT                        ToStr(int)
#define CLLINT                      ToStr(long long int)
#define CFLOAT                      ToStr(float)
#define CDOUBLE                     ToStr(double)
#define CSTRING                     ToStr(string)
#define COBJECT                     ToStr(CrfObject)
#define CUCHAR                      ToStr(unsigned char)
#define CUSHORT                     ToStr(unsigned short)
#define CUINT                       ToStr(unsigned int)
#define CULLINT                     ToStr(unsigned long long int)

#define CmpStr(str1, str2)          (strcmp(str1, str2) == 0)
#define IsArray(str)                (strchr(str, '['))
#define IsPointer(str)              (strchr(str, '*'))

#define CmpBaseType(type)   \
(  \
    CmpStr(type, CBOOL) ||  \
    CmpStr(type, CCHAR) ||  \
    CmpStr(type, CUCHAR) ||  \
    CmpStr(type, CSHORT) || \
    CmpStr(type, CUSHORT) ||  \
    CmpStr(type, CINT) ||   \
    CmpStr(type, CUINT) ||  \
    CmpStr(type, CLLINT) || \
    CmpStr(type, CULLINT) ||  \
    CmpStr(type, CFLOAT) ||   \
    CmpStr(type, CDOUBLE)   \
)


#define TBOOL(p)                     (*(bool*)(p))
#define TCHAR(p)                     (*(char*)(p))
#define TSHORT(p)                    (*(short*)(p))
#define TINT(p)                      (*(int*)(p))
#define TLLINT(p)                    (*(long long int*)(p))
#define TFLOAT(p)                    (*(float*)(p))
#define TDOUBLE(p)                   (*(double*)(p))
#define TSTRING(p)                   (*(string*)(p))
#define TUCHAR(p)                    (*(unsigned char*)(p))
#define TUSHORT(p)                   (*(unsigned short*)(p))
#define TUINT(p)                     (*(unsigned int*)(p))
#define TULLINT(p)                   (*(unsigned long long int*)(p))

#define TBOOLP(p)                     ((bool*)(p))
#define TCHARP(p)                     ((char*)(p))
#define TSHORTP(p)                    ((short*)(p))
#define TINTP(p)                      ((int*)(p))
#define TLLINTP(p)                    ((long long int*)(p))
#define TFLOATP(p)                    ((float*)(p))
#define TDOUBLEP(p)                   ((double*)(p))
#define TSTRINGP(p)                   ((string*)(p))
#define TUCHARP(p)                    ((unsigned char*)(p))
#define TUSHORTP(p)                   ((unsigned short*)(p))
#define TUINTP(p)                     ((unsigned int*)(p))
#define TULLINTP(p)                   ((unsigned long long int*)(p))



/*
* 以下宏用来定义需要反射的类成员
*/
#define ROBJADDFIELD(type, name)   \
    type name;                         \
    const CReflectHelper __##name = CReflectHelper(this, &this->name, ToStr(type), ToStr(name), sizeof(type))

#define ROBJADDFIELDARR(type, name, num)   \
    type name[num];                         \
    const CReflectHelper __##name = CReflectHelper(this, &this->name, ToStr(type), ToStr(name), sizeof(type), num)


template<class T>
class CSingleton
{
public:
    using object_type = T;
    struct ObjectCreator_t
    {
        ObjectCreator_t() { CSingleton<T>::instance(); }
    };

    static ObjectCreator_t m_CreatorObject;
public:
    static object_type* instance()
    {
        static object_type _instance;
        return &_instance;
    }
};
template<typename T> typename CSingleton<T>::ObjectCreator_t CSingleton<T>::m_CreatorObject;

//该宏定义实现了一个动态类的创建，
//   ## 合并操作符 将操作两边的字符合并成一个新的标识符，合并后新的标识符不是字符串
//   #  构串操作符 它将实参的字符序列（而不是实参代表的值）转换成字符串常量, 转换后是一个字符串
//   class className##Helper : 如className是FileItem，程序将会生成一个FileItemHelper类。
//   构造函数 : 调用工厂类的注册函数，实现了类名和生成类实例函数的绑定
//   CreatObjFunc函数 : 生成一个类实例   比如className是FileItem,则new FileItem.  返回void *减少了代码的耦合
//  定义了一个成员变量，如FileItemHelper类的成员变量 FileItemhelper
#define CREGISTERCLASS(className) \
class className##Helper { \
public: \
    className##Helper() \
    { \
        CClassFactory::instance()->RegisterItem(#className, className##Helper::CreatObjFunc); \
    } \
    static void* CreatObjFunc() \
    { \
        return new className; \
    } \
}; \
className##Helper className##helper;


#define CClassFactoryCreate(type) static_cast<type *>(CClassFactory::instance()->CreateItem(ToStr(type)))



#endif // CREFLECTCOMMON_H
