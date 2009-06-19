


#include "RuntimeSystem.h"
#include <QApplication>
#include "DebugDialog.h"

void showDebugDialog(RuntimeVariablesType * stack, int stackSize,
                     RuntimeVariablesType * heap, int heapSize,
                     MemoryType * mem, int memSize)
{
    QApplication app(0,NULL);
    DebugDialog dlg;
    dlg.setHeapVars(heap,heapSize);
    dlg.setStackVars(stack,stackSize);
    dlg.setMemoryLocations(mem,memSize);

    dlg.exec();
    app.exec();
}


// testMain
int main(int argc, char**argv)
{
    RuntimeVariablesType stack[2];
    RuntimeVariablesType heap[2];

    MemoryType mem[2];

    //Stack Setup
    stack[0].name = "StackVar0";
    stack[1].name = "StackVar1";

    stack[0].mangled_name = "StackMangledName0";
    stack[1].mangled_name = "StackMangledName1";

    stack[0].type = "r";
    stack[1].type = "v";

    stack[0].address=NULL;
    stack[1].address=&mem[0];

    stack[0].fileOpen="r";
    stack[1].fileOpen="r";

    stack[0].arrays=NULL;
    stack[1].arrays=NULL;

    //Heap Setup
    heap[0].name = "StackVar0";
    heap[1].name = "StackVar1";

    heap[0].mangled_name = "StackMangledName0";
    heap[1].mangled_name = "StackMangledName1";

    heap[0].fileOpen="r";
    heap[1].fileOpen="r";

    heap[0].type = "r";
    heap[1].type = "v";

    heap[0].address=NULL;
    heap[1].address=&mem[0];

    heap[0].arrays=NULL;
    heap[1].arrays=NULL;


    //Mem Loc Setup
    mem[0].address = 123;
    mem[1].address = 2343;

    mem[0].variables = NULL;
    mem[1].variables = NULL;

    showDebugDialog(stack,2,heap,2,mem,2);
}