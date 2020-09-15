#include<stdio.h>
#include<string.h>

int main(int argc, char * * argv)
{
    int pix = 0;

#if __AVX2__
      _(avx2, "AVX2")
      pix = 1;
#endif
#if __AVX__
        _(avx, "AVX")
      pix = 2;
#endif
#if __SSE4_2__
        _(sse42, "SSE4.2")
      pix = 3;
#endif
#if __SSE4_1__
        _(sse41, "SSE4.1")
      pix = 4;
#endif
#if __SSSE3__
        _(ssse3, "SSSE3")
      pix = 5;
#endif
#if __SSE3__
        _(sse3, "SSE3")
      pix = 6;
#endif

    printf("pix = %d \n",pix);

    return 0;
}
