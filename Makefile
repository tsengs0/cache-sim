SRC_DIR := src
POLICY_DIR := policies
SRC_FILES := $(wildcard $(SRC_DIR)/*pp)
POLICY_FILES := $(wildcard $(POLICY_DIR)/*pp)

all: cache.exe

cache.exe: $(SRC_FILES) $(POLICY_FILES) # TODO: need to recompile when G++FLAGS is changed
	g++ -std=c++11 $(SRC_FILES) $(POLICY_FILES) $(G++FLAGS) -o $@

run:
	./run.sh -t pinatrace.out.gz -c params.cfg > eviction.log

run_456.hmmer:
	./run.sh -t /home/s1820419/DRAM/Cache/cache-sim_MAIN/result/456.hmmer/mem_addr_val.out.gz -c params.cfg > eviction.log 
	mv eviction.log L2_eviction_trace.csv /home/s1820419/DRAM/Cache/cache-sim_MAIN/result/456.hmmer/

run_462.libquantum:
	./run.sh -t /home/s1820419/DRAM/Cache/cache-sim_MAIN/result/462.libquantum/mem_addr_val.out.gz -c params.cfg > eviction.log 
	mv eviction.log L2_eviction_trace.csv /home/s1820419/DRAM/Cache/cache-sim_MAIN/result/462.libquantum/

clean:
	rm cache.exe
