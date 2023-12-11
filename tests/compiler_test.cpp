#include <gtest/gtest.h>
#include "module.h"
#include <algorithm>

TEST(BasicBlockTest, MustHaveNameBasic) {
    // Expect equality.
    EXPECT_EQ(BasicBlock("blk0").get_name(), "blk0");
}

// By design a function always has a name and an entry basic block.
TEST(FunctionTest, MustHaveNameAndEntryBasicBlock) {
    auto fn1 = Function("fn1", BasicBlock("blk0"));
    EXPECT_EQ(fn1.get_basic_blocks().size(), 0);

    EXPECT_EQ(fn1.get_name(), "fn1");
    EXPECT_EQ(fn1.get_entry_block().get_name(), "blk0");
}

TEST(FunctionTest, InsertBasicBlock) {
    auto fn1 = Function("fn1", BasicBlock("blk0"));
    EXPECT_EQ(fn1.get_basic_blocks().size(), 0);

    fn1.insert_basic_block("blk1");
    EXPECT_EQ(fn1.get_basic_blocks().size(), 1);
    EXPECT_EQ(fn1.get_basic_blocks()[0].get_name(), "blk1");

    fn1.insert_basic_block("blk2");
    EXPECT_EQ(fn1.get_basic_blocks().size(), 2);
}

TEST(FunctionTest, RemoveBasicBlock) {}

TEST(ModuleTest, MustHaveNameBasic) {
    auto mod1 = Module("mod1");

    EXPECT_EQ(mod1.get_name(), "mod1");
}

TEST(ModuleTest, InsertBasicBlock) {
    auto mod1 = Module("mod1");

    EXPECT_EQ(mod1.get_functions().size(), 0);

    mod1.insert_basic_block("fn1", "blk0");
    EXPECT_EQ(mod1.get_functions().size(), 1);
    EXPECT_EQ(mod1.get_functions()[0].get_name(), "fn1");

    mod1.insert_basic_block("fn2", "blk0");
    EXPECT_EQ(mod1.get_functions().size(), 2);
    EXPECT_EQ(mod1.get_functions()[1].get_name(), "fn2");
}

TEST(SuccessorTest, InsertSuccessor) {
    auto graph = SuccGraph();
    graph.add_successor("blk2", "blk3", "tag3");
    graph.add_successor("blk1", "blk3", "tag1");
    graph.add_successor("blk2", "blk3", "tag2");
    graph.add_successor("blk1", "blk4", "tag1");

    EXPECT_EQ(graph.get_succ_graph().size(), 2);
    auto succ_graph = graph.get_succ_graph();
    auto &links = succ_graph["blk1"];
    EXPECT_EQ(links.size(), 1);

    graph.remove_successor("blk1", "blk4");
}