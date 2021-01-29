//
// Created by 10069 on 2020/12/24.
//

#ifndef WANNAFLY_ENCRPTYFILE_H
#define WANNAFLY_ENCRPTYFILE_HW


//#include <fstream>

#include "encrptyStd.h"


using namespace std;
class encrptyFile {
private:
    char* prikey, * pubkey;
    std::string RsaPriDecrypt(const std::string& cipher_text, const std::string& pri_key,int size)
    {
        std::string decrypt_text;
        RSA* rsa = RSA_new();
        BIO* keybio;
        keybio = BIO_new_mem_buf((unsigned char*)pri_key.c_str(), -1);

        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
        if (rsa == nullptr) {
            unsigned long err = ERR_get_error(); //��ȡ�����
            char err_msg[1024] = { 0 };
            ERR_error_string(err, err_msg); // ��ʽ��error:errId:��:����:ԭ��
            printf("err msg: err:%ld, msg:%s\n", err, err_msg);
            return std::string();
        }

        // ��ȡRSA���δ������󳤶�
        int key_len = RSA_size(rsa);
        char* sub_text = new char[key_len + 1];
        memset(sub_text, 0, key_len + 1);
        int ret = 0;
        std::string sub_str;
        int pos = 0;
        // �����Ľ��зֶν���
        while (pos < size) {
            sub_str = cipher_text.substr(pos, key_len);
            memset(sub_text, 0, key_len + 1);
            ret = RSA_private_decrypt(sub_str.length(), (const unsigned char*)sub_str.c_str(), (unsigned char*)sub_text, rsa, RSA_PKCS1_PADDING);
            if (ret >= 0) {
                decrypt_text.append(std::string(sub_text, ret));
                pos += key_len;
            }
        }
        // �ͷ��ڴ�  
        delete[] sub_text;
        BIO_free_all(keybio);
        RSA_free(rsa);

        return decrypt_text;
    }

    std::string RsaPubEncrypt(const std::string& clear_text, const std::string& pub_key,int size)
    {

        std::string encrypt_text;
        BIO* keybio = BIO_new_mem_buf((unsigned char*)pub_key.c_str(), -1);
        RSA* rsa = RSA_new();

        rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);


        // ��ȡRSA���ο��Դ�������ݿ����󳤶�
        int key_len = RSA_size(rsa);
        int block_len = key_len - 11;    // ��Ϊ��䷽ʽΪRSA_PKCS1_PADDING, ����Ҫ��key_len�����ϼ�ȥ11

        // �����ڴ棺�������ܺ����������
        char* sub_text = new char[key_len + 1];
        memset(sub_text, 0, key_len + 1);
        int ret = 0;
        int pos = 0;
        std::string sub_str;
        // �����ݽ��зֶμ��ܣ�����ֵ�Ǽ��ܺ����ݵĳ��ȣ�
        
        while (pos < size) {
            sub_str = clear_text.substr(pos, block_len);
            memset(sub_text, 0, key_len + 1);
            ret = RSA_public_encrypt(sub_str.length(), (const unsigned char*)sub_str.c_str(), (unsigned char*)sub_text, rsa, RSA_PKCS1_PADDING);
            if (ret >= 0) {
                encrypt_text.append(std::string(sub_text, ret));
            }
            pos += block_len;
        }

        // �ͷ��ڴ�  
        BIO_free_all(keybio);
        RSA_free(rsa);
        delete[] sub_text;

        return encrypt_text;
    }
public:
    std::string encryptData(string data){
        return RsaPubEncrypt(data, this->pubkey, data.size());
    }
    void decryptFile(string from, string to)
    {
        const int BUFFER_SIZE = 384000;
        const int OUT_SIZE = 373000;
        ifstream ifs;
        ofstream ofs;
        ifs.open(from, ios::binary);
        ofs.open(to, ios::out|ios::binary);
        ifs.seekg(0, ifs.end);
        int length = (int)ifs.tellg() - sizeof(int);
        ifs.seekg(0, ifs.beg);
        std::string buffer(BUFFER_SIZE + 1, '\0');
        int offset = 0;
        int readSize = min(BUFFER_SIZE, length - offset);
        int srcLength;
        ifs.read((char *)&srcLength, sizeof(int));
        
        for (int i = 0; readSize > 0 && ifs.read(&buffer[0], readSize); i ++) {

            string out = this->RsaPriDecrypt(buffer, this->prikey, readSize);
            ofs.write(out.c_str(), min(OUT_SIZE, srcLength - i*OUT_SIZE));
            offset += readSize;
            readSize = min(BUFFER_SIZE, length - offset);

        }
        ifs.close();
        ofs.close();
        return ;


    }


    void encryptFile(string from, string to)
    {
        const int BUFFER_SIZE = 373000;
        ifstream ifs;
        ofstream ofs;
        ifs.open(from, ios::binary);
        ofs.open(to, ios::out|ios::binary);
        ifs.seekg(0, ifs.end);
        int length = (int)ifs.tellg();
        ifs.seekg(0, ifs.beg);
        std::string buffer(BUFFER_SIZE + 1, '\0');
        int offset = 0;
        int readSize = min(BUFFER_SIZE, length - offset);
        string out;
        ofs.write((char *)&length, sizeof(int));
        while (readSize > 0 && ifs.read(&buffer[0], readSize)) {
            buffer[readSize] = 0;
            out = this->RsaPubEncrypt(buffer, this->pubkey,readSize);
            //cout << this->RsaPriDecrypt(out, this->prikey, out.length());
            ofs.write(out.c_str(), out.size());
            //cout << buffer;
            offset += readSize;
            readSize = min(BUFFER_SIZE, length - offset);

        }
        ifs.close();
        ofs.close();
        return;


    }
    void setPrikey(char* prikey)
    {
        this->prikey = prikey;
        return;
    }
    void setpubkey(char* pubkey)
    {
        this->pubkey = pubkey;
        return;
    }
    char* getPubkey() {
        return pubkey;
    }
    char* getPrikey() {
        return prikey;
    }

    void genKeys()
    {


        RSA* rsa = RSA_new();
        BIGNUM* e = BN_new();
        BN_set_word(e, 17);
        BN_GENCB* gencb = NULL;
        EVP_PKEY* pkey = EVP_PKEY_new();
        int rst = RSA_generate_key_ex(rsa, 3072, e, gencb);
        rst = EVP_PKEY_set1_RSA(pkey, rsa);

        BIO* pri = BIO_new(BIO_s_mem());
        BIO* pub = BIO_new(BIO_s_mem());

        rst = PEM_write_bio_RSAPublicKey(pub, rsa);
        rst = PEM_write_bio_RSAPrivateKey(pri, rsa, 0, 0, 0, 0, 0);

        int prilen, publen;


        prilen = BIO_pending(pri);
        publen = BIO_pending(pub);
        this->prikey = (char*)malloc(prilen + 1);
        this->pubkey = (char*)malloc(publen + 1);
        BIO_read(pri, this->prikey, prilen);
        BIO_read(pub, this->pubkey, publen);
        this->prikey[prilen] = '\0';
        this->pubkey[publen] = '\0';

        RSA_free(rsa);
        BN_free(e);
        EVP_PKEY_free(pkey);
        BN_GENCB_free(gencb);
    }

};


#endif //WANNAFLY_ENCRPTYFILE_H
