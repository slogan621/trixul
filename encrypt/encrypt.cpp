#include "encrypt.h"

#include <gcrypt.h>

AESEncrypt::AESEncrypt()
{
}

AESEncrypt::~AESEncrypt()
{
}

PRStatus 
AESEncrypt::SetKey(string& Key)
{
    m_key = Key;
    return PR_SUCCESS;
}

string AESEncrypt::Encrypt(string& Plaintext)
{
    gcry_cipher_hd_t hd = 0;
    gcry_error_t err = 0;
    char *iv = "1234567812345678";
    char ctext[128] = "";
    char actext[256] = "";

    err = gcry_cipher_open(&hd, GCRY_CIPHER_AES, GCRY_CIPHER_MODE_CFB, 0);
    if (!err) {
        err = gcry_cipher_setkey(hd, m_key.c_str(), m_key.length());
    }
    if (!err) {
        err = gcry_cipher_setiv(hd, iv, strlen(iv));
    }
    if(!err) {
        err = gcry_cipher_encrypt(hd, ctext, sizeof(ctext), Plaintext.c_str(), 
            Plaintext.length());
    }
    if (!err) {
        int i;
        char ascbuf[3];

        for (i=0; i < strlen(ctext); i++) {
            snprintf(ascbuf, 2, "%02x", ctext[i]);
            strcat(actext, ascbuf);
        }
    }
    if (hd) 
        gcry_cipher_close(hd);
    return actext;
}

string AESEncrypt::Decrypt(string& Ciphertext)
{
    gcry_cipher_hd_t hd = 0;
    gcry_error_t err = 0;
    char *iv = "1234567812345678";
    char ptext[128] = "";

    err = gcry_cipher_open(&hd, GCRY_CIPHER_AES, GCRY_CIPHER_MODE_CFB, 0);
    if (!err)
        err = gcry_cipher_setkey(hd, m_key.c_str(), m_key.length());
    if (!err) 
        err = gcry_cipher_setiv(hd, iv, strlen(iv));
    if(!err)     
        err = gcry_cipher_decrypt(hd, ptext, sizeof(ptext), Ciphertext.c_str(),
            Ciphertext.length());
    if (hd) 
        gcry_cipher_close(hd);
    return ptext;
}

