#ifndef _HALIERRCODE_H
#define _HALIERRCODE_H
#ifdef __cplusplus
extern "C" {
#endif


/* Error codes passed around between file input, tokenizer, parser and
   interpreter.  This is necessary so we can turn them into Python
   exceptions at a higher level.  Note that some errors have a
   slightly different meaning when passed from the tokenizer to the
   parser than when passed from the parser to the interpreter; e.g.
   the parser only returns E_EOF when it hits EOF immediately, and it
   never returns E_OK. */

#define ERROR_SUCCESS		0	/* No error */
#define ERROR_FAILED		1	/* No error */


#ifdef __cplusplus
}
#endif
#endif

