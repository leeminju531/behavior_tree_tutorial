#include <behaviortree_cpp_v3/bt_factory.h>
#include "dummy_nodes.h" // file that contains the custom nodes definitions

int main()
{
    using namespace DummyNodes; // 등록되는 이름은 XML에서 사용되는 이름과 같아야 한다.

    BehaviorTreeFactory factory; // 내 custom 노드를 해당 obj에 등록시켜줘야 한다.
    factory.registerNodeType<SaySomething>("SaySomething");
    factory.registerNodeType<ThinkWhatToSay>("ThinkWhatToSay");



    // SimpleActionNodes can not define their own method providedPorts().
    // We should pass a PortsList explicitly if we want the Action to 
    // be able to use getInput() or setOutput();
    // ->class 객체가 아닌 function을 등록했기에 필요한 인자를 넘겨준다.
    PortsList say_something_ports = {InputPort<std::string>("message")};
    factory.registerSimpleAction("SaySomething2",SaySomethingSimple,
                                                say_something_ports);
    //tree는 run time에 시작할 때 생성된다.
    auto tree = factory.createTreeFromFile("./my_tree.xml");
    // tree를 실행할려면 tick (호출) 해줘야 한다.
    tree.tickRoot();


    return 0;
}