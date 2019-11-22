#include <stdio.h>
#include <stdlib.h>

int main () {
   int a = 546546;
   char buffer[10];

   sprintf(buffer, "%d", a);

   printf("%s\n", buffer);


   return(0);
}