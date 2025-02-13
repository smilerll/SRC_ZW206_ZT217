
/***************************************************************************
 *                                                                         *
 * Input method library database API                                       *
 *                                                                         *
 * Copyright (C) 2000-2007 Beijing Komoxo Inc.                             *
 * All rights reserved.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef KMXIME_H
#define KMXIME_H

#ifdef __cplusplus 
extern "C"{ 
#endif 

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Language database list                                                  */
/*                                                                         */
/*-------------------------------------------------------------------------*/

extern const void* kmx_ldb_pinyin_chs;
extern const void* kmx_ldb_pinyin_cht;

extern const void* kmx_ldb_pinyin_gb2312;
extern const void* kmx_ldb_pinyin_gbk;
extern const void* kmx_ldb_pinyin_gb18030;
extern const void* kmx_ldb_pinyin_big5;


extern const void* kmx_ldb_pinyin_chs_suretype;
extern const void* kmx_ldb_pinyin_cht_suretype;

extern const void* kmx_ldb_pinyin_gb2312_suretype;
extern const void* kmx_ldb_pinyin_gb18030_suretype;
extern const void* kmx_ldb_pinyin_big5_suretype;


extern const void* kmx_ldb_zhuyin_chs;
extern const void* kmx_ldb_zhuyin_cht;

extern const void* kmx_ldb_zhuyin_gb2312;
extern const void* kmx_ldb_zhuyin_gb18030;
extern const void* kmx_ldb_zhuyin_big5;
extern const void* kmx_ldb_zhuyin_big5normal;
extern const void* kmx_ldb_zhuyin_big5hksiIME;

extern const void* kmx_ldb_pinyin_danzi;
extern const void* kmx_ldb_zhuyin_danzi;

extern const void* kmx_ldb_stroke_chs;
extern const void* kmx_ldb_stroke_gb2312;
extern const void* kmx_ldb_stroke_gbk;
extern const void* kmx_ldb_stroke_gb18030;
extern const void* kmx_ldb_stroke_big5;
extern const void* kmx_ldb_stroke_big5hksiIME;

extern const void* kmx_ldb_armenian;
extern const void* kmx_ldb_arabic;
extern const void* kmx_ldb_bengali;
extern const void* kmx_ldb_bulgarian;
extern const void* kmx_ldb_croatian;
extern const void* kmx_ldb_czech;
extern const void* kmx_ldb_danish;
extern const void* kmx_ldb_dutch;
extern const void* kmx_ldb_english;
extern const void* kmx_ldb_finnish;
extern const void* kmx_ldb_french;
extern const void* kmx_ldb_german;
extern const void* kmx_ldb_greek;
extern const void* kmx_ldb_hebrew;
extern const void* kmx_ldb_hindi;
extern const void* kmx_ldb_hinglish;
extern const void* kmx_ldb_hungarian;
extern const void* kmx_ldb_indonesian;
extern const void* kmx_ldb_italian;
extern const void* kmx_ldb_lithuanian;
extern const void* kmx_ldb_malay;
extern const void* kmx_ldb_norwegian;
extern const void* kmx_ldb_persian;
extern const void* kmx_ldb_polish;
extern const void* kmx_ldb_portuguese;
extern const void* kmx_ldb_romanian;
extern const void* kmx_ldb_russian;
extern const void* kmx_ldb_slovak;
extern const void* kmx_ldb_slovenian;
extern const void* kmx_ldb_spanish;
extern const void* kmx_ldb_swahili;
extern const void* kmx_ldb_swedish;
extern const void* kmx_ldb_tagalog;
extern const void* kmx_ldb_kyrgzy;
extern const void* kmx_ldb_thai;
extern const void* kmx_ldb_tibetan;
extern const void* kmx_ldb_turkish;
extern const void* kmx_ldb_ukrainian;
extern const void* kmx_ldb_vietnamese;
extern const void* kmx_ldb_hangul;

extern const void* kmx_ldb_english_suretype;
extern const void* kmx_ldb_uyghur;
extern const void* kmx_ldb_urdu;

extern const void* kmx_ldb_telugu;
extern const void* kmx_ldb_marathi;
extern const void* kmx_ldb_tamil;
extern const void* kmx_ldb_punjabi;
extern const void* kmx_ldb_gujarati;
extern const void* kmx_ldb_oriya;
extern const void* kmx_ldb_kannada;
extern const void* kmx_ldb_malayalam;
extern const void* kmx_ldb_myanmar;
extern const void* kmx_ldb_khmer;
extern const void* kmx_ldb_lao;
extern const void* kmx_ldb_amharic;

extern const void* kmx_ldb_assamese;
extern const void* kmx_ldb_nepali;
extern const void* kmx_ldb_bodo;//daodejing
extern const void* kmx_ldb_dogri;
extern const void* kmx_ldb_sanskrit;
extern const void* kmx_ldb_kashmiri;
extern const void* kmx_ldb_konkani;
extern const void* kmx_ldb_maithili;
extern const void* kmx_ldb_manipuri;
extern const void* kmx_ldb_santali;
extern const void* kmx_ldb_somali;
extern const void* kmx_ldb_sindhi;
extern const void* kmx_ldb_albanian;
extern const void* kmx_ldb_catalan;
extern const void* kmx_ldb_basque;
extern const void* kmx_ldb_galician;
extern const void* kmx_ldb_igbo;

extern const void* kmx_ldb_serbian;
extern const void* kmx_ldb_serbianc;

extern const void* kmx_ldb_zulu;
extern const void* kmx_ldb_cebuano;

extern const void* kmx_ldb_kazakh;
extern const void* kmx_ldb_uzbek;
extern const void* kmx_ldb_georgian;

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* General input method interface                                          */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define KMX_IME_ENGINE_SIZE         (22 * 1024)

typedef struct
{
    int                             *engine;
} KMX_IME_ENGINE;

typedef struct
{
    const void*                     database;
    void*                           user_data;
    unsigned int                    user_data_size;
    int                             flags;
    const unsigned short*           letter_group;
} KMX_IME_ENGINE_INIT;

#define KMX_MAX_WORD_MATCH          9

typedef struct
{
    const unsigned short*           text_before;
    const unsigned short*           text_after;
    unsigned short                  text_before_length;
    unsigned short                  text_after_length;
} KMX_INPUT_CONTEXT;

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Chinese pinyin input method interface                                   */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define KMX_PINYIN_FUZZY_Z_ZH       0x0001
#define KMX_PINYIN_FUZZY_C_CH       0x0002
#define KMX_PINYIN_FUZZY_S_SH       0x0004
#define KMX_PINYIN_FUZZY_H_F        0x0008
#define KMX_PINYIN_FUZZY_N_L        0x0010
#define KMX_PINYIN_FUZZY_IN_ING     0x0100
#define KMX_PINYIN_FUZZY_EN_ENG     0x0200
#define KMX_PINYIN_FUZZY_AN_ANG     0x0400
#define KMX_PINYIN_FUZZY_IAN_IANG   0x0800
#define KMX_PINYIN_FUZZY_UAN_UANG   0x1000

#define KMX_PINYIN_SEPARATOR        0x20

#define KMX_MAX_CHAR_PINYIN         6
#define KMX_MAX_PINYIN_WORD_LENGTH  8

#define KMX_MAX_PINYIN_INPUT        ((KMX_MAX_CHAR_PINYIN + 2) * KMX_MAX_PINYIN_WORD_LENGTH)
#define KMX_MAX_PINYIN_COMPOSITION  KMX_MAX_PINYIN_INPUT

#define KMX_MAX_DIGIT_PINYIN_MATCH  20
#define KMX_PINYIN_MATCH_INDEX_NONE 0xFFFF

typedef struct
{
    char                            string[KMX_MAX_CHAR_PINYIN];
    unsigned short                  length;
} KMX_CHAR_PINYIN;

typedef struct
{
    char                            string[KMX_MAX_PINYIN_COMPOSITION];
    unsigned short                  length;
} KMX_PINYIN_COMPOSITION;

typedef struct
{
    KMX_CHAR_PINYIN                 table[KMX_MAX_DIGIT_PINYIN_MATCH];
    unsigned short                  count;
} KMX_PINYIN_MATCH_RESULT;

typedef struct
{
    unsigned short                  string[KMX_MAX_PINYIN_WORD_LENGTH];
    unsigned short                  length;
    unsigned short                  match_length;
} KMX_PINYIN_CANDIDATE;

typedef struct
{
    KMX_PINYIN_CANDIDATE            table[KMX_MAX_WORD_MATCH];
    unsigned short                  count;
    unsigned short                  more_available;
} KMX_PINYIN_WORD_MATCH_RESULT;

int kmx_pinyin_initialize(
    KMX_IME_ENGINE*                 engine,
    const KMX_IME_ENGINE_INIT*      init_info);

int kmx_digit_pinyin_match(
    KMX_IME_ENGINE*                 engine,
    const char*                     digit_string,
    unsigned short                  digit_string_length,
    unsigned short                  pinyin_match_index,
    unsigned short                  word_match_index,
    KMX_PINYIN_COMPOSITION*         pinyin_composition,
    KMX_PINYIN_MATCH_RESULT*        pinyin_match_result,
    KMX_PINYIN_WORD_MATCH_RESULT*   word_match_result);

int kmx_digit_pinyin_match_length(
    KMX_IME_ENGINE*                 engine,
    const char*                     digit_string,
    unsigned short                  digit_string_length,
    unsigned short                  pinyin_match_index,
    unsigned short                  word_match_index,
    unsigned short                  max_match_word_length,
    KMX_PINYIN_COMPOSITION*         pinyin_composition,
    KMX_PINYIN_MATCH_RESULT*        pinyin_match_result,
    KMX_PINYIN_WORD_MATCH_RESULT*   word_match_result);

int kmx_pinyin_match(
    KMX_IME_ENGINE*                 engine,
    const char*                     pinyin_string,
    unsigned short                  pinyin_string_length,
    unsigned short                  word_match_index,
    KMX_PINYIN_WORD_MATCH_RESULT*   word_match_result);

int kmx_pinyin_match_length(
    KMX_IME_ENGINE*                 engine,
    const char*                     pinyin_string,
    unsigned short                  pinyin_string_length,
    unsigned short                  word_match_index,
    unsigned short                  max_match_word_length,
    KMX_PINYIN_WORD_MATCH_RESULT*   word_match_result);

/* this interface is for huawei only */
int kmx_pinyin_match_1(
    KMX_IME_ENGINE*                 engine,
    const char*                     pinyin_string,
    unsigned short                  pinyin_string_length,
    unsigned short                  word_match_index,
    KMX_PINYIN_WORD_MATCH_RESULT*   word_match_result);

int kmx_pinyin_update_cache(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           string,
    unsigned short                  length);

int kmx_pinyin_check_association(
    KMX_IME_ENGINE*                 engine,
    unsigned short                  char_code,
    unsigned short*                 buffer,
    int                             number);

#ifndef iIMEK2API_H
typedef int (*CALLBACK_READ_UPDATE_DATA)(unsigned char *buffer, int pos, int length);
#endif

int kmx_pinyin_set_update_callback(
    KMX_IME_ENGINE*                 engine,
    CALLBACK_READ_UPDATE_DATA       pfnReadUpdateData);

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Chinese zhuyin input method interface                                   */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define KMX_MAX_CHAR_ZHUYIN         4
#define KMX_MAX_ZHUYIN_WORD_LENGTH  8
#define KMX_MAX_ZHUYIN_INPUT        (KMX_MAX_CHAR_ZHUYIN * KMX_MAX_ZHUYIN_WORD_LENGTH)

typedef struct
{
    unsigned short                  string[KMX_MAX_CHAR_ZHUYIN];
    unsigned short                  length;
} KMX_CHAR_ZHUYIN;

typedef struct
{
    unsigned short                  string[KMX_MAX_ZHUYIN_WORD_LENGTH];
    unsigned short                  length;
    unsigned short                  match_length;
} KMX_ZHUYIN_CANDIDATE;

typedef struct
{
    KMX_ZHUYIN_CANDIDATE            table[KMX_MAX_WORD_MATCH];
    unsigned short                  count;
    unsigned short                  more_available;
} KMX_ZHUYIN_WORD_MATCH_RESULT;

int kmx_zhuyin_initialize(
    KMX_IME_ENGINE*                 engine,
    const KMX_IME_ENGINE_INIT*      init_info);

int kmx_zhuyin_match(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           zhuyin_string,
    unsigned short                  zhuyin_string_length,
    unsigned short                  word_match_index,
    KMX_ZHUYIN_WORD_MATCH_RESULT*   word_match_result);

int kmx_zhuyin_match_length(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           zhuyin_string,
    unsigned short                  zhuyin_string_length,
    unsigned short                  word_match_index,
    unsigned short                  max_match_word_length,
    KMX_ZHUYIN_WORD_MATCH_RESULT*   word_match_result);

int kmx_digit_to_zhuyin(
    KMX_IME_ENGINE*                 engine,
    const char*                     digit_string,
    unsigned short                  digit_length,
    unsigned short*                 zhuyin_string,
    unsigned short*                 p_zhuyin_number);
 
int kmx_zhuyin_update_cache(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           string,
    unsigned short                  length);

int kmx_zhuyin_check_association(
    KMX_IME_ENGINE*                 engine,
    unsigned short                  char_code,
    unsigned short*                 buffer,
    int                             number);

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Chinese stroke input method interface                                   */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define KMX_STROKE_SPACE            0x20
#define KMX_STROKE_HENG             0x31
#define KMX_STROKE_SHU              0x32
#define KMX_STROKE_PIE              0x33
#define KMX_STROKE_DIAN             0x34
#define KMX_STROKE_ZHE              0x35
#define KMX_STROKE_WILDCARD         0x36

#define KMX_MAX_STROKE_INPUT_LENGTH 64
#define KMX_MAX_STROKE_WORD_LENGTH  8

#define KMX_STROKE_FLAG_WORD        0x0001

typedef struct
{
    unsigned short                  length;
    unsigned short                  string[KMX_MAX_STROKE_WORD_LENGTH + 1];
} KMX_STROKE_CANDIDATE;

typedef struct
{
    unsigned short                  count;
    unsigned short                  more_available;
    KMX_STROKE_CANDIDATE            table[KMX_MAX_WORD_MATCH];
} KMX_STROKE_MATCH_RESULT;

typedef struct
{
    int                             flags;
} KMX_STROKE_ENGINE_PROPERTIES;

int kmx_stroke_initialize(
    KMX_IME_ENGINE*                 engine,
    const KMX_IME_ENGINE_INIT*      init_info);

int kmx_stroke_query_engine_properties(
    KMX_IME_ENGINE*                 engine,
    KMX_STROKE_ENGINE_PROPERTIES*   properties);

int kmx_stroke_match(
    KMX_IME_ENGINE*                 engine,
    const char*                     stroke_string,
    unsigned short                  stroke_string_length,
    unsigned short                  match_index,
    KMX_STROKE_MATCH_RESULT*        match_result);

int kmx_stroke_update_cache(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           string,
    unsigned short                  length);

#ifndef iIMEK2API_H
typedef int (*CALLBACK_CHECK_VALID_HANZI)(unsigned short unichr);
#endif

int kmx_stroke_set_callback(
	KMX_IME_ENGINE*					engine,
	CALLBACK_CHECK_VALID_HANZI		callback);

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Additional input method interface for Chinese                           */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define KMX_MAX_ASSOCIATION_QUERY_BUFFER    32

int kmx_chinese_query_association(
    const void*                     database,
    unsigned short                  char_code,
    unsigned short*                 buffer,
    int                             number);

int kmx_chinese_query_pinyin(
    const void*                     database,
    unsigned short                  char_code,
    KMX_CHAR_PINYIN*                pinyin);

int kmx_chinese_query_pinyin_dyz(
    const void*                     database,
    unsigned short                  char_code,
    KMX_CHAR_PINYIN*                pinyin,
    int                             maxDyz);

int kmx_chinese_query_zhuyin(
    const void*                     database,
    unsigned short                  char_code,
    KMX_CHAR_ZHUYIN*                zhuyin);

int kmx_chinese_query_stroke(
    const void*                     database,
    unsigned short                  char_code,
    char*                           buffer,
    unsigned short                  buffer_length);

int kmx_chinese_mars_translate(
    const void*                     database,
    unsigned short*                 buffer,
    unsigned short                  buffer_length);

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Input method interface for latin-based languages                        */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#define KMX_MAX_LATIN_WORD_LENGTH   32

#define KMX_LATIN_LOWER_CASE        0x0001
#define KMX_LATIN_UPPER_CASE        0x0002
#define KMX_LATIN_INITIAL_CASE      0x0004
#define KMX_LATIN_NUMBER_FIRST      0x0008

typedef struct
{
    unsigned short                  string[KMX_MAX_LATIN_WORD_LENGTH];
    unsigned short                  length;
} KMX_LATIN_CANDIDATE;

typedef struct
{
    KMX_LATIN_CANDIDATE             table[KMX_MAX_WORD_MATCH];
    unsigned short                  count;
    unsigned short                  more_available;
} KMX_LATIN_MATCH_RESULT;

int kmx_latin_initialize(
    KMX_IME_ENGINE*                 engine,
    const KMX_IME_ENGINE_INIT*      init_info);

int kmx_latin_match(
    KMX_IME_ENGINE*                 engine,
    const KMX_INPUT_CONTEXT*        context,
    const char*                     input_string,
    unsigned short                  input_string_length,
    unsigned short                  match_flags,
    unsigned short                  match_index,
    KMX_LATIN_MATCH_RESULT*         match_result);

int kmx_latin_query_association(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           input_string,
    unsigned short                  input_string_length,
    unsigned short                  match_flags,
    unsigned short                  match_index,
    KMX_LATIN_MATCH_RESULT*         match_result);

int kmx_latin_query_association_ex(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           input_string,
    unsigned short                  input_string_length,
    unsigned short                  match_flags,
    unsigned short                  match_index,
    KMX_LATIN_MATCH_RESULT*         match_result);

int kmx_latin_update_cache(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           string,
    unsigned short                  length);

int kmx_latin_query_key_map(
    const void*                     database,
    unsigned short                  char_code,
    unsigned short*                 key_code);

int kmx_latin_query_multitap(
    const void*                     database,
    unsigned short                  key,
    unsigned short                  flags,
    const unsigned short**          string,
    unsigned short*                 length);

#define KMX_MAX_HANGUL_MULTITAP_STRING_LENGTH 6

int kmx_hangul_query_multitap(
    unsigned short                  comp_char,
    unsigned short                  key,    
    unsigned short*                 string_buffer,
    unsigned short*                 length,
    unsigned short*                 commit_char);

int kmx_is_valid_thai_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_hindi_word_start_char(
    unsigned short              c);

int kmx_is_valid_hindi_follow_char(
    unsigned short              c1,
    unsigned short              c2);
//IM_NEPALI_SUPPORT
int kmx_is_valid_nepali_word_start_char(
    unsigned short              c);

int kmx_is_valid_nepali_follow_char(
    unsigned short              c1,
    unsigned short              c2);

//daodejing
int kmx_is_valid_bodo_word_start_char(
    unsigned short              c);

int kmx_is_valid_bodo_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_dogri_word_start_char(
    unsigned short              c);

int kmx_is_valid_dogri_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_sanskrit_word_start_char(
    unsigned short              c);

int kmx_is_valid_sanskrit_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_kashmiri_word_start_char(
    unsigned short              c);

int kmx_is_valid_kashmiri_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_konkani_word_start_char(
    unsigned short              c);

int kmx_is_valid_konkani_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_maithili_word_start_char(
    unsigned short              c);

int kmx_is_valid_maithili_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_manipuri_word_start_char(
    unsigned short              c);

int kmx_is_valid_manipuri_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_santali_word_start_char(
    unsigned short              c);

int kmx_is_valid_santali_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_somali_word_start_char(
    unsigned short              c);

int kmx_is_valid_somali_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_sindhi_word_start_char(
    unsigned short              c);

int kmx_is_valid_sindhi_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_bengali_word_start_char(
    unsigned short              c);

int kmx_is_valid_bengali_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int dummy_is_valid_bengali_word_start_char(
    unsigned short              c);

int dummy_is_valid_bengali_follow_char(
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_telugu_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_telugu_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_marathi_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_marathi_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_tamil_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_tamil_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_punjabi_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_punjabi_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_gujarati_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_gujarati_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_oriya_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_oriya_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_kannada_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_kannada_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_malayalam_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_malayalam_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_khmer_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_khmer_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_lao_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_lao_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

int kmx_is_valid_myanmar_word_start_char(
    const void*                     database,
    unsigned short              c);

int kmx_is_valid_myanmar_follow_char(
    const void*                     database,
    unsigned short              c1,
    unsigned short              c2);

/* Tibetan */

/* Tibetan character types */
#define KMX_LDB_TIBETAN_OOV       0       /* Not a Tibetan letter */
#define KMX_LDB_TIBETAN_CON       1       /* Consonants */
#define KMX_LDB_TIBETAN_JOC       2       /* Subjoined Consonants  */
#define KMX_LDB_TIBETAN_DEV       3       /* Dependent vowels */
#define KMX_LDB_TIBETAN_VIR       4       /* Virama sign */
#define KMX_LDB_TIBETAN_SIG       5       /* Special signs */

unsigned char kmx_get_tibetan_char_type(
    const void*                 database,
    unsigned short              c);
    
int kmx_is_valid_tibetan_start_char(
    const void*                 database,
    unsigned short              c);
    
int kmx_is_valid_tibetan_follow_char(
    const void*                 database,
    unsigned short              c1,
    unsigned short              c2);
    
unsigned short kmx_tibetan_consonant_to_subjoined(
    const void*                 database,
    unsigned short              c);


unsigned short kmx_tibetan_subjoined_to_consonant(
    const void*                 database,
    unsigned short              c);
    
int kmx_is_sou_delimiter(
    KMX_IME_ENGINE*             engine,
    unsigned short              c);

int kmx_latin_to_lower_case(
    const void*         database,
    unsigned short*		str,
	unsigned short		len);

int kmx_latin_to_upper_case(
    const void*         database,
    unsigned short*		str,
	unsigned short		len);

int kmx_latin_to_initial_case(
    const void*         database,
    unsigned short*		str,
	unsigned short		len);


/* 
 * Ethiopic language use Ge'ez script
 */

#ifndef iIME_LANG_AMHARIC
    #define iIME_LANG_AMHARIC         88
#endif

#ifndef iIME_LANG_TIGRE
    #define iIME_LANG_TIGRE           75
#endif

#ifndef iIME_LANG_TIGRINYA
    #define iIME_LANG_TIGRINYA        76
#endif

#define MAX_ETHIOPIC_CANDIDATE  8

/*
 *  the key is 0-10, the 10 means star
 */

int kmx_ethiopic_query_leader(int               lang,     
                              unsigned short    key,
                              unsigned short    *string,
                              unsigned short    *length);

int kmx_ethiopic_query_multimap(int             lang,
                                unsigned short  leader,
                                unsigned short  *string,
                                unsigned short  *length);

int kmx_ethiopic_query_symbol(int               lang,     
                              unsigned short    key,
                              unsigned short    *string,
                              unsigned short    *length);

/* 
 * raw_ldb_buffer is the original content read from the ldb file, ldb_buffer_size is the actual size of ldb file.
 * after the function successfully returned, pointer points to ldb_header be used as database for all the 
 * ldb interface. 
 */
#define MAX_LDB_HEADER_SIZE 256

typedef struct
{
    unsigned char header[MAX_LDB_HEADER_SIZE];
} KMX_LDB_HEADER;

int kmx_latin_relocate_ldb(
    const void* raw_ldb_buffer, 
    int ldb_buffer_size,
    KMX_LDB_HEADER *ldb_header);

int kmx_stroke_relocate_ldb(
    const void* raw_ldb_buffer, 
    int ldb_buffer_size,
    KMX_LDB_HEADER *ldb_header);
    
int kmx_pinyin_relocate_ldb(
    const void* raw_ldb_buffer, 
    int ldb_buffer_size,
    KMX_LDB_HEADER *ldb_header);

#ifdef __cplusplus 
} 
#endif 

#endif /* KMXIME_H */
