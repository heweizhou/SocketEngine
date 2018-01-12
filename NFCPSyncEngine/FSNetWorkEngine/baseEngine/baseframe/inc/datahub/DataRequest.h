//
//  DataRequest.h
//  commons
//
//  Created by 周贺伟 on 15/5/29.
//  Copyright (c) 2015年 周贺伟. All rights reserved.
//

#ifndef commons_DataRequest_h
#define commons_DataRequest_h


#include "RefBase.h"
#include "DataRequestProtocol.h"

class IHubRequestBack;

typedef struct _tag_param_data
{
    _tag_param_data()
    {
        buffer = _NULL;
        bufferSize = 0;
    }
    _tag_param_data(_PVOID data, _ULONG dataSize){
        buffer = malloc(dataSize);
        bufferSize = dataSize;
        memcpy(buffer, data, dataSize);
    }
    
    ~_tag_param_data()
    {
        SAFE_FREE(buffer);
    };
    
    _VOID setData(_PVOID data, _ULONG dataSize)
    {
        SAFE_COPY(buffer, bufferSize, data, dataSize);
        bufferSize = dataSize;
    }

    _PVOID buffer;
    _ULONG bufferSize;
}ParamData,*PParamData;

class IHubRequestBack:public _MY_NAME_SPACE_::RefBase
{
public:
    IHubRequestBack( _DataHubMessageKind kind );
protected:
    virtual ~IHubRequestBack();
public:
    virtual _VOID OnDataBack( _HANDLE memHandler, _PVOID reserve, PParamData param ) = 0;
    virtual _VOID OnEvent(_UINT code, _PVOID reserve, PParamData param) = 0;
private:
    _DataHubMessageKind m_kind;
};

#endif
