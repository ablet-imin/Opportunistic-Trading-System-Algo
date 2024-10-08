#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <cassert>

#include "mergefiles/mergefiles.h"

void test_basic() {
    string tstemps1 = "2021-03-05 10:00:00.123, 46.14, 120, NYSE_ARCA, Ask";
    string tstemps2 = "2021-03-05 10:00:00.133, 228.5, 120, NYSE, TRADE";
    string tstemps3 = "2021-03-05 10:00:00.113, 228.5, 120, NYSE, TRADE";
    string name1 = "CSCO", name2 = "MSFT";

    // test tnode
    tnode node1 = create_node(tstemps1, name1);
    tnode node2 = create_node(tstemps2, name2);
    tnode node3 = create_node(tstemps3, name2);

    assert(node1.name == name1);
    assert(node1.time == "2021-03-05 10:00:00.123");
    assert(node1.line == tstemps1);
    cout<<"tnode creation passed!"<<endl;

    assert(tnode_less(node1, node2));
    assert(tnode_less(node1, node3) == false);
    cout<<"tnode comparison passed!"<<endl;


}

int main(int, char**)
{
  test_basic();
}