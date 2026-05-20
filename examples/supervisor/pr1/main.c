#include <arinc653/partition.h>
#include <core/thread.h>
#include <libc/stdio.h>

void* user_thread1 ()
{
   int counter = 0;
   while (1)
   {
      counter++;
      printf ("[Partition 0 (Worker)]: Working, step %d\n", counter);
      
      POK_HEARTBEAT_PULSE();

      pok_thread_sleep (1000000);

      if (counter == 4)
      {
         printf ("[Partition 0 (Worker)]: Critical failure\n");
         while (1) {
         }
      }
   }
}

int main ()
{
   uint32_t tid;
   pok_thread_attr_t attr;

   attr.priority = 42;
   attr.entry = user_thread1;
   
   pok_thread_create (&tid, &attr);

   pok_thread_wait_infinite ();
   return (0);
}
