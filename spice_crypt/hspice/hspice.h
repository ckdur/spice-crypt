typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef char    sbyte;
typedef unsigned char    dwfenc;
typedef unsigned int    dword;
typedef long double    longdouble;
typedef long long    longlong;
typedef unsigned long    qword;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined3;
typedef unsigned int    undefined4;
typedef unsigned long    undefined5;
typedef unsigned long    undefined6;
typedef unsigned long    undefined7;
typedef unsigned long    undefined8;
typedef unsigned short    ushort;
typedef int    wchar_t;
typedef unsigned short    word;
typedef struct Randkeydecipher Randkeydecipher, *PRandkeydecipher;

typedef struct TDESDecipher_ptr_table TDESDecipher_ptr_table, *PTDESDecipher_ptr_table;

typedef struct istream istream, *Pistream;

typedef struct ostream ostream, *Postream;

struct TDESDecipher_ptr_table {
    undefined (*Init)(ulonglong, char *, char *);
    undefined (*ReadCipherHead)(void *, struct istream *, int, char *, int *);
    undefined (*ReadCipherText)(void *, struct istream *, struct ostream *);
    undefined (*ReadCipherEnd)(void *, struct istream *, struct ostream *);
    undefined (*ParseData)(void *, struct istream *, struct ostream *, int, char *, int *);
    void (*Delete1)(void);
    void (*Delete2)(void);
    undefined (*InitKey)(struct istream *);
    undefined (*InitHead)(void *, struct istream *, int, char *, int *);
};

struct Randkeydecipher { /* PlaceHolder Structure */
    struct TDESDecipher_ptr_table *functable;
    undefined valid1;
    undefined field2_0x9;
    undefined field3_0xa;
    undefined field4_0xb;
    undefined field5_0xc;
    undefined field6_0xd;
    undefined field7_0xe;
    undefined field8_0xf;
    long from_param1_assign_1;
    byte subkey[3][8];
    byte select2_dest[48][6];
    byte select1_dest[3][7];
    byte sha_rand_key[72];
    undefined field21_0x1ad;
    undefined field22_0x1ae;
    undefined field23_0x1af;
    byte *sha;
};

typedef Randkeydecipher TDESdecipher;

struct des {
    byte key[8];
};

