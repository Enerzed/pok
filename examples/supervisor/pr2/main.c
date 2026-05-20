#include <arinc653/partition.h>
#include <arinc653/types.h>
#include <core/thread.h>
#include <libc/stdio.h>

void* supervisor_thread ()
{
   RETURN_CODE_TYPE ret;
   printf ("[Partition 1 (Supervisor)]: Watching Partition 0...\n");
   
   pok_thread_sleep (7000000);

   printf ("[Partition 1 (Supervisor)]: Partition 0 not responding REINIT_PARTITION_REMOTE...\n");
   
   REINIT_PARTITION_REMOTE(0, &ret);
   
   if (ret == NO_ERROR) {
      printf ("[Partition 1 (Supervisor)]: Reinit successful!\n");
   } else {
      printf ("[Partition 1 (Supervisor)]: Reinit failed ARINC: %d\n", ret);
   }

   while (1) {
      pok_thread_sleep (1000000);
   }
}

int main ()
{
   uint32_t tid;
   pok_thread_attr_t attr;

   attr.priority = 42;
   attr.entry = supervisor_thread;
   pok_thread_create (&tid, &attr);

   pok_thread_wait_infinite ();
   return (0);
}