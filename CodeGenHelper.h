//
// 
//

#ifndef SCENESERVER_CODEGENHELPER_H
#define SCENESERVER_CODEGENHELPER_H

#include <string>

class CodeGenHelper
{
public:
    virtual ~CodeGenHelper(){}

    std::string GenCodeID(uint64_t id);
    std::string Gen(uint64_t id, int32_t length);
    uint64_t DecodeID(std::string code);
    int32_t FindIndex(char c);
};


#endif //SCENESERVER_CODEGENHELPER_H
