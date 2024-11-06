//
// 参考来源 https://gist.github.com/zjnxyz/6ef6007f493a0edea6837cc06e934abc
//

#include "CodeGenHelper.h"

// 随机字符串
static std::vector<char> CHARS = { 
    'F', 'L', 'G', 'W', 
    '5', 'X', 'C', '3', 
    '9', 'Z', 'M', '6', 
    '7', 'Y', 'R', 'T', 
    '2', 'H', 'S', '8', 
    'D', 'V', 'E', 'J', 
    '4', 'K', 'Q', 'P', 
    'U', 'A', 'N', 'B' 
};

// 随机字符串长度
static const int32_t CHARS_LENGTH = 32;
// 邀请码长度
static const int32_t CODE_LENGTH = 8;
// 加密盐
static const int32_t SLAT = 2618251L;
// PRIME1 与 CHARS 的长度 L互质，可保证 ( id * PRIME1) % L 在 [0,L)上均匀分布
static const int32_t PRIME1 = 3;
// PRIME2 与 CODE_LENGTH 互质，可保证 ( index * PRIME2) % CODE_LENGTH  在 [0，CODE_LENGTH）上均匀分布
static const int32_t PRIME2 = 11;

// 生成邀请码
std::string CodeGenHelper::GenCodeID(uint64_t id)
{
    return Gen(id, CODE_LENGTH);
}

/*
* 注意当前算法支持的ID范围为 [0,(32^7 - 1234561)/3]
* 超过这个范围会出现重复邀请码，还原id也会失败
* 如需扩大范围可适当增加邀请码长度
*/
std::string CodeGenHelper::Gen(uint64_t id, int32_t length)
{
    // 补位
    id = id * PRIME1 + SLAT;
    //将 id 转换成32进制的值
    std::vector<uint64_t> bin_value(CODE_LENGTH, 0);
    bin_value[0] = id;
    
    for (int32_t i = 0; i < CODE_LENGTH - 1; i++) 
    {
        bin_value[i + 1] = bin_value[i] / CHARS_LENGTH;
        // 按位扩散
        bin_value[i] = (bin_value[i] + i * bin_value[0]) % CHARS_LENGTH;
    }
    
    uint64_t tmp = 0;
    for (int32_t i = 0; i < length - 2; i++) 
    {
        tmp += bin_value[i];
    }
    
    bin_value[length - 1] = tmp * PRIME1 % CHARS_LENGTH;
    
    // 打乱顺序
    std::vector<uint64_t> code_upset(CODE_LENGTH, 0);
    for (int32_t i = 0; i < CODE_LENGTH; i++) 
    {
        code_upset[i] = bin_value[i * PRIME2 % CODE_LENGTH];
    }
    
    std::string ret;
    for (int32_t i = 0; i < CODE_LENGTH; i++) 
    {
        ret += CHARS[code_upset[i]];
    }
    
    return ret;
}

// 还原ID
uint64_t CodeGenHelper::DecodeID(std::string code)
{
    if (code.length() != CODE_LENGTH) 
    {
        return 0;
    }
    
    // 将字符还原成对应数字
    std::vector<uint64_t> pos(CODE_LENGTH, 0);
    for (int32_t i = 0; i < CODE_LENGTH; i++) 
    {
        char c = code[i];
        int index = FindIndex(c);
        if (index == -1) 
        {
            return 0;
        }
        pos[i * PRIME2 % CODE_LENGTH] = index;
    }
    
    std::vector<uint64_t> value(CODE_LENGTH, 0);
    for (int32_t i = CODE_LENGTH - 2; i >= 0; i--) 
    {
        value[i] = (pos[i] - pos[0] * i + CHARS_LENGTH * i) % CHARS_LENGTH;
    }
    
    uint64_t res = 0;
    for (int32_t i = CODE_LENGTH - 2; i >= 0; i--) 
    {
        res += value[i];
        res *= (i > 0 ? CHARS_LENGTH : 1);
    }
    
    return (res - SLAT) / PRIME1;
}

// 查找对应字符的index
int32_t CodeGenHelper::FindIndex(char c)
{
    for (int32_t i = 0; i < CHARS_LENGTH; i++) 
    {
        if (CHARS[i] == c)
        {
            return i;
        }
    }
    return -1;
}
