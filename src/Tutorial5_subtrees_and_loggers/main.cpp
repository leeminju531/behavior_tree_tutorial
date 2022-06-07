#include "behaviortree_cpp_v3/loggers/bt_cout_logger.h"
#include "behaviortree_cpp_v3/loggers/bt_minitrace_logger.h"
#include "behaviortree_cpp_v3/loggers/bt_file_logger.h"
#include "behaviortree_cpp_v3/bt_factory.h"

#include "crossdoor_node.h"

//#ifdef ZMQ_FOUND
#include "behaviortree_cpp_v3/loggers/bt_zmq_publisher.h"
//#endif

//A Logger is a mechanism to display, record and/or publish any state change in the tree.
// subtree를 재사용하기위해 build할 필요가 없다.
int main(int argc, char** argv)
{
    
    using namespace BT;
    BehaviorTreeFactory factory;
    // register all the actions into the factory
    CrossDoor::RegisterNodes(factory);

    // Important: when the object tree goes out of scope, all the TreeNodes are destroyed
    auto tree = factory.createTreeFromFile("./my_tree.xml");

    // This logger prints state changes on console
    StdCoutLogger logger_cout(tree);

    // This logger saves state changes on file
    FileLogger logger_file(tree,"bt_trace.fbl");

    // This logger stores the execution time of each node
    MinitraceLogger logger_minitrace(tree, "bt_trace.json");

// #ifdef ZMQ_FOUND
    // This logger publish status changes using ZeroMQ. Used by Groot
    PublisherZMQ publisher_zmq(tree);
// #endif
    // Debug function to print the hierarchy of the tree. Prints to std::cout by default.    
    printTreeRecursively(tree.rootNode());
    
    using std::chrono::milliseconds;
    do
    {   
        NodeStatus status = NodeStatus::RUNNING;
        while(status == NodeStatus::RUNNING)
        {
            status = tree.tickRoot();
            // IMPORTANT: you must always add some sleep if you call tickRoot()
            // in a loop, to avoid using 100% of your CPU (busy loop).
            tree.sleep(milliseconds(10));
        }

        std::this_thread::sleep_for( milliseconds(1000) );
        
    } while (1);
    

    return 0;
}