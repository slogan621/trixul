/* This is generated source code. Do not edit */

#if !defined(__94981D9E_FC99_11D9_8BDE_F66BAD1E3F3A_COMP_H__)
#define __94981D9E_FC99_11D9_8BDE_F66BAD1E3F3A_COMP_H__

#include "prtypes.h"
#include <string>
using namespace std;
class HelloClass {
public:
    HelloClass();
    ~HelloClass();
    PRStatus Get_age(PRInt32& val) {val = m_age; return PR_SUCCESS;};
    PRStatus Set_age(PRInt32& val) {m_age = val; return PR_SUCCESS;};
    PRInt32 Hello(string& Hello);
private:
    PRInt32 m_age;
};

class HelloClass2 {
public:
    HelloClass2();
    ~HelloClass2();
    PRStatus Get_age(PRInt32& val) {val = m_age; return PR_SUCCESS;};
    PRStatus Set_age(PRInt32& val) {m_age = val; return PR_SUCCESS;};
    PRInt32 Hello(string& Hello);
private:
    PRInt32 m_age;
};

#endif

