// programmed by Daniil Kistanov 5250

#include <stdio.h>
#include <emmintrin.h>
#include <string.h>

static __m128i regRotateLeft(__m128i x, int count) {
    return _mm_or_si128(_mm_slli_epi32 (x, count),_mm_srli_epi32 (x, 32-count));
}

void P52(unsigned s[12]) {
    
    //load items into registers
    __m128i first_row  = _mm_loadu_si128 ((__m128i *)s);
    __m128i second_row = _mm_loadu_si128 ((__m128i *)&s[4]);
    __m128i third_row  = _mm_loadu_si128 ((__m128i *)&s[8]);
    
    //copy registers into temporary registers
    __m128i x = _mm_loadu_si128 (&first_row);
    __m128i y = _mm_loadu_si128 (&second_row);
    __m128i z = _mm_loadu_si128 (&third_row);
    
    int *p = (int *)&first_row; //pointer to first value of first row
    
    for(int i = 0; i < 24; i++) {
        x = _mm_loadu_si128 (&first_row);
        y = _mm_loadu_si128 (&second_row);
        z = _mm_loadu_si128 (&third_row);
        
        x = regRotateLeft(x, 24);
        y = regRotateLeft(y, 9);
        
        third_row  = _mm_xor_si128(x,_mm_xor_si128(_mm_slli_epi32(z, 1),_mm_slli_epi32(_mm_and_si128(y,z), 2)));
        second_row = _mm_xor_si128(y,_mm_xor_si128(x,_mm_slli_epi32(_mm_or_si128(x,z), 1)));
        first_row  = _mm_xor_si128(z,_mm_xor_si128(y,_mm_slli_epi32(_mm_and_si128(x,y), 3)));
        
        if (i % 4 == 0) {
            first_row = _mm_shuffle_epi32(first_row,_MM_SHUFFLE(2, 3, 0, 1));
            p[0] = p[0] ^ 0x9e377900 ^ (24-i);
        } else if (i % 4 == 2) {
            first_row = _mm_shuffle_epi32(first_row,_MM_SHUFFLE(1, 0, 3, 2));
        }
    }
    // put permuted values back to s
    memcpy(s,&first_row,16);
    memcpy(s+4,&second_row,16);
    memcpy(s+8,&third_row,16);
}
