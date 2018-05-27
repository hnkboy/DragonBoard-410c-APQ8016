/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * Issue an HTTP POST and provide the data through the read callback.
 * </DESC>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/time.h>
#include "md5.h"
/* silly test data to POST */
static const char data[]="Lorem ipsum dolor sit amet, consectetur adipiscing "
  "elit. Sed vel urna neque. Ut quis leo metus. Quisque eleifend, ex at "
  "laoreet rhoncus, odio ipsum semper metus, at tempus ante urna in mauris. "
  "Suspendisse ornare tempor venenatis. Ut dui neque, pellentesque a varius "
  "eget, mattis vitae ligula. Fusce ut pharetra est. Ut ullamcorper mi ac "
  "sollicitudin semper. Praesent sit amet tellus varius, posuere nulla non, "
  "rhoncus ipsum.";

struct WriteThis {
  const char *readptr;
  size_t sizeleft;
};

static size_t read_callback(void *dest, size_t size, size_t nmemb, void *userp)
{
  struct WriteThis *wt = (struct WriteThis *)userp;
  size_t buffer_size = size*nmemb;
  FILE *fp;
  printf("callback\n");
  if(wt->sizeleft) {
    /* copy as much as possible from the source to the destination */
    size_t copy_this_much = wt->sizeleft;
    if(copy_this_much > buffer_size)
      copy_this_much = buffer_size;
    memcpy(dest, wt->readptr, copy_this_much);

    wt->readptr += copy_this_much;
    wt->sizeleft -= copy_this_much;
    return copy_this_much; /* we copied this many bytes */
  }

  return 0; /* no more data left to deliver */
}
struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}
int curl_post_data(char *svstr, int strlenth)
{
  CURL *curl;
  CURLcode res;

  struct curl_slist *chunk = NULL;
  struct curl_httppost *post=NULL;
  struct curl_httppost *last=NULL;

  char cstampsec[21]={0};
  char data[100]={0};
  unsigned char md5_value[16]={0};
  char md5_value_s[33]={0};
  char md5_value_ls[100]={0};
  struct timeval tv_begin;
  MD5_CTX md5;
  struct WriteThis wt;
  int i;
  FILE *fp;

  struct MemoryStruct chunk_m;

  chunk_m.memory = malloc(1);  /* will be grown as needed by the realloc above */
  chunk_m.size = 0;    /* no data at this point */

  wt.readptr = data;
  wt.sizeleft = strlen(data);

  /* In windows, this will init the winsock stuff */
  res = curl_global_init(CURL_GLOBAL_DEFAULT);
  /* Check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed: %s\n",
            curl_easy_strerror(res));
    return 1;
  }

  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. */
    curl_easy_setopt(curl, CURLOPT_URL, "http://api.xfyun.cn/v1/service/v1/tts");

    /* Now specify we want to POST data */
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    /* we want to use our own read function */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

    /* pointer to pass to our read function */
    curl_easy_setopt(curl, CURLOPT_READDATA, &wt);

 /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk_m); 
//    curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    /* get verbose debug output please */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl,CURLOPT_COOKIEFILE,"");
/*
    curl_formadd(&post,&last,
				CURLFORM_COPYNAME,"test",
				CURLFORM_COPYCONTENTS,"father",CURLFORM_END);
*/
#if 0
    curl_formadd(&post,&last,
				CURLFORM_COPYNAME,"999",
				CURLFORM_COPYCONTENTS,"666",CURLFORM_END);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
#endif
	if (0 < strlenth && 100 > strlenth)
	{
		char text[110];
		sprintf(text,"text=%s",svstr);
		curl_easy_setopt(curl,CURLOPT_POSTFIELDS,text);
	}
    /*
      If you use POST to a HTTP 1.1 server, you can send data without knowing
      the size before starting the POST if you use chunked encoding. You
      enable this by adding a header like "Transfer-Encoding: chunked" with
      CURLOPT_HTTPHEADER. With HTTP 1.0 or without chunked transfer, you must
      specify the size in the request.
    */
//#define USE_CHUNKED
#if 0
#ifdef USE_CHUNKED
    {
      struct curl_slist *chunk = NULL;

      chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
      /* use curl_slist_free_all() after the *perform() call to free this
         list again */
    }
#else
    /* Set the expected POST size. If you want to POST large amounts of data,
       consider CURLOPT_POSTFIELDSIZE_LARGE */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)wt.sizeleft);
#endif

#ifdef DISABLE_EXPECT
    /*
      Using POST with HTTP 1.1 implies the use of a "Expect: 100-continue"
      header.  You can disable this header with CURLOPT_HTTPHEADER as usual.
      NOTE: if you want chunked transfer too, you need to combine these two
      since you can only set one list of headers with CURLOPT_HTTPHEADER. */

    /* A less good option would be to enforce HTTP 1.0, but that might also
       have other implications. */
    {
      struct curl_slist *chunk = NULL;

      chunk = curl_slist_append(chunk, "Expect:");
      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
      /* use curl_slist_free_all() after the *perform() call to free this
         list again */
    }
#endif
#endif
#if 1
      chunk = curl_slist_append(chunk, "Content-Type:application/x-www-form-urlencoded");
      chunk = curl_slist_append(chunk, "charset=utf-8");
      chunk = curl_slist_append(chunk, "auf:audio/L16;rate=16000");
      chunk = curl_slist_append(chunk, "aue:raw");
      chunk = curl_slist_append(chunk, "voice_name:xiaoyan");
      chunk = curl_slist_append(chunk, "X-Appid:5ac4bdf1");
      chunk = curl_slist_append(chunk, "X-Param:ew0KICAgICJhdWYiOiAiYXVkaW8vTDE2O3JhdGU9MTYwMDAiLA0KICAgICJhdWUiOiAicmF3IiwNCiAgICAidm9pY2VfbmFtZSI6ICJ4aWFveWFuIg0KfQ==");
      chunk = curl_slist_append(chunk, "Expect:");
#endif
#if 1
	  gettimeofday(&tv_begin,NULL);
	  printf("timestamp is %ld\n",tv_begin.tv_sec);
	 // (void)ltoa(tv_begin.tv_sec,cstampsec,10);
	  sprintf(cstampsec,"X-CurTime:%ld",tv_begin.tv_sec);
	  cstampsec[20] = '\0';
	  printf("timestamp is %s\n",cstampsec);
      chunk = curl_slist_append(chunk, cstampsec);

	  sprintf(data,"%s%ld%s","b821296f39b13f897570dda39b769b3e",tv_begin.tv_sec,"ew0KICAgICJhdWYiOiAiYXVkaW8vTDE2O3JhdGU9MTYwMDAiLA0KICAgICJhdWUiOiAicmF3IiwNCiAgICAidm9pY2VfbmFtZSI6ICJ4aWFveWFuIg0KfQ==");
	  printf("pppam is %s\n",data);
      MD5Init(&md5);
      MD5Update(&md5,data,162);
      MD5Final(&md5,md5_value);
	  for ( i =0; i < 16; i++)
	  {
	    snprintf(md5_value_s + i*2, 2+1,"%02x",md5_value[i]);
      }
	  md5_value_s[32]= '\0'; 
	  sprintf(md5_value_ls,"X-CheckSum:%s",md5_value_s);
      chunk = curl_slist_append(chunk, md5_value_ls);

#endif

      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
     // res = curl_easy_setopt(curl, CURLOPT_POSTFELDS, "text:");


		
      /* use curl_slist_free_all() after the *perform() call to free this
         list again */
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

  fp = fopen("./tmp.wav","wr");
  if (chunk_m.size == fwrite(chunk_m.memory,1,chunk_m.size,fp))
  {
    printf("Write success\n");
  }
  else 
    printf("Write file\n");

  fclose(fp); 


	curl_formfree(post);
	curl_slist_free_all(chunk);
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  free(chunk_m.memory);
  curl_global_cleanup();
  return 0;
}

