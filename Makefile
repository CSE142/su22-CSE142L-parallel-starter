SHELL=/bin/bash
OPENMP=yes
.SUFFIXES:
default:

.PHONY: create-labs
create-labs:
	cse142 lab delete -f parallel-bench
	cse142 lab delete -f parallel
	cse142 lab create --name "Lab 5: Parallelism (Benchmark)" --short-name "parallel-bench" --docker-image stevenjswanson/cse142l-runner:latest --execution-time-limit 0:05:00 --total-time-limit 1:00:00 --due-date 2021-12-07T23:59:59 --starter-repo https://github.com/CSE142/fa21-CSE142L-parallel-starter.git --starter-branch main
	cse142 lab create --name "Lab 5: Parallelism" --short-name "parallel" --docker-image stevenjswanson/cse142l-runner:v47 --execution-time-limit 0:05:00 --total-time-limit 1:00:00 --due-date 2021-12-07T23:59:59

STUDENT_EDITABLE_FILES=matexp_solution.hpp config.make
PRIVATE_FILES=Lab.key.ipynb admin .git solution bad-solution

OPTIMIZE+=-march=x86-64
COMPILER=gcc-8
include $(ARCHLAB_ROOT)/cse141.make

.PHONY: autograde
autograde: matexp.exe regressions.json bench.csv
bench.csv:
	./matexp.exe --MHz 3500 --stats bench0.csv --thread 12 --canary 500000000 --stat-set ./L1.cfg --function bench_solution
	./matexp.exe --MHz 3500 --stats bench1.csv --thread 12 --canary 500000000 --stat-set ./L1.cfg --function bench_solution
	./matexp.exe --MHz 3500 --stats bench2.csv --thread 12 --canary 500000000 --stat-set ./L1.cfg --function bench_solution
	./matexp.exe --MHz 3500 --stats bench3.csv --thread 12 --canary 500000000 --stat-set ./L1.cfg --function bench_solution
#	./matexp.exe --MHz 3500 --stats bench4.csv --thread 12 --canary 500000000 --stat-set ./L1.cfg --function bench_solution
#	./matexp.exe --MHz 3500 --stats bench5.csv --thread 12 --canary 500000000 --stat-set ./L1.cfg --function bench_solution
	./summarize.py --out bench.csv bench0.csv bench1.csv bench2.csv bench3.csv #bench4.csv #bench5.csv

#run_tests.exe: $(BUILD)ChunkAlloc.o
regressions.json: run_tests.exe
	./run_tests.exe --thread 12 --gtest_output=json:$@
test: regressions.json

matexp.exe:  $(BUILD)matexp_main.o  $(BUILD)matexp.o
matexp.exe: EXTRA_LDFLAGS=
$(BUILD)matexp.o : OPTIMIZE=$(MATEXP_OPTIMIZE)
$(BUILD)matexp.s : OPTIMIZE=$(MATEXP_OPTIMIZE)
$(BUILD)matexp_main.o : OPTIMIZE=$(MATEXP_OPTIMIZE)

$(BUILD)run_tests.o : OPTIMIZE=-O3
fiddle.exe:  $(BUILD)fiddle.o $(FIDDLE_OBJS)
fiddle.exe: EXTRA_LDFLAGS=-pg
$(BUILD)fiddle.o : OPTIMIZE=-O3 -pg


-include $(DJR_JOB_ROOT)/$(LAB_SUBMISSION_DIR)/config.make

