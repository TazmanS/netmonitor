#include "app.h"
#include <unistd.h>

int main(void)
{
  app_context_t context;

  if (init_app(&context) != NM_OK)
  {
    return 1;
  }

  while (1)
  {
    sleep(1);

    update_app(&context);

    start_app(&context);
  }

  return 0;
}