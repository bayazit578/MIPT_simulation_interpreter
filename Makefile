SRC          = src/main.cpp src/cpu.cpp src/interpreter.cpp src/elf_ext.cpp
INCLUDE_DIRS = include
OUT_DIR      = build
OBJ          = $(patsubst src/%.cpp,$(OUT_DIR)/%.o,$(SRC))
EXEC         = interpreter

# bindings
BINDINGS = src/bindings.so

# tests
TEST_SRC  = $(filter-out src/main.cpp,$(SRC)) \
            $(wildcard tests/*.cpp)
TEST_OBJ  = $(patsubst %.cpp,$(OUT_DIR)/gtests/%.o,$(TEST_SRC))
TEST_EXEC = interpreter_tests
ASM_SRC   = $(wildcard tests/asm/*.asm)
ASM_BIN   = $(patsubst tests/asm/%.asm,tests/bin/%.test,$(ASM_SRC))
FIB_BIN   = tests/bin/fibonacci.bin
FIB_TEST_BIN = tests/bin/fibonacci.test

# performance
CRC_EXEC         = crc32
CRC_ASM_BIN      = tests/bin/crc32.test
SORT_EXEC        = sort
SORT_ASM_BIN     = tests/bin/sort.test
BRANCHES_EXEC    = branches
BRANCHES_ASM_BIN = tests/bin/branches.test

# flags
CXX            = g++
RICE_INCLUDE   = $(shell ruby -e 'print Gem::Specification.find_by_name("rice").full_gem_path, "/include"')
RUBY_INCLUDE   = $(shell ruby -rrbconfig -e 'print RbConfig::CONFIG.fetch("rubyhdrdir")')
RUBY_ARCH_INCLUDE = $(shell ruby -rrbconfig -e 'print RbConfig::CONFIG.fetch("rubyarchhdrdir")')
CXXFLAGS_GP    = -std=c++20 -I$(RICE_INCLUDE) -I$(RUBY_INCLUDE) -I$(RUBY_ARCH_INCLUDE) -IELFIO
CXXFLAGS_RUN   = -DNDEBUG
CXXFLAGS_DEBUG = -O2 -g
CXXFLAGS_ASAN  = -fcheck-new -fsized-deallocation -fstack-protector \
                 -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer \
                 -pie -fPIE \
                 -fsanitize=address,undefined,leak,float-divide-by-zero,float-cast-overflow
CXXFLAGS_TESTS = -lgtest_main -lgtest


.PHONY: all run clean tests crc32 sort


all: $(OUT_DIR)/$(EXEC) $(BINDINGS)

$(OUT_DIR)/$(EXEC): $(OBJ)
	@$(CXX) $(OBJ) -o $@

$(OUT_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS_GP) -I$(INCLUDE_DIRS) $(CXXFLAGS_RUN) -c $< -o $@


bindings: $(BINDINGS)

$(BINDINGS): src/ruby_bindings.cpp src/elf_ext.cpp include/elf_ext.hpp
	@$(CXX) $(CXXFLAGS_GP) -Iinclude -fPIC -shared src/ruby_bindings.cpp\
		src/elf_ext.cpp -o $@ -lruby


tests: $(ASM_BIN) $(FIB_BIN) $(FIB_TEST_BIN) $(OUT_DIR)/gtests/$(TEST_EXEC)
	./$(OUT_DIR)/gtests/$(TEST_EXEC)

$(OUT_DIR)/gtests/$(TEST_EXEC): $(TEST_OBJ)
	@$(CXX) $(TEST_OBJ) -o $@ $(CXXFLAGS_TESTS)

$(OUT_DIR)/gtests/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS_GP) -I$(INCLUDE_DIRS) $(CXXFLAGS_DEBUG) -c $< -o $@

tests/bin/%.test: tests/asm/%.asm asm.rb
	@mkdir -p $(@D)
	@ruby asm.rb $< $@

$(FIB_BIN) $(FIB_TEST_BIN): tests/fibonacci.asm asm.rb
	@mkdir -p $(@D)
	@ruby asm.rb $< $@


crc32: $(CRC_ASM_BIN) $(OUT_DIR)/crc32/$(CRC_EXEC)
	hyperfine -w 10 -r 10000 './$(OUT_DIR)/crc32/$(CRC_EXEC) -m b -i $(CRC_ASM_BIN)' \
											     './$(OUT_DIR)/crc32/$(CRC_EXEC) -m t -i $(CRC_ASM_BIN)'

$(OUT_DIR)/crc32/$(CRC_EXEC): $(OBJ)
	@mkdir -p $(@D)
	@$(CXX) $(OBJ) -o $@


sort: $(SORT_ASM_BIN) $(OUT_DIR)/sort/$(SORT_EXEC)
	hyperfine -w 10 -r 100 './$(OUT_DIR)/sort/$(SORT_EXEC) -m b -i $(SORT_ASM_BIN)' \
											   './$(OUT_DIR)/sort/$(SORT_EXEC) -m t -i $(SORT_ASM_BIN)'

$(OUT_DIR)/sort/$(SORT_EXEC): $(OBJ)
	@mkdir -p $(@D)
	@$(CXX) $(OBJ) -o $@


branches: $(BRANCHES_ASM_BIN) $(OUT_DIR)/branches/$(BRANCHES_EXEC)
	hyperfine -w 5 -r 10 './$(OUT_DIR)/branches/$(BRANCHES_EXEC) -m b -i $(BRANCHES_ASM_BIN)' \
											 './$(OUT_DIR)/branches/$(BRANCHES_EXEC) -m t -i $(BRANCHES_ASM_BIN)'

$(OUT_DIR)/branches/$(BRANCHES_EXEC): $(OBJ)
	@mkdir -p $(@D)
	@$(CXX) $(OBJ) -o $@


run: all
	@./$(OUT_DIR)/$(EXEC)

clean:
	@rm -rf $(OUT_DIR)
	@rm -f $(ASM_BIN) $(FIB_TEST_BIN)
