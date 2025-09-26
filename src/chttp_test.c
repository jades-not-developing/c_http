#include "chttp.h"

#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    CHttpResponse res = chttp_get("https://jsonplaceholder.typicode.com/todos/1");
    if (!res.valid) {
        fprintf(stderr, "[Error] Failed to %s url '%s': %s", res.method, res.url, res.error);
        exit(1);
    }

    printf("Result:\n%s\n", res.result);

    return 0;
}