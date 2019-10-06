// Arquivo mq-send.c: envia mensagens para uma fila de mensagens POSIX.
// Em Linux, compile usando: gcc -o fila-msg-send fila-msg-send.c -lrt
#include<stdio.h>
#include<stdlib.h>
#include<mqueue.h>
#include<unistd.h>

#define QUEUE "/my_queue"

int main() {
   mqd_t queue; // descritor da fila
   int  msg; // mensagem a enviar

   // abre a fila de mensagens, se existir
   if((queue = mq_open (QUEUE, O_RDWR)) < 0){
      perror ("mq_open");
      exit (1);
   }

   for (;;) {
      //msg = random() % 100 ; // valor entre 0 e 99
      msg = 2 ; // valor entre 0 e 99
      // envia a mensagem
      if (mq_send (queue, (void*) &msg, sizeof(msg), 0) < 0){
         perror ("mq_send");
         exit (1);
      }
      printf ("Sent message with value %d\n", msg);
      sleep (1) ;
   }
   return 0;
}
