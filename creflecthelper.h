#ifndef CREFLECTHELPER_H
#define CREFLECTHELPER_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include <string.h>

#include "ccommon.h"
#include "CReflectCommon.h"

using namespace std;

class CReflectItem;
class CReflectHelper;

string GetNonCompositeTypeStr(const char *str);

class CrfObject
{
public:
    CrfObject(){strcpy(m_BaseName, ToStr(CrfObject));}

    // 判断基类是否是CrfObject类型;
    static bool IsCrfObj(void *p);

    // 结构体字符串转为CrfObject类型;
    static bool StructToCrfObject(const string &str);

    // 获取基类名;
    const char * GetBaseClassName(){return m_BaseName;}
    //用于在运行时获取对象的真实类型名称
    virtual string GetClassName() const;

    // 获取元素列表;
    bool GetList(vector<CReflectItem> &vec);
    // 获取指定的元素;
    CReflectItem GetItem(const string& name);

    // 计算序列化内存大小;
    int GetSerializationSz();
    // 序列化;
    bool Serialization(unsigned char *buf, int sz);
    // 反序列化;
    bool Deserialization(unsigned char *buf, int sz);
    // 打印结构体;
    bool PrintStruct();
    // CrfObject类型转换为struct类型数据;
    bool ToStructData(int sz, void *data);
    bool AssignFromStructData(int sz, void *data);

private:
    char m_BaseName[16];
};



/*
* 反射属性操作类
* set方法用于修改属性
* get方法用于获取属性
*/
class CReflectItem
{
protected:
    int m_Offset;
    const char *m_ItemType;
    string m_ItemName;
    const void *m_Addrp;
    int m_ItemSz;
    int m_ArrNum;


public:
    CReflectItem();
    CReflectItem(int _offset, const char* _type, const string& _name, int tSz, int arrLen = 1);

    CReflectItem &operator = (const CReflectItem &item);

    bool CanUse() const
    {
        return m_ItemType ? true : false;
    }
    bool operator < (const CReflectItem& obj) const
    {
        return m_ItemName < obj.m_ItemName;
    }
    bool operator == (const CReflectItem& obj) const
    {
        return m_ItemName == obj.m_ItemName;
    }

    void SetStartAddr(void *p){m_Addrp = p;}
    //获取成员的偏移地址;
    int GetOffset() const {return m_Offset;}
    //获取成员名称;
    string GetName() const { return m_ItemType ? m_ItemName : ""; }
    //获取成员类型;
    string GetType() const { return m_ItemType ? m_ItemType : ""; }
    // 获取当前元素所在数据结构的起始地址;
    const void *GetStartAddr() const { return m_Addrp; }
    // 获取当前项数据大小;
    int GetSize() const {return m_ItemSz;}
    // 获取当前数组个数;
    int GetArrNum() const {return m_ArrNum;}
    // 获取当前元素所在内存地址;
    void* GetValAddr()const {return ((char*)(m_Addrp) + m_Offset);}


    //获取成员的值;
    string GetVal() const;
    bool GetVal(void *d, int num);

    template<typename T>
    bool GetVal(T &v) const
    {
        bool ret = false;
        void* dest = GetValAddr();

        if(!m_Addrp || m_ArrNum != 1){LogPos(1); return false;}
        if(m_ItemType == NULL){LogPos(1); return false;}

        ret = GetArr(dest, 1, &v);

        return ret;
    }

    template<typename T>
    bool GetVal(T *v, int num) const
    {
        bool ret = false;
        void* dest = GetValAddr();

        if(!m_Addrp){LogPos(1); return false;}
        if(m_ItemType == NULL){LogPos(1); return false;}
        if(num > m_ArrNum) {LogPos(1); return false;}

        ret = GetArr(dest, num, &v);

        return ret;
    }

    //修改成员的值
    bool SetVal(void *d, int num);

    template<typename T>
    bool SetVal(T v)
    {
        bool ret = false;
        void* dest = GetValAddr();

        if(!m_Addrp || m_ArrNum != 1){LogPos(1); return false;}
        if (m_ItemType == NULL){LogPos(1); return false;}

        ret = SetArr(dest, 1, &v);

        return ret;
    }

    template<typename T>
    bool SetVal(T *v, int num)
    {
        bool ret = false;
        void* dest = GetValAddr();

        if(!m_Addrp){LogPos(1); return false;}
        if(m_ItemType == NULL){LogPos(1); return false;}
        if(num > m_ArrNum) {LogPos(1); return false;}

        ret = SetArr(dest, num, &v);

        return ret;
    }

private:

    template<typename T>
    bool GetArr(void *data, int num, T *dst) const
    {
        T *src = (T*)(data);
        string type = GetNonCompositeTypeStr(m_ItemType);
//        if(!CmpBaseType(type.c_str())){LogPos(1); return false;}

        for(int i=0; i<num; i++){
            dst[i] = src[i];
        }

        return true;
    }

    template<typename T>
    bool SetArr(void *data, int num, T *src)
    {
        T *dst = (T*)(data);
        if(num > 1 && !IsArray(m_ItemType)){LogPos(1); return false;}

        string type = GetNonCompositeTypeStr(m_ItemType);
//        if(!CmpBaseType(type.c_str())){LogPos(1); return false;}

        for(int i=0; i<num; i++){
            dst[i] = src[i];
        }

        return true;
    }

};

/*
* 反射属性管理类
* 以map存储相关类成员变量的类型、偏移地址等信息
*/
class CReflectHelper
{
    typedef map<string, set<CReflectItem> > CReflectItemMap;
    typedef CReflectItemMap::iterator CReflectItemMapIt;
    typedef set<CReflectItem> CReflectItemSet;
    typedef CReflectItemSet::iterator CReflectItemSetIt;
public:
    CReflectHelper();

    //将相关类成员的信息记录在反射信息中
    CReflectHelper(void *self, void* data, const char* type, const char* name, int sz, int num=1);

    //根据类名称获取成员变量反射信息列表
    static bool GetList(void *p, const string &key, vector<CReflectItem> &vec);

    //获取指定类成员变量的反射信息
    static CReflectItem GetItem(void *p, const string& key, const string& name);

private:
    // 全局变量Map;
    static CReflectItemMap &GetMap();
};



//定义一个返回值为void* 参数为null的函数指针
typedef void* (*CItemObject)();

struct CItemObjectClass
{
    explicit CItemObjectClass(CItemObject item) : m_ItemObject(item) {}
    CItemObject m_ItemObject;
};


//作为所有类的工厂，如有需要某一类型的类的工厂可以继承此类
class CClassFactory : public CSingleton<CClassFactory>
{
public:
    CClassFactory();
    ~CClassFactory();


    //************************************
    // Method:    CreateItem 创建类，
    // FullName:  ClassFactory::CreateItem
    // Access:    public
    // Returns:   void *
    // Qualifier:
    // Parameter: string className
    //************************************
    void * CreateItem(string className);    // 返回void *减少了代码的耦合

    //
    //************************************
    // Method:    RegisterItem
    // FullName:  ClassFactory::RegisterItem
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: const string & className 要创建类的类名
    // Parameter: ItemObject item 函数指针，该指针在宏REGISTER_CLASS中被绑定
    //************************************
    void RegisterItem(const string& className, CItemObject item);

private:
    //缓存类名和生成类实例函数指针的map，ItemObject实际上是一个函数指针
    map<string, CItemObjectClass *> m_ObjectItems;
};


#endif // CREFLECTHELPER_H
