#include <gtest/gtest.h>

#include "interpreter.hpp"

class InterpreterTest : public testing::TestWithParam<std::string> {
protected:
  Interpreter interpreter{};
};

TEST_P(InterpreterTest, TestInstruction) {
  std::string f_name = GetParam();

  EXPECT_TRUE(interpreter.load_program(f_name)) 
    << "Failed to load prog from file" << f_name;

  while (!interpreter.terminated() != true) {
    interpreter.iter();
  }

  EXPECT_EQ(interpreter.exit_code(), EXIT_SUCCESS);
}

INSTANTIATE_TEST_SUITE_P(ClzTest , InterpreterTest, testing::Values("data_bin/clz.test" ));
INSTANTIATE_TEST_SUITE_P(LiTest  , InterpreterTest, testing::Values("data_bin/li.test"  ));
INSTANTIATE_TEST_SUITE_P(SyscTest, InterpreterTest, testing::Values("data_bin/sysc.test"));
INSTANTIATE_TEST_SUITE_P(StTest  , InterpreterTest, testing::Values("data_bin/st.test"  ));
INSTANTIATE_TEST_SUITE_P(StpTest , InterpreterTest, testing::Values("data_bin/stp.test" ));
INSTANTIATE_TEST_SUITE_P(BneTest , InterpreterTest, testing::Values("data_bin/bne.test" ));
INSTANTIATE_TEST_SUITE_P(BeqTest , InterpreterTest, testing::Values("data_bin/beq.test" ));
INSTANTIATE_TEST_SUITE_P(SelcTest, InterpreterTest, testing::Values("data_bin/selc.test"));
INSTANTIATE_TEST_SUITE_P(StiTest , InterpreterTest, testing::Values("data_bin/sti.test" ));
INSTANTIATE_TEST_SUITE_P(JTest   , InterpreterTest, testing::Values("data_bin/j.test"   ));
INSTANTIATE_TEST_SUITE_P(SsatTest, InterpreterTest, testing::Values("data_bin/ssat.test"));
INSTANTIATE_TEST_SUITE_P(LdTest  , InterpreterTest, testing::Values("data_bin/ld.test"  ));
INSTANTIATE_TEST_SUITE_P(SbitTest, InterpreterTest, testing::Values("data_bin/sbit.test"));
INSTANTIATE_TEST_SUITE_P(AddTest , InterpreterTest, testing::Values("data_bin/add.test" ));
INSTANTIATE_TEST_SUITE_P(AddiTest, InterpreterTest, testing::Values("data_bin/addi.test"));
