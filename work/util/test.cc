
#include <omnetpp.h>

#include "Util.h"



using namespace omnetpp;

namespace util {



class Test : public cSimpleModule
{
    public:
        Test() : cSimpleModule(65536) {}
        virtual void activity();
};

Define_Module(Test);

void Test::activity()
{
Util *util = new Util();
delete util;


}

}; //namespace
