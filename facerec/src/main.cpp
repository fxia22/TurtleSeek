#include <stdio.h>
#include "Recognizer.h"

int main(int argc, char** argv)
{
    printf("Recognizer began to work\n");
    Recognizer* rec = new Recognizer();
    bool flag = rec->Init(argc, argv);

    if (flag)
    {
        rec->Run();
    };

    delete rec;
    printf("Bye!\n");
    return 0;
}