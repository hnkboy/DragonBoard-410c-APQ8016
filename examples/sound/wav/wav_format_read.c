#include <stdio.h>  
#include <stdint.h>  
#include <stdlib.h>  
  
struct WAV_Format {  
    uint8_t ChunkID[4];   /* "RIFF" */  
    uint32_t ChunkSize; /* 36 + Subchunk2Size */  
    uint8_t Format[4];    /* "WAVE" */  
  
    /* sub-chunk "fmt" */  
    uint8_t Subchunk1ID[4];   /* "fmt " */  
    uint32_t Subchunk1Size; /* 16 for PCM */  
    uint16_t AudioFormat;   /* PCM = 1*/  
    uint16_t NumChannels;   /* Mono = 1, Stereo = 2, etc. */  
    uint32_t SampleRate;    /* 8000, 44100, etc. */  
    uint32_t ByteRate;  /* = SampleRate * NumChannels * BitsPerSample/8 */  
    uint16_t BlockAlign;    /* = NumChannels * BitsPerSample/8 */     
    uint16_t BitsPerSample; /* 8bits, 16bits, etc. */  
  
    /* sub-chunk "data" */  
    uint8_t Subchunk2ID[4];   /* "data" */  
    uint32_t Subchunk2Size; /* data size */  
};   
  
int main(int argc, char *argv[])
{  
    FILE *fp = NULL;  
    struct WAV_Format wav;  
  
    fp = fopen(argv[1], "rb");  
    if (!fp) {  
        printf("can't open audio file\n");  
        exit(1);  
    }  
  
    fread(&wav, 1, sizeof(struct WAV_Format), fp);  
  
    printf("ChunkID \t%s\n", wav.ChunkID);  
    printf("ChunkSize \t%d\n", wav.ChunkSize);  
    printf("Format \t\t%s\n", wav.Format);  
    printf("Subchunk1ID \t%s\n", wav.Subchunk1ID);  
    printf("Subchunk1Size \t%d\n", wav.Subchunk1Size);  
    printf("AudioFormat \t%d\n", wav.AudioFormat);  
    printf("NumChannels \t%d\n", wav.NumChannels);  
    printf("SampleRate \t%d\n", wav.SampleRate);  
    printf("ByteRate \t%d\n", wav.ByteRate);  
    printf("BlockAlign \t%d\n", wav.BlockAlign);  
    printf("BitsPerSample \t%d\n", wav.BitsPerSample);  
    printf("Subchunk2ID \t%s\n", wav.Subchunk2ID);  
    printf("Subchunk2Size \t%d\n", wav.Subchunk2Size);  
  
    fclose(fp);  
  
    return 0;  
}  