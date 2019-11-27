#ifndef __HALIBASE_
#define __HALIBASE_







//safe len
#define TLV_TAG_LEN 2
#define TLV_VALUE_LEN sizeof(int) +2

#define STLV_PACK_LONG(p , v) pack_stlv_long((p) , (v))


#define TLV_PACK_SAFE_LEN(vlen) (TLV_TAG_LEN + TLV_VALUE_LEN+ vlen) //tag+len+most(len.extra)+value_len
#define TLV_BUFF_ALLOC(vlen) ((unsigned char *)calloc(1 , TLV_PACK_SAFE_LEN((vlen))))

/*
 * STLV
 */
typedef struct
{
	STLV_TAG tag;
	STLV_LEN len;
	unsigned char value[0];	//addr will be floated by len.extra in constructed obj
	//unsigned short check_sum  is append to tail of value
}STLV_PACK;

/*
 * STLV_ENV
 */
typedef struct
{
	int slog_fd; //slog descriptor
}STLV_ENV;



#endif




