#include <arinc653/partition.h>
#include <arinc653/types.h>
#include <core/thread.h>
#include <libc/stdio.h>

void* supervisor_thread ()
{
   RETURN_CODE_TYPE ret_arinc;
   uint32_t last_heartbeat = 0;
   uint32_t current_heartbeat = 0;

   printf ("[Partition 1 (Supervisor)]: monitoring started.\n");
   
   while (1)
   {
      pok_thread_sleep (2000000); 

      current_heartbeat = POK_HEARTBEAT_CHECK(0);

      if (current_heartbeat == last_heartbeat && current_heartbeat > 0)
      {
         printf ("[Partition 1 (Supervisor)]: Detected failure didn't receive heartbeat in time:!\n");
         printf ("[Partition 1 (Supervisor)]: Calling REINIT_PARTITION_REMOTE... \n");
         
         REINIT_PARTITION_REMOTE (0, &ret_arinc);
         
         current_heartbeat = 0;
         last_heartbeat = 0;
         
         pok_thread_sleep (3000000);
      }
      else
      {
         // Система здорова, обновляем значение
         last_heartbeat = current_heartbeat;
      }
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
