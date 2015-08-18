#ifndef USTRVECTOR
#define USTRVECTOR

#include "ucharLib.h"
#include <ctype.h>
#include <math.h>

#ifndef NO_TGMATH   // The tgmath.h is replace by math.h in Visual Studio
#include <tgmath.h>
#endif

#define BUF_SIZE 10000
#define BASE_HASH_LEN 65536
#define MAX_UNICODE 130000  // Should be bigger than the biggest unicode code
#define HASH_SEED 0

// ----- typedef -----
typedef bool(*Checker)(const uchar uc[]);   // The pointer of checker function

/* A Parser should accept a ustring and parse it into ustring_parse_list
   ����������һ���ַ������ҽ����г��з��б� */
typedef int(*Parser)(struct ustring_parse_list * p, const struct ustring * cp_us, const Checker cf);

typedef long double Lf;
typedef int type_t; // This type is supposed to be big enough to store TYPE_COUNT

// ----- struct -----
/* List struct for analysing uchar */
struct uchar_analysis {
    llu total_count;
    lld uchar_list[MAX_UNICODE];
};

/* A link node of ustring_analysis
   ���������� */
struct ustring_analysis {
    struct ustring * us;
    lld count;
    struct ustring_analysis * next;
};

/* A hashmap to store ustring_analysis,
   which use link list to deal with collapse.
   �洢 ustring_analysis ��ɢ�б�ʹ��������������ײ */
struct hash_vector {
    llu total_count;	// the number of words that counted
    struct ustring_analysis ** usa_list;
    llu hashlen;	// the length of hashmap
    llu count;		// the number of total node in hashmap
};

/* The struct of a ustring_parse_list */
struct ustring_parse_list {
    llu * start;    // The array of starting indexes
    llu * end;  // The array of ending indexes
    llu len;    // The length of both start and end
};

// ----- function for hash_vector -----
/* */
int init_hash_vector(struct hash_vector ** pp_hv);

/* rehash p_hv to len of hashlen
   ������ p_hv ����ɢ�е����� hashlen */
int rehash_hash_vector(struct hash_vector * p_hv, llu hashlen);

/* */
int clear_hash_vector(struct hash_vector ** pp_hv);

/* Use a parse_list to build the hash_vector of a ustring
   ʹ�÷ָ��б� p_uspl �ָ��ַ��� cp_us �����뵽���� hash_vector �� */
int append_hash_vector(struct hash_vector * p_hv, const struct ustring * cp_us, const struct ustring_parse_list * p_uspl);

/* Add p_hv2 to p_hv1
   ������ p_hv2 �ӵ����� p_hv1 �� */
int add_hash_vector(struct hash_vector * p_hv1, const struct hash_vector * p_hv2);

/* Sub p_hv2 to p_hv1
   ������ p_hv1 �ϼ�ȥ���� p_hv2 */
int sub_hash_vector(struct hash_vector * p_hv1, const struct hash_vector * p_hv2);

/* Omit all nodes which have not enough count
   ȥ������������ count С�� min_count ������ */
int low_cut_hash_vector(struct hash_vector * p_hv, lld min_count);

/* Refresh the total_count and count in a vector
   ���¼��������� total_count �� count */
int recount_hash_vector(struct hash_vector * p_hv);

/* Get the dot product of two vectors
   �������������ĵ�� */
lld product_hash_vector(const struct hash_vector * p_hv1, const struct hash_vector * p_hv2);

/* Get the square of the Euclidean norm of the vector
   ����������ŷ�����ȵ�ƽ�� */
llu len2_hash_vector(const struct hash_vector * p_hv);

/* Get the cosine of the angle between two vectors
   �������������нǵ�����ֵ */
Lf cos_hash_vector(const struct hash_vector * p_hv1, const struct hash_vector * p_hv2);

// ----- predefined Parser -----
/* The Parser which omit and split where Checker returns true
   �ý�����ȥ������ Checker ���� true ���ַ����������з� */
int commonParser(struct ustring_parse_list * p, const struct ustring * cp_us, const Checker cf);

/* The Parser which split at every unicode character
   �ý�������ÿһ��unicode�ַ����з� */
int ucharParser(struct ustring_parse_list * p, const struct ustring * cp_us, const Checker cf);

// ----- function for ustring_parse_list -----
/* */
int init_uspl(struct ustring_parse_list ** pp_uspl);

/* */
int clear_uspl(struct ustring_parse_list ** pp_uspl);

// ----- I/O -----
/* */
void output_hash_vector(FILE * out, const struct hash_vector * p_hv);

/* save a vector into a binary file stream
   �������洢������������ */
int save_vector(FILE * output, const struct hash_vector * p_hv);

/* load a vector from a binary file stream
   �Ӷ��������ж�ȡһ������ */
int load_vector(FILE * input, struct hash_vector * p_hv);

#endif
