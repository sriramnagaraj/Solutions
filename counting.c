#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
   FILE *f = fopen(argv[1], "r");


   uint8_t *bitmap = calloc(1ULL << 32, 1);  // 4GB
  
   uint32_t num;
   while (fread(&num, sizeof(uint32_t), 1, f) == 1) {
       uint64_t idx = (uint64_t)num;
       uint8_t shift = (idx & 3) * 2;
       uint64_t byte_idx = idx >> 2;
      
       uint8_t count = (bitmap[byte_idx] >> shift) & 3;
       if (count < 2) {
           bitmap[byte_idx] &= ~(3 << shift);
           bitmap[byte_idx] |= ((count + 1) << shift);
       }
   }


   uint64_t unique = 0, seen_once = 0;
   for (uint64_t i = 0; i < (1ULL << 30); i++) {
       for (int j = 0; j < 4; j++) {
           uint8_t count = (bitmap[i] >> (j * 2)) & 3;
           if (count > 0) {
               unique++;
               if (count == 1) seen_once++;
           }
       }
   }


   printf("Unique numbers: %llu\n", unique);
   printf("Numbers seen only once: %llu\n", seen_once);


   free(bitmap);
}
