/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.
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
 * Shows how the write callback function can be used to download data into a
 * chunk of memory instead of storing it in a file.
 * </DESC>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include "cJSON.h"
#include "json.h"
#include "getinmemory.h"
FILE *fp;
struct MemoryStruct {
  char *memory;
  size_t size;
};

//weather stweather;
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
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
#define CURL_UPDATA 1
int curl_get_weather(char *pvstr,weather *pstweather)
//int main(void)
{ 
  CURL *curl_handle;
  struct curl_slist *chunk_h = NULL;
  CURLcode res;
#if CURL_UPDATA
  struct MemoryStruct chunk;

  chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, "https://free-api.heweather.com/s6/weather/forecast?location=CN101010100&key=f84b10202847419086d87937f79adee1");

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0);
  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);
  chunk_h = curl_slist_append(chunk_h, "User-Agent:Mozilla/5.0(Macintosh;IntelMacOSX10.10;rv:47.0)Gecko/20100101Firefox/47.0");
  curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, chunk_h);
  /* get it! */
  res = curl_easy_perform(curl_handle);
  curl_slist_free_all(chunk_h);
  /* check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
    /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */

    printf("%lu bytes retrieved\n", (long)chunk.size);
  }
  fp = fopen("./tmp.txt","wr");
  fp = fopen("./tmp.txt","w");
  if (chunk.size == fwrite(chunk.memory,1,chunk.size,fp))
  {
    printf("Write success\n");
  }
  else 
    printf("Write error\n");

    fclose(fp); 

//	memcpy(pvstr,chunk.memory,chunk.size);
//	pvstr[chunk.size] = '\0';
#endif
   //*pstweather = dofile("./tmp.txt");
   //*pstweather = (weather)0;
		
   (void)dofile("./tmp.txt",pstweather);
	



#if CURL_UPDATA
  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  free(chunk.memory);

  printf("curl_global_cleanup\n");
  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();
#endif

  return 0;
}

