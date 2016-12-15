/* This is generated source code. Do not edit */

#if !defined(__79463AD7_02C3_42FD_A8F0_061525C45FF8_COMP_H__)
#define __79463AD7_02C3_42FD_A8F0_061525C45FF8_COMP_H__

#include "prtypes.h"
#include <string>
using namespace std;
class AESEncrypt {
public:
    AESEncrypt();
    ~AESEncrypt();
    PRStatus Get_key(string& val) {val = m_key; return PR_SUCCESS;};
    PRStatus Set_key(string& val) {m_key = val; return PR_SUCCESS;};
    PRStatus SetKey(string& Key);
    string Encrypt(string& Plaintext);
    string Decrypt(string& Ciphertext);
private:
    string m_key;
};

#endif

