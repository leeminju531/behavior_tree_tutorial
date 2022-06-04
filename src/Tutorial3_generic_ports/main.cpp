#include <behaviortree_cpp_v3/bt_factory.h>
#include "dummy_node.h"// custom node definition

static const char* xml_text = R"(

 <root main_tree_to_execute = "MainTree" >
     <BehaviorTree ID="MainTree">
        <SequenceStar name="root">
            <CalculateGoal   goal="{GoalPosition}" />
            <PrintTarget     target="{GoalPosition}" />
            <SetBlackboard   output_key="OtherGoal" value="-1;3" />
            <PrintTarget     target="{OtherGoal}" />
        </SequenceStar>
     </BehaviorTree>
 </root>
 )";


/*SetBlackboard 노드는 behavior tree에 정의된 노드

    이전 튜토리얼에도 message="hello" 로 xml에서 써줄 수 있었는데,
    이 것은 'message'를 key로 이용해야만 했음.

    위 노드를 사용하면 key - value쌍 각각을 이름으로 지어줄 수 있는듯.
*/
int main(){
    using namespace BT;
    BehaviorTreeFactory factory;
    factory.registerNodeType<CalculateGoal>("CalculateGoal");
    factory.registerNodeType<PrintTarget>("PrintTarget");
    auto tree = factory.createTreeFromText(xml_text);
    tree.tickRoot();

    return 0;
}