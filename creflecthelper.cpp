#include <set>
#include <string>
#include <utility>

#include <stdio.h>


#include "creflecthelper.h"

struct StructFiledInfo_t{
    string type;
    string name;
    int arrNum;
};


string GetNonCompositeTypeStr(const char *str)
{
    const char *p = NULL;
    char type[100] = {0};
    if(str == NULL) return "";

    if((p = IsArray(str)) || (p = IsPointer(str))){
        strncpy(type, str, p-str);
        CStrip(type);
//    }else if((p = IsPointer(str))){
//        strncpy(type, str, p-str);
//        CStrip(type);
    }else{
        strcpy(type, str);
    }
    return string(type);
}



int GetStructField(char *buf, StructFiledInfo_t &info)
{
    char tmp[64];
    char *pBuf = buf;
    char *p = NULL;
    string type, name;
    int arrNum = 1;

//    printf("\n");

    sscanf(pBuf, "%s", tmp);
    type = tmp;
    pBuf += strlen(tmp);
    if(strcmp(tmp, "unsigned") == 0){
        type += " ";
        sscanf(pBuf, "%s", tmp); pBuf += 1;
        type += tmp;
        pBuf += strlen(tmp);
    }
//printf("1> type=%s, pBuf=%s\n", type.c_str(), pBuf);

    sscanf(pBuf, "%s", tmp); pBuf += 1;
    p = strchr(tmp, ';');
    if(p) *p = 0;
    pBuf += strlen(tmp);
//printf("2> tmp=%s, pBuf=%s\n", tmp, pBuf);

    p = strchr(tmp, '*');
    if(p){
        type += '*';
        int len = strlen(p);
        if(len == 1){
            sscanf(pBuf, "%s", tmp); pBuf += 1;
            p = strchr(tmp, ';');
            if(p) *p = 0;
        }else{
            p += 1;
            sscanf(p, "%s", tmp);
        }
//        printf("3> p=%s, tmp=%s\n", p, tmp);
    }

    p = strchr(tmp, '[');
    if(p){
        arrNum = atoi(p+1);
        *p = 0;
    }

    name = tmp;
    pBuf = strchr(pBuf, ';');
    if(!pBuf){LogPos(1); return -1;}

    info.type = type;
    info.name = name;
    info.arrNum = arrNum;

    return pBuf - buf;
}

void PrintHelper(string cName, vector<StructFiledInfo_t> &info)
{
    printf("class %sHelper : public CrfObject \n", cName.c_str());
    printf("{\n");
    printf("public:\n");

    for(int i=0; i<info.size(); i++){
        StructFiledInfo_t &filed = info[i];
        if(filed.arrNum > 1){
            printf("    ROBJADDFIELDARR(%s, %s, %d);\n", filed.type.c_str(), filed.name.c_str(), filed.arrNum);
        }else{
            printf("    ROBJADDFIELD(%s, %s);\n", filed.type.c_str(), filed.name.c_str());
        }
    }
    printf("};\n");
}


bool CrfObject::IsCrfObj(void *p)
{
    CrfObject *cp = (CrfObject *)p;
    return strcmp(cp->m_BaseName, ToStr(CrfObject)) == 0;
}

bool CrfObject::StructToCrfObject(const string &str)
{
    const char *cStr = str.c_str();
    const char *cEnd = cStr + str.length();;
    char *pStr = NULL;
    char name[64] = {0};
    string cName;

    vector<StructFiledInfo_t > lists;

//printf("%s\n", str.c_str());

    if(str.empty()){LogPos(1); return false;}

    pStr = strstr(cStr, "struct");
    if(pStr == NULL){LogPos(1); return false;}
    pStr += 6;

    while(pStr < cEnd){
        if(*pStr == ' '){ pStr++; continue;}
        if(*pStr == '{') break;
        sscanf(pStr, "%s", name);
        char *p = strchr(name, '{');
        if(p) *p = 0;
        cName = name;
        pStr += strlen(name);
        break;
    }
//printf("name=%s\n", cName.c_str());
    pStr = strchr(pStr, '{');
    if(!pStr){LogPos(1); return false;}

    pStr += 1;
    while (pStr < cEnd) {
        StructFiledInfo_t info;
        int len = 0;
        if(*pStr == ' '){pStr++; continue;}
        if(*pStr == '}') break;
        len = GetStructField(pStr, info);
        if(len <= 0){LogPos(1); break;}
        lists.push_back(info);
//printf("type=%s, name=%s, arrNum=%d\n", info.type.c_str(), info.name.c_str(), info.arrNum);

        pStr += len;
        pStr = strchr(pStr, ';');
        if(!pStr){LogPos(1); break;}
        pStr++;
    }

    pStr = strchr(pStr, '}');
    if(!pStr){LogPos(1); return false;}

    PrintHelper(cName, lists);

    return true;
}

string CrfObject::GetClassName() const
{
#ifdef _MSC_VER
        return typeid(*this).name() + 6;
#else
        const char* name = typeid(*this).name();

        while (*name >= '0' && *name <= '9') name++;

        return name;
#endif
}

bool CrfObject::GetList(vector<CReflectItem> &vec)
{
    return CReflectHelper::GetList(this, this->GetClassName(), vec);
}

CReflectItem CrfObject::GetItem(const string &name)
{
    return CReflectHelper::GetItem(this, this->GetClassName(), name);
}

int CrfObject::GetSerializationSz()
{
    bool ret;
    int sz = 0;
    vector<CReflectItem> vec;

    ret = GetList(vec);
    if(!ret){LogPos(1); return -1;}

    for(int i=0; i<vec.size(); i++){
        CReflectItem &item = vec[i];
        string type = item.GetType();
        type = GetNonCompositeTypeStr(type.c_str());

        if(CmpBaseType(type.c_str())){
            sz += item.GetSize();
        }else{
            CrfObject *obj = (CrfObject*)item.GetValAddr();
            if(IsCrfObj(obj)){
                int objSz = obj->GetSerializationSz();
                if(objSz > 0){
                    sz += objSz;
                    continue;
                }
            }
            LOGFILE("Error: %s %s is not a serializable type!\n", item.GetType().c_str(), item.GetName().c_str());
            return -1;
        }
    }

    return sz;
}

bool CrfObject::Serialization(unsigned char *buf, int sz)
{
    bool ret = false;;
    vector<CReflectItem> vec;
    unsigned char *pBuf = buf;

    if(!buf || sz <= 0){LogPos(1); return false;}
    if(sz < GetSerializationSz()){LogPos(1); return false;}

    ret = GetList(vec);
    if(!ret){LogPos(1); return false;}

    for(int i=0; i<vec.size(); i++){
        int itemSz = 0;
        CReflectItem &item = vec[i];
        string type = item.GetType();
        type = GetNonCompositeTypeStr(type.c_str());
        LogFile("Serial: i=%d, %s(%s) %s size=%d p=%p\n",
               i, item.GetType().c_str(), type.c_str(), item.GetName().c_str(), item.GetSize(), item.GetStartAddr());
        if(IsPointer(item.GetType().c_str())){LogFile("Warning Serial type %s %s\n", item.GetType().c_str(), item.GetName().c_str());}
        if(CmpBaseType(type.c_str())){
            itemSz = item.GetSize();
            memcpy(pBuf, item.GetValAddr(), itemSz);
            pBuf += itemSz;
        }else{
            CrfObject *obj = (CrfObject*)item.GetValAddr();
            itemSz = obj->GetSerializationSz();
            ret = obj->Serialization(pBuf, itemSz);
            if(!ret){LogPos(1); return false;}
            pBuf += itemSz;
        }
    }

    return true;
}

bool CrfObject::Deserialization(unsigned char *buf, int sz)
{
    bool ret = false;;
    vector<CReflectItem> vec;
    unsigned char *pBuf = buf;

    if(!buf || sz <= 0){LogPos(1); return false;}
    if(sz < GetSerializationSz()){LogPos(1); return false;}

    ret = GetList(vec);
    if(!ret){LogPos(1); return false;}

    for(int i=0; i<vec.size(); i++){
        int itemSz = 0;
        CReflectItem &item = vec[i];
        string type = item.GetType();
        type = GetNonCompositeTypeStr(type.c_str());
        LogFile("Deser: i=%d, %s(%s) %s size=%d\n", i, item.GetType().c_str(), type.c_str(), item.GetName().c_str(), item.GetSize());

        if(CmpBaseType(type.c_str())){
            itemSz = item.GetSize();
            memcpy(item.GetValAddr(), pBuf, itemSz);
            pBuf += itemSz;
        }else{
            CrfObject *obj = (CrfObject*)item.GetValAddr();
            itemSz = obj->GetSerializationSz();
            ret = obj->Deserialization(pBuf, itemSz);
            if(!ret){LogPos(1); return false;}
            pBuf += itemSz;
        }
    }

    return true;
}

bool CrfObject::PrintStruct()
{
    bool ret;
    vector<CReflectItem> vec;

    ret = GetList(vec);
    if(!ret){LogPos(1); return false;}

    printf("struct %s_t{\n", GetClassName().c_str());

    for(int i=0; i<vec.size(); i++){
        CReflectItem &item = vec[i];
        string type = item.GetType();
        string name = item.GetName();
        int arrNum = item.GetArrNum();
        if(arrNum > 1)
            printf("    %s %s[%d];\n", type.c_str(), name.c_str(), arrNum);
        else
            printf("    %s %s;\n", type.c_str(), name.c_str());
    }

    printf("};\n");

    return true;
}

bool CrfObject::ToStructData(int sz, void *data)
{
    bool ret = false;
    char *d = (char *)data;
    int len = 0;

    if(!data){LogPos(1); return false;}
    if(sz <= 0){LogPos(1); return false;}

    vector<CReflectItem> vec;
    ret = GetList(vec);
    if(!ret){LogPos(1); return false;}

    for(int i=0; i<vec.size(); i++){
        CReflectItem &item = vec[i];
        int arrNum = item.GetArrNum();
        int dSz = item.GetSize();
        int typeSz = dSz / arrNum;

        if((len % typeSz) != 0 ) len += typeSz - (len % typeSz);
        if(len >= sz){LogPos(1); return false;}
        ret = item.GetVal((void *)(d + len), arrNum);
        len += dSz;
    }

    return true;
}

bool CrfObject::AssignFromStructData(int sz, void *data)
{
    bool ret = false;
    char *d = (char *)data;
    int len = 0;

    if(!data){LogPos(1); return false;}
    if(sz <= 0){LogPos(1); return false;}

    vector<CReflectItem> vec;
    ret = GetList(vec);
    if(!ret){LogPos(1); return false;}

    for(int i=0; i<vec.size(); i++){
        CReflectItem &item = vec[i];
        int arrNum = item.GetArrNum();
        int dSz = item.GetSize();
        int typeSz = dSz / arrNum;

        if((len % typeSz) != 0 ) len += typeSz - (len % typeSz);
        if(len >= sz){LogPos(1); return false;}
        ret = item.SetVal((void *)(d + len), arrNum);
        len += dSz;
    }

    return true;
}

CReflectItem::CReflectItem()
{
     m_Offset = 0;
     m_ItemType = NULL;
     m_Addrp = NULL;
     m_ArrNum = 0;
     m_ItemSz = 0;
}

CReflectItem::CReflectItem(int _offset, const char *_type, const string &_name, int tSz, int arrLen)
    : m_Offset(_offset), m_ItemType(_type), m_ItemName(_name), m_ItemSz(tSz*arrLen), m_ArrNum(arrLen)
{
    m_Addrp = (NULL);
}

CReflectItem &CReflectItem::operator =(const CReflectItem &item)
{
    m_Offset = item.m_Offset;
    m_ItemType = item.m_ItemType;
//    m_Addrp = item.m_Addrp;
    m_ArrNum = item.m_ArrNum;
    m_ItemSz = item.m_ItemSz;

    return *this;
}

string CReflectItem::GetVal() const
{
    string ret = "";
    void* dest = GetValAddr();
    if(m_Addrp == NULL) return "";

    string type = GetNonCompositeTypeStr(m_ItemType);
    for(int i=0; i<m_ArrNum; i++){
        if(i > 0) ret += ", ";
        if(CmpStr(type.c_str(), CBOOL))     ret += TBOOLP(dest)[i] ? "true" : "false";
        if(CmpStr(type.c_str(), CCHAR))     ret += (TCHARP(dest)[i]);
        if(CmpStr(type.c_str(), CSHORT))    ret += to_string(TSHORTP(dest)[i]);
        if(CmpStr(type.c_str(), CINT))      ret += to_string(TINTP(dest)[i]);
        if(CmpStr(type.c_str(), CFLOAT))    ret += to_string(TFLOATP(dest)[i]);
        if(CmpStr(type.c_str(), CDOUBLE))   ret += to_string(TDOUBLEP(dest)[i]);
        if(CmpStr(type.c_str(), CSTRING))   ret += ((string*)(dest))[i];
    }

    return ret;
}

bool CReflectItem::GetVal(void *dst, int num)
{
    void* src = GetValAddr();
    void *d = dst;

    if(num > m_ArrNum){LogPos(1); return false;}
    if(m_Addrp == NULL){LogPos(1); return false;}

#if 0
    string ctype = GetNonCompositeTypeStr(m_ItemType);
    const char *type = ctype.c_str();
    printf("type=%s\n", type);

    if(CmpStr(type, CBOOL)){
        for(int i=0; i<num; i++) TBOOLP(d)[i] = TBOOLP(src)[i];
    }
    else if(CmpStr(type, CCHAR)){
        for(int i=0; i<num; i++) TCHARP(d)[i] = TCHARP(src)[i];
    }
    else if(CmpStr(type, CUCHAR)){
        for(int i=0; i<num; i++) TUCHARP(d)[i] = TUCHARP(src)[i];
    }
    else if(CmpStr(type, CSHORT)){
        for(int i=0; i<num; i++) TSHORTP(d)[i] = TSHORTP(src)[i];
    }
    else if(CmpStr(type, CUSHORT)){
        for(int i=0; i<num; i++) TUSHORTP(d)[i] = TUSHORTP(src)[i];
    }
    else if(CmpStr(type, CINT)){
        for(int i=0; i<num; i++) TINTP(d)[i] = TINTP(src)[i];
    }
    else if(CmpStr(type, CUINT)){
        for(int i=0; i<num; i++) TUINTP(d)[i] = TUINTP(src)[i];
    }
    else if(CmpStr(type, CLLINT)){
        for(int i=0; i<num; i++) TLLINTP(d)[i] = TLLINTP(src)[i];
    }
    else if(CmpStr(type, CULLINT)){
        for(int i=0; i<num; i++) TULLINTP(d)[i] = TULLINTP(src)[i];
    }
    else if(CmpStr(type, CFLOAT)){
        for(int i=0; i<num; i++) TFLOATP(d)[i] = TFLOATP(src)[i];
    }
    else if(CmpStr(type, CDOUBLE)){
        for(int i=0; i<num; i++) TDOUBLEP(d)[i] = TDOUBLEP(src)[i];
    }
    else{
        LogPos(1); return false;
    }
#endif

    int sz = m_ItemSz / m_ArrNum * num;
    memcpy(d, src, sz);

    return true;
}

bool CReflectItem::SetVal(void *d, int num)
{
    void* dst = GetValAddr();

    if(num > m_ArrNum){LogPos(1); return false;}
    if(m_Addrp == NULL){LogPos(1); return false;}

    int sz = m_ItemSz / m_ArrNum * num;
    memcpy(dst, d, sz);

    return true;
}


CReflectHelper::CReflectHelper()
{

}

CReflectHelper::CReflectHelper(void *self, void *data, const char *type, const char *name, int sz, int num)
{
    CReflectItemMap &refMap = GetMap();
    string key = ((CrfObject*)self)->GetClassName();
    refMap[key].insert(CReflectItem((char*)(data)-(char*)(self), type, name, sz, num));

#if 0
    if(refMap.size() > 0){
        CReflectItemSetIt it = refMap[key].end();
        it --;
        printf("%s %s, offset=%d, sz=%d, num=%d\n",
               it->GetType().c_str(), it->GetName().c_str(), it->GetOffset(), sz, num);
    }
#endif
}

bool CReflectHelper::GetList(void *p, const string &key, vector<CReflectItem> &vec)
{
    CReflectItemMap &refMap = GetMap();
//printf("GetList: p=%p attrmap.sz=%d, key=%s\n", p, refMap.size(), key.c_str());

    auto it = refMap.find(key);
    if(it == refMap.end()) return false;

    int idx = 0;
    for(auto& item : it->second){
        vec.push_back(item);
        vec[idx++].SetStartAddr(p);
    }

    return vec.size() > 0;
}

CReflectItem CReflectHelper::GetItem(void *p, const string &key, const string &name)
{
    CReflectItemMap &refMap = GetMap();

    CReflectItem item(0, NULL, name, 0);

    auto it = refMap.find(key);
    if(it == refMap.end()) return item;

    auto tmp = it->second.find(item);
    if(tmp == it->second.end()) return item;

    item = *tmp;
    item.SetStartAddr(p);
    return item;
}

CReflectHelper::CReflectItemMap &CReflectHelper::GetMap()
{
    static CReflectItemMap map;
    return map;
}



CClassFactory::CClassFactory()
{
}


CClassFactory::~CClassFactory()
{
    for (auto it : m_ObjectItems)
    {
        if (it.second != nullptr)
        {
            delete it.second;
            it.second = nullptr;
        }
    }
    m_ObjectItems.clear();
}


//返回void *减少了代码的耦合
void * CClassFactory::CreateItem(string className)
{
    CItemObject constructor = nullptr;

    if (m_ObjectItems.find(className) != m_ObjectItems.end())
        constructor = m_ObjectItems.find(className)->second->m_ItemObject;

    if (constructor == nullptr)
        return nullptr;

    // 调用函数指针指向的函数 调用REGISTER_CLASS中宏的绑定函数，也就是运行new className代码
    return (*constructor)();
}

//ItemObject相当于一个回掉函数
void CClassFactory::RegisterItem(const string& className, CItemObject item)
{
    map<string, CItemObjectClass *>::iterator it = m_ObjectItems.find(className);
    if (it != m_ObjectItems.end())
        m_ObjectItems[className]->m_ItemObject = item;
    else
        m_ObjectItems.insert(make_pair(className, new CItemObjectClass(item)));
}
