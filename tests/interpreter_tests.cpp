#include <gtest/gtest.h>

#include "interpreter.hpp"

class InterpreterTest : public testing::TestWithParam<std::string> {
protected:
  Interpreter interpreter{};
};

TEST_P(InterpreterTest, TestInstruction) {
  std::string f_name = GetParam();

  ASSERT_EQ(interpreter.load_program(f_name), EXIT_SUCCESS)
    << "Failed to load prog from file " << f_name;

  while (!interpreter.terminated()) {
    interpreter.iter();
  }

  EXPECT_EQ(interpreter.exit_code(), EXIT_SUCCESS);
}

INSTANTIATE_TEST_SUITE_P(ClzTest  , InterpreterTest, testing::Values("tests/bin/clz.test" ));
INSTANTIATE_TEST_SUITE_P(LiTest   , InterpreterTest, testing::Values("tests/bin/li.test"  ));
INSTANTIATE_TEST_SUITE_P(SyscTest , InterpreterTest, testing::Values("tests/bin/sysc.test"));
INSTANTIATE_TEST_SUITE_P(StTest   , InterpreterTest, testing::Values("tests/bin/st.test"  ));
INSTANTIATE_TEST_SUITE_P(StpTest  , InterpreterTest, testing::Values("tests/bin/stp.test" ));
INSTANTIATE_TEST_SUITE_P(BneTest  , InterpreterTest, testing::Values("tests/bin/bne.test" ));
INSTANTIATE_TEST_SUITE_P(BeqTest  , InterpreterTest, testing::Values("tests/bin/beq.test" ));
INSTANTIATE_TEST_SUITE_P(SelcTest , InterpreterTest, testing::Values("tests/bin/selc.test"));
INSTANTIATE_TEST_SUITE_P(StiTest  , InterpreterTest, testing::Values("tests/bin/sti.test" ));
INSTANTIATE_TEST_SUITE_P(JTest    , InterpreterTest, testing::Values("tests/bin/j.test"   ));
INSTANTIATE_TEST_SUITE_P(SsatTest , InterpreterTest, testing::Values("tests/bin/ssat.test"));
INSTANTIATE_TEST_SUITE_P(LdTest   , InterpreterTest, testing::Values("tests/bin/ld.test"  ));
INSTANTIATE_TEST_SUITE_P(SbitTest , InterpreterTest, testing::Values("tests/bin/sbit.test"));
INSTANTIATE_TEST_SUITE_P(AddTest  , InterpreterTest, testing::Values("tests/bin/add.test" ));
INSTANTIATE_TEST_SUITE_P(AddiTest , InterpreterTest, testing::Values("tests/bin/addi.test"));

INSTANTIATE_TEST_SUITE_P(Fibonacci, InterpreterTest, testing::Values("tests/bin/fibonacci.bin"));
