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

    // �жϻ����Ƿ���CrfObject����;
    static bool IsCrfObj(void *p);

    // �ṹ���ַ���תΪCrfObject����;
    static bool StructToCrfObject(const string &str);

    // ��ȡ������;
    const char * GetBaseClassName(){return m_BaseName;}
    //����������ʱ��ȡ�������ʵ��������
    virtual string GetClassName() const;

    // ��ȡԪ���б�;
    bool GetList(vector<CReflectItem> &vec);
    // ��ȡָ����Ԫ��;
    CReflectItem GetItem(const string& name);

    // �������л��ڴ��С;
    int GetSerializationSz();
    // ���л�;
    bool Serialization(unsigned char *buf, int sz);
    // �����л�;
    bool Deserialization(unsigned char *buf, int sz);
    // ��ӡ�ṹ��;
    bool PrintStruct();
    // CrfObject����ת��Ϊstruct��������;
    bool ToStructData(int sz, void *data);
    bool AssignFromStructData(int sz, void *data);

private:
    char m_BaseName[16];
};



/*
* �������Բ�����
* set���������޸�����
* get�������ڻ�ȡ����
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
    //��ȡ��Ա��ƫ�Ƶ�ַ;
    int GetOffset() const {return m_Offset;}
    //��ȡ��Ա����;
    string GetName() const { return m_ItemType ? m_ItemName : ""; }
    //��ȡ��Ա����;
    string GetType() const { return m_ItemType ? m_ItemType : ""; }
    // ��ȡ��ǰԪ���������ݽṹ����ʼ��ַ;
    const void *GetStartAddr() const { return m_Addrp; }
    // ��ȡ��ǰ�����ݴ�С;
    int GetSize() const {return m_ItemSz;}
    // ��ȡ��ǰ�������;
    int GetArrNum() const {return m_ArrNum;}
    // ��ȡ��ǰԪ�������ڴ��ַ;
    void* GetValAddr()const {return ((char*)(m_Addrp) + m_Offset);}


    //��ȡ��Ա��ֵ;
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

    //�޸ĳ�Ա��ֵ
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
* �������Թ�����
* ��map�洢������Ա���������͡�ƫ�Ƶ�ַ����Ϣ
*/
class CReflectHelper
{
    typedef map<string, set<CReflectItem> > CReflectItemMap;
    typedef CReflectItemMap::iterator CReflectItemMapIt;
    typedef set<CReflectItem> CReflectItemSet;
    typedef CReflectItemSet::iterator CReflectItemSetIt;
public:
    CReflectHelper();

    //��������Ա����Ϣ��¼�ڷ�����Ϣ��
    CReflectHelper(void *self, void* data, const char* type, const char* name, int sz, int num=1);

    //���������ƻ�ȡ��Ա����������Ϣ�б�
    static bool GetList(void *p, const string &key, vector<CReflectItem> &vec);

    //��ȡָ�����Ա�����ķ�����Ϣ
    static CReflectItem GetItem(void *p, const string& key, const string& name);

private:
    // ȫ�ֱ���Map;
    static CReflectItemMap &GetMap();
};



//����һ������ֵΪvoid* ����Ϊnull�ĺ���ָ��
typedef void* (*CItemObject)();

struct CItemObjectClass
{
    explicit CItemObjectClass(CItemObject item) : m_ItemObject(item) {}
    CItemObject m_ItemObject;
};


//��Ϊ������Ĺ�����������Ҫĳһ���͵���Ĺ������Լ̳д���
class CClassFactory : public CSingleton<CClassFactory>
{
public:
    CClassFactory();
    ~CClassFactory();


    //************************************
    // Method:    CreateItem �����࣬
    // FullName:  ClassFactory::CreateItem
    // Access:    public
    // Returns:   void *
    // Qualifier:
    // Parameter: string className
    //************************************
    void * CreateItem(string className);    // ����void *�����˴�������

    //
    //************************************
    // Method:    RegisterItem
    // FullName:  ClassFactory::RegisterItem
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: const string & className Ҫ�����������
    // Parameter: ItemObject item ����ָ�룬��ָ���ں�REGISTER_CLASS�б���
    //************************************
    void RegisterItem(const string& className, CItemObject item);

private:
    //����������������ʵ������ָ���map��ItemObjectʵ������һ������ָ��
    map<string, CItemObjectClass *> m_ObjectItems;
};


#endif // CREFLECTHELPER_H
