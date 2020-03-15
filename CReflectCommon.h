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
* ���º�����������Ҫ��������Ա
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

//�ú궨��ʵ����һ����̬��Ĵ�����
//   ## �ϲ������� ���������ߵ��ַ��ϲ���һ���µı�ʶ�����ϲ����µı�ʶ�������ַ���
//   #  ���������� ����ʵ�ε��ַ����У�������ʵ�δ����ֵ��ת�����ַ�������, ת������һ���ַ���
//   class className##Helper : ��className��FileItem�����򽫻�����һ��FileItemHelper�ࡣ
//   ���캯�� : ���ù������ע�ắ����ʵ����������������ʵ�������İ�
//   CreatObjFunc���� : ����һ����ʵ��   ����className��FileItem,��new FileItem.  ����void *�����˴�������
//  ������һ����Ա��������FileItemHelper��ĳ�Ա���� FileItemhelper
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
