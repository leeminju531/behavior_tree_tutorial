// port를 위해 xml과 데이터 관련 통신
// xml에 사용되는 모든 데이터가 string 타입이기에 std::string 타입이 가장 쉬운 방법이였음.
// 물론 다른 형식도 BehaviorTree.cpp이 자동 변환을 지원해준다.
#ifndef SIMPLE_BT_NODES_H
#define SIMPLE_BT_NODES_H
#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>

using namespace BT;

// 사용자 정의 데이터를 parsing 해보기.
struct Position2D
{
    double x;
    double y;
};

// To parse a string into a Position2D
// BT::convertFromString<Position2D>(StringView) 형식을 사용해 주어야 한다.

// Template specialization to converts a string to Position2D.
namespace BT
{
    template <> inline Position2D convertFromString(StringView str)
    {
        printf("Converting string: \"%s\"\n", str.data() ); // In production code, you would (obviously) remove the printf statement.

        // real numbers separated by semicolons
        auto parts = splitString(str,';'); 
        if(parts.size() != 2)
        {
            throw RuntimeError("invalid input");
        }
        else
        {
            Position2D output;
            output.x = convertFromString<double>(parts[0]); // Once we split the input into single numbers, we can reuse the specialization
            output.y = convertFromString<double>(parts[1]);
            return output;
        }
    }
}



// we can create two custom Actions, 
// one will write into a port and the other will read from a port.
class CalculateGoal : public SyncActionNode
{
public:
    // node에 ports가 존재하면, 해당 클래스 형식으로 상속
    CalculateGoal(const std::string& name,const NodeConfiguration& config)
    : SyncActionNode(name,config)
    {}

    // port를 위해 해당 static 함수는 꼭 정의
    static PortsList providedPorts()
    {
        return {OutputPort<Position2D>("goal")};
    }
    NodeStatus tick() override
    {
        Position2D mygoal = {1.1,2.3};
        setOutput<Position2D>("goal",mygoal);
        return NodeStatus::SUCCESS;
    }
};

class PrintTarget : public SyncActionNode
{
public:
    PrintTarget(const std::string& name,const NodeConfiguration& config)
    :  SyncActionNode(name,config)
    {}


    // 아마 해당 static 함수가 getInput<> - 읽기 , setInput<> - 쓰기 를 사용하기 전에 
    // 선행되도록 짜여진 듯.
    static PortsList providedPorts()
    {
        const char* description = "Simply print the goal opn console ...";
        return {InputPort<Position2D>("target",description)}; // description 인자로 부가 설명도 가능하긴함
    }


    NodeStatus tick() override
    {
        auto res = getInput<Position2D>("target");
        if(!res)
        {
            throw RuntimeError("error reading port [target]:",res.error());
            // fale 반환해도 괜춘.
        }
        Position2D target = res.value();
        printf("Target positions: [ %.1f, %.1f ]\n", target.x, target.y );
        return NodeStatus::SUCCESS;
    }
};

#endif