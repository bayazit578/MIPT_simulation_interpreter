SRC          = src/main.cpp src/fetcher.cpp src/decoder.cpp src/executor.cpp
INCLUDE_DIRS = include
OUT_DIR      = build
EXEC         = interpreter

# flags
CXXFLAGS       = 
CXXFLAGS_RUN   = -DNDEBUG
CXXFLAGS_DEBUG = -O2 -g
CXXFLAGS_ASAN  = -fcheck-new -fsized-deallocation -fstack-protector \
								 -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer \
								 -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,\
								 float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,\
								 nonnull-attribute,null,object-size,return,returns-nonnull-attribute,\
								 shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

.PHONY: run clean
