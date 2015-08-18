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

// The pointer of checker function
typedef bool(*Checker)(const uchar uc[]);

/* The pointer of Parser function
A Parser should accept a ustring and parse it into ustring_parse_list
*/
typedef int(*Parser)(struct ustring_parse_list * p, const struct ustring * cp_us, const Checker cf);

typedef long double Lf;
typedef int type_t; // This type is supposed to be big enough to store TYPE_COUNT

struct uchar_analysis {
    llu total_count;
    lld uchar_list[MAX_UNICODE];
};

// A link node of ustring_analysis
struct ustring_analysis {
    struct ustring * us;
    lld count;
    struct ustring_analysis * next;
};

/* A hashmap to store ustring_analysis,
which use link list to deal with collapse.
*/
struct hash_vector {
    llu total_count;	// the number of words that counted
    struct ustring_analysis ** usa_list;
    llu hashlen;	// the length of hashmap
    llu count;		// the number of non-NULL node in hashmap
};

// The struct of a ustring_parse_list
struct ustring_parse_list {
    llu * start;    // The array of starting indexes
    llu * end;  // The array of ending indexes
    llu len;    // The length of both start and end
};

int init_hash_vector(struct hash_vector ** pp_hv);
int rehash_hash_vector(struct hash_vector * p_hv, llu hashlen);
int clear_hash_vector(struct hash_vector ** pp_hv);

// Use a parse_list to build the hash_vector of a ustring
int append_hash_vector(struct hash_vector * p_hv, const struct ustring * cp_us, const struct ustring_parse_list * p_uspl);

// Insert a ustring_analysis into hash_vector
static int insert_hash_vector(struct hash_vector * p_hv, const struct ustring * us, lld count, struct ustring_analysis * next);

// Add vector_2 to vector_1
int add_hash_vector(struct hash_vector * p_hv1, const struct hash_vector * p_hv2);

// Sub vector_2 from vector_1
int sub_hash_vector(struct hash_vector * p_hv1, const struct hash_vector * p_hv2);

// Omit all nodes which have not enough count
int low_cut_hash_vector(struct hash_vector * p_hv, lld min_count);

// Refresh the total_count and count in a vector
int recount_hash_vector(struct hash_vector * p_hv);

// Get the dot product of two vectors
lld product_hash_vector(const struct hash_vector * p_hv1, const struct hash_vector * p_hv2);

// Get the square of the Euclidean norm of the vector
llu len2_hash_vector(const struct hash_vector * p_hv);

// Get the cosine of the angle between two vectors
Lf cos_hash_vector(const struct hash_vector * p_hv1, const struct hash_vector * p_hv2);

// The Parser which omit and split where Checker returns true
int commonParser(struct ustring_parse_list * p, const struct ustring * cp_us, const Checker cf);

// The Parser which split at every unicode character
int ucharParser(struct ustring_parse_list * p, const struct ustring * cp_us, const Checker cf);

int init_uspl(struct ustring_parse_list ** pp_uspl);
int clear_uspl(struct ustring_parse_list ** pp_uspl);

void output_hash_vector(FILE * out, const struct hash_vector * p_hv);

int save_vector(FILE * output, const struct hash_vector * p_hv);
int load_vector(FILE * input, struct hash_vector * p_hv);

#endif
