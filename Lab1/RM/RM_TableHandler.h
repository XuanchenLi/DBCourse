#ifndef RM_TABLEHANDLER_H
#define RM_TABLEHANDLER_H

#include <utility>
#include <string>
#include <iostream>
#include "RM/RM_Record.h"
#include "utils/RC.h"
#include "FM/FM_FileHandler.h"

struct RM_Rid;
class FM_FileHandler;
class RM_TblIterator;

class RM_TableHandler {
friend class RM_TblIterator;

public:
    ~RM_TableHandler();
    RM_TableHandler(){fHandler = nullptr;}
    RM_TableHandler(const char* tblPath);
    
    RM_TableHandler& operator=(const RM_TableHandler& other) {
        if (this == &other)
            return *this;
        if (fHandler != nullptr)
            delete fHandler;
        fHandler = new FM_FileHandler;
        *fHandler = *other.fHandler;
        return *this;
        //r.fHandler = nullptr;
    }
    
    RC OpenTbl(const char* tblPath);
    RC CloseTbl();
    template<typename T>
    RC Insert(T);
    RC GetRec(const RM_Rid&, RM_Record&);
    RC GetIter(RM_TblIterator& iter);

    //void test() {std::cout<<fHandler->GetFileHdr().firstFreeHole<<std::endl;}
private:
    RC InsertRec(const RM_Record&);
    RC GetNextFreeSlot(RM_Rid&);
    FM_FileHandler* fHandler;
    //int recLen;
    bool isChanged;
};

template<typename T>
RC RM_TableHandler::Insert(T obj) {
    RM_Record rec;
    rec.addr = new char[sizeof(T)];
    rec.len = sizeof(T);
    int rc = this->GetNextFreeSlot(rec.rid);
    if (rc!=SUCCESS)
        return rc;

    rec.SetContent<T>(obj);
    rc = this->InsertRec(rec);
    delete[] rec.addr;
    return rc;
}



#endif