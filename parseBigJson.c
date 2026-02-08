#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_MODEL_LEN 64


typedef struct {
   char model[MAX_MODEL_LEN];
   long count;
} ModelCount;


ModelCount models[1000];
int model_count = 0;


int find_or_add_model(const char *model) {
   for (int i = 0; i < model_count; i++) {
       if (strcmp(models[i].model, model) == 0) {
           return i;
       }
   }
   if (model_count < 1000) {
       strncpy(models[model_count].model, model, MAX_MODEL_LEN - 1);
       models[model_count].count = 0;
       return model_count++;
   }
   return -1;
}


int main(int argc, char *argv[]) {
  
   FILE *fp = fopen(argv[1], "r");
  
   char buffer[65536];
   size_t bytes;
  
   while ((bytes = fread(buffer, 1, 65535, fp)) > 0) {
       buffer[bytes] = '\0';
      
       char *pos = buffer;
       while ((pos = strstr(pos, "\"model\":\"")) != NULL) {
           pos += 9;
           char *end = strchr(pos, '"');
           if (end) {
               char model[MAX_MODEL_LEN];
               int len = end - pos;
               if (len >= MAX_MODEL_LEN) len = MAX_MODEL_LEN - 1;
               strncpy(model, pos, len);
               model[len] = '\0';
              
               int idx = find_or_add_model(model);
               if (idx >= 0) models[idx].count++;
              
               pos = end;
           }
       }
   }

   printf("model count %d \n", model_count);
   
   printf("Model Frequency Count:\n");
   for (int i = 0; i < model_count; i++) {
       printf("%s: %ld\n", models[i].model, models[i].count);
   }
}
