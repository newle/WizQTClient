#ifndef WIZXMLRPC_H
#define WIZXMLRPC_H

#include "wizmisc.h"
#include "wizxml.h"


class CWizXmlRpcValue;

bool WizXmlRpcResultFromXml(CWizXMLDocument& doc, CWizXmlRpcValue** ppRet);


class CWizXmlRpcRequest
{
public:
    CWizXmlRpcRequest(const QString& strMethodName);
    void addParam(CWizXmlRpcValue* pParam);
    QByteArray toData();

protected:
    CWizXMLDocument m_doc;
};


struct CWizXmlRpcValue
{
	virtual ~CWizXmlRpcValue() {}
    virtual bool Write(CWizXMLNode& nodeParent) = 0;
    virtual bool Read(CWizXMLNode& nodeParent) = 0;
    virtual CString ToString() const = 0;

    template <class TData>
    bool ToData(TData& data, const QString& strKbGUID);

    template <class TData>
    bool ToArray(std::deque<TData>& arrayData, const QString& strKbGUID);
};


class CWizXmlRpcIntValue 
	: public CWizXmlRpcValue
{
	int m_n;
public:
    CWizXmlRpcIntValue(int n = 0);

    virtual bool Write(CWizXMLNode& nodeValue);
    virtual bool Read(CWizXMLNode& nodeValue);
    virtual CString ToString() const;

	operator int();
};


class CWizXmlRpcBoolValue
	: public CWizXmlRpcValue
{
    bool m_b;
public:
    CWizXmlRpcBoolValue(bool b = FALSE);

    virtual bool Write(CWizXMLNode& nodeValue);
    virtual bool Read(CWizXMLNode& nodeValue);
    virtual CString ToString() const;

    operator bool ();
};


class CWizXmlRpcStringValue
	: public CWizXmlRpcValue
{
	CString m_str;
public:
    CWizXmlRpcStringValue(const CString& strDef = "");

    virtual bool Write(CWizXMLNode& nodeValue);
    virtual bool Read(CWizXMLNode& nodeValue);
    virtual CString ToString() const;

	operator CString();
};


class CWizXmlRpcTimeValue
	: public CWizXmlRpcValue
{
	COleDateTime m_t;
public:
    CWizXmlRpcTimeValue(const COleDateTime& t = ::WizGetCurrentTime());

    virtual bool Write(CWizXMLNode& nodeValue);
    virtual bool Read(CWizXMLNode& nodeValue);
    virtual CString ToString() const;

	operator COleDateTime();
};


class CWizXmlRpcBase64Value
	: public CWizXmlRpcValue
{
    QByteArray m_arrayData;
public:
    CWizXmlRpcBase64Value(const QByteArray& arrayData = QByteArray());

    virtual bool Write(CWizXMLNode& nodeValue);
    virtual bool Read(CWizXMLNode& nodeValue);
    virtual CString ToString() const;

    bool GetStream(QByteArray& arrayData);
};


class CWizXmlRpcArrayValue 
	: public CWizXmlRpcValue
{
    std::deque<CWizXmlRpcValue*> m_array;
public:
	CWizXmlRpcArrayValue();
	CWizXmlRpcArrayValue(const CWizStdStringArray& arrayData);
	virtual ~CWizXmlRpcArrayValue ();
    virtual bool Write(CWizXMLNode& nodeValue);
    virtual bool Read(CWizXMLNode& nodeValue);
    virtual CString ToString() const;

    void Add(CWizXmlRpcValue* pValue);

    void Clear();

	template <class TData>
    bool ToArray(std::deque<TData>& arrayRet, const QString& strKbGUID);

    bool ToStringArray(CWizStdStringArray& arrayRet);
    bool SetStringArray(const CWizStdStringArray& arrayData);
};


class CWizXmlRpcStructValue 
	: public CWizXmlRpcValue
{
    std::map<CString, CWizXmlRpcValue*> m_map;

    void AddValue(const CString& strzName, CWizXmlRpcValue* pValue);
    CWizXmlRpcValue* GetValue(const CString& strName) const;

	template<class T>
    T* GetValue(const CString& strName) const { return dynamic_cast<T*>(GetValue(strName)); }

public:
	virtual ~CWizXmlRpcStructValue();

    virtual bool Write(CWizXMLNode& nodeValue);
    virtual bool Read(CWizXMLNode& nodeValue);
	virtual CString ToString() const;

    void AddInt(const CString& strName, int n);
    void AddString(const CString& strName, const CString& str);
    void AddStruct(const CString& strName, CWizXmlRpcStructValue* pStruct);
    void AddArray(const CString& strName, CWizXmlRpcValue* pArray);
    void AddBool(const CString& strName, bool b);
    void AddTime(const CString& strName, const COleDateTime& t);
    void AddBase64(const CString& strName, const QByteArray& arrayData);
    void AddInt64(const CString& strName, __int64 n);
    void AddColor(const CString& strName, COLORREF cr);
    bool AddFile(const CString& strName, const CString& strFileName);

	void Clear();
    void RemoveValue(const CString& strName);
	void DeleteValue(CWizXmlRpcValue* pValue);

    bool GetBool(const CString& strName, bool& b) const;
    bool GetInt(const CString& strName, int& n) const;
    bool GetString(const QString& strName, QString& str) const;
    bool GetTime(const CString& strName, COleDateTime& t) const;
    bool GetStream(const CString& strName, QByteArray& arrayData) const;
    CWizXmlRpcStructValue* GetStruct(const CString& strName) const;
    CWizXmlRpcArrayValue* GetArray(const CString& strName) const;

    bool GetStr(const QString& strName, QString& str) const { return GetString(strName, str); }
    bool GetInt64(const CString& strName, __int64& n) const;
    bool GetInt(const CString& strName, long& n) const;
    bool GetColor(const CString& strName, COLORREF& cr) const;

    bool ToStringMap(std::map<CString, CString>& ret) const;

	template <class TData>
    bool GetArray(const CString& strName, std::deque<TData>& arrayData, const QString& kbGUID);

    template <class TData>
    bool AddArray(const CString& strName, const std::deque<TData>& arrayData);

    void AddStringArray(const CString& strName, const CWizStdStringArray& arrayData);
    bool GetStringArray(const CString& strName, CWizStdStringArray& arrayData) const;
};

class CWizXmlRpcFaultValue
    : public CWizXmlRpcValue
{
    CWizXmlRpcStructValue m_val;
public:
    ~CWizXmlRpcFaultValue();

    virtual bool Write(CWizXMLNode& nodeValue) { Q_UNUSED(nodeValue); ATLASSERT(FALSE); return FALSE; }
    virtual bool Read(CWizXMLNode& nodeValue);
    virtual CString ToString() const { return WizFormatString2(_T("Fault error: %1, %2"), WizIntToStr(GetFaultCode()), GetFaultString()); }

    int GetFaultCode() const;
    CString GetFaultString() const;
};

enum WizXmlRpcError
{
    errorNetwork,
    errorContentType,
    errorXmlFormat,
    errorXmlRpcFormat,
    errorXmlRpcFault
};


template <class TData>
inline bool CWizXmlRpcValue::ToData(TData& data, const QString& strKbGUID)
{
    if (CWizXmlRpcStructValue* pStruct = dynamic_cast<CWizXmlRpcStructValue*>(this))
    {
        return data.LoadFromXmlRpc(*pStruct, strKbGUID);
    }

    return false;
}

template <class TData>
inline bool CWizXmlRpcValue::ToArray(std::deque<TData>& arrayData,
                                     const QString& strKbGUID)
{
    if (CWizXmlRpcArrayValue* pArray = dynamic_cast<CWizXmlRpcArrayValue*>(this))
    {
        return pArray->ToArray<TData>(arrayData, strKbGUID);
    }

    return false;
}

template <class TData>
inline bool CWizXmlRpcArrayValue::ToArray(std::deque<TData>& arrayRet,
                                          const QString& strKbGUID)
{
    std::deque<CWizXmlRpcValue*>::const_iterator it;
    for (it = m_array.begin(); it != m_array.end(); it++) {
        CWizXmlRpcValue* pValue = *it;
        if (!pValue)
        {
            TOLOG(_T("Fault error: element of array is null"));
            return false;
        }

        TData data;
        if (!pValue->ToData<TData>(data, strKbGUID))
        {
            TOLOG(_T("Failed load data form value"));
            return false;
        }

        arrayRet.push_back(data);
    }

    return true;
}

template <class TData>
inline bool CWizXmlRpcStructValue::GetArray(const CString& strName,
                                            std::deque<TData>& arrayData,
                                            const QString& kbGUID)
{
    CWizXmlRpcArrayValue* pArray = GetArray(strName);
    if (!pArray)
    {
        TOLOG(_T("Failed to get array data in struct"));
        return false;
    }

    return pArray->ToArray<TData>(arrayData, kbGUID);
}

template <class TData>
inline bool CWizXmlRpcStructValue::AddArray(const CString& strName, const std::deque<TData>& arrayData)
{
    CWizXmlRpcArrayValue* pArray = new CWizXmlRpcArrayValue();
    AddArray(strName, pArray);

    for (typename std::deque<TData>::const_iterator it = arrayData.begin();
        it != arrayData.end();
        it++)
    {
        CWizXmlRpcStructValue* pStruct = new CWizXmlRpcStructValue();
        it->SaveToXmlRpc(*pStruct);

        pArray->Add(pStruct);
    }

    return true;
}


#endif //WIZXMLRPC_H
