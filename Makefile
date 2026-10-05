SRC          = src/main.cpp src/cpu.cpp src/interpreter.cpp
INCLUDE_DIRS = include
OUT_DIR      = build
OBJ          = $(patsubst src/%.cpp,$(OUT_DIR)/%.o,$(SRC))
EXEC         = interpreter

# tests
TEST_SRC  = $(filter-out src/main.cpp,$(SRC)) \
            $(wildcard tests/*.cpp)
TEST_OBJ  = $(patsubst %.cpp,$(OUT_DIR)/gtests/%.o,$(TEST_SRC))
TEST_EXEC = interpreter_tests
ASM_SRC   = $(wildcard tests/asm/*.asm)
ASM_BIN   = $(patsubst tests/asm/%.asm,tests/bin/%.test,$(ASM_SRC))

# flags
CXX            = g++
CXXFLAGS_GP    = -std=c++20
CXXFLAGS_RUN   = -DNDEBUG
CXXFLAGS_DEBUG = -O2 -g
CXXFLAGS_ASAN  = -fcheck-new -fsized-deallocation -fstack-protector \
                -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer \
                -pie -fPIE \
                -fsanitize=address,undefined,leak,float-divide-by-zero,float-cast-overflow
CXXFLAGS_TESTS = -lgtest_main -lgtest -pthread


.PHONY: all run clean tests


all: $(OUT_DIR)/$(EXEC)

$(OUT_DIR)/$(EXEC): $(OBJ)
	@$(CXX) $(OBJ) -o $@

$(OUT_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS_GP) -I$(INCLUDE_DIRS) $(CXXFLAGS_RUN) -c $< -o $@


tests: $(ASM_BIN) $(OUT_DIR)/gtests/$(TEST_EXEC)
	./$(OUT_DIR)/gtests/$(TEST_EXEC)

$(OUT_DIR)/gtests/$(TEST_EXEC): $(TEST_OBJ)
	@$(CXX) $(TEST_OBJ) -o $@ $(CXXFLAGS_TESTS)

$(OUT_DIR)/gtests/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS_GP) -I$(INCLUDE_DIRS) $(CXXFLAGS_DEBUG) -pthread -c $< -o $@

tests/bin/%.test: tests/asm/%.asm asm.rb
	@mkdir -p $(@D)
	@ruby asm.rb $< $@


run: all
	@./$(OUT_DIR)/$(EXEC)

clean:
	@rm -rf $(OUT_DIR)
	@rm -f $(ASM_BIN)
