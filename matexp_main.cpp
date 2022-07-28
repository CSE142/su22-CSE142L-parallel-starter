#include <cstdlib>
#include "archlab.hpp"
#include <unistd.h>
#include"pin_tags.h"
#include<algorithm>
#include<cstdint>
#include"function_map.hpp"
#include <dlfcn.h>
#include"omp.h"
#include"params.hpp"
#include"tensor_t.hpp"
#include<thread>

#define ELEMENT_TYPE double

uint64_t  canary_size = 500000000;
uint64_t histogram[256];


extern "C"
void  __attribute__ ((optimize("O3"))) _canary(tensor_t<ELEMENT_TYPE> & dst, const tensor_t<ELEMENT_TYPE> & A, uint32_t power,
					       int64_t p1=0,
					       int64_t p2=0,
					       int64_t p3=0,
					       int64_t p4=0,
					       int64_t p5=0) {
	uint64_t arg1 = 1024;
	uint64_t * data = new uint64_t[canary_size];
	uint64_t histogram[256];
	pristine_machine();
	//set_cpu_clock_frequency(clock_rate);
	//theDataCollector->disable_prefetcher();
	ArchLabTimer timer; // create it.
	timer.attr("function", "_canary");
	timer.attr("power", 0);
	timer.attr("size", 0);
	timer.go();

			    
	for(int i =0; i < 256;i++) {
		histogram[i] = 0;
	}

#pragma omp parallel for
	for(uint64_t ii = 0; ii < canary_size; ii+=arg1) {
		uint64_t my_histogram[256];
		for(int i =0; i < 256;i++) {
			my_histogram[i] = 0;
		}
		for(uint64_t i = ii; i < canary_size && i < ii + arg1; i++) {
			
			for(int k = 0; k < 64; k+=8) {
				uint8_t b = (data[i] >> k)& 0xff;
				my_histogram[b]++;
			}
		}

#pragma omp critical 
		for(int i =0; i < 256;i++) {
			histogram[i] += my_histogram[i];
		}
	}


	delete data;
}
FUNCTION(matexp, _canary);

uint array_size;

typedef void(*matexp_impl)(tensor_t<ELEMENT_TYPE> & , const tensor_t<ELEMENT_TYPE> &, uint32_t power,
			   uint64_t,
			   int64_t,
			   int64_t,
			   int64_t,
			   int64_t,
			   int64_t);

int main(int argc, char *argv[])
{

	
	std::vector<int> mhz_s;
	std::vector<int> default_mhz;
	archlab_add_option<uint64_t>("canary",
				     canary_size,
				     0,
				     "mininum random value in tensors");
	

	load_frequencies();
	default_mhz.push_back(cpu_frequencies_array[1]);
	std::stringstream clocks;
	for(int i =0; cpu_frequencies_array[i] != 0; i++) {
		clocks << cpu_frequencies_array[i] << " ";
	}
	std::stringstream fastest;
	fastest << cpu_frequencies_array[0];

	
	archlab_add_multi_option<std::vector<int> >("MHz",
						    mhz_s,
						    default_mhz,
						    fastest.str(),
						    "Which clock rate to run.  Possibilities on this machine are: " + clocks.str());

	std::vector<std::string> functions;
	std::vector<std::string> default_functions;
	
	default_functions.push_back("ALL");
	archlab_add_multi_option<std::vector<std::string>>("function",
							   functions,
							   default_functions,
							   "ALL",
							   "Which functions to run.");

	std::vector<unsigned long int> powers;
	std::vector<unsigned long int> default_powers;
	default_powers.push_back(16);
	archlab_add_multi_option<std::vector<unsigned long int> >("power",
								  powers,
								  default_powers,
								  "16",
								  "Power.  Pass multiple values to run with multiple sizes.");
	std::vector<unsigned long int> threads;
	std::vector<unsigned long int> default_threads;
	default_threads.push_back(1);
	archlab_add_multi_option<std::vector<unsigned long int> >("thread",
					      threads,
					      default_threads,
					      "1",
					      "Thread.  Pass multiple values to run with multiple arg3s.");
		
	std::vector<unsigned long int> sizes;
	std::vector<unsigned long int> default_sizes;
	default_sizes.push_back(16);
	archlab_add_multi_option<std::vector<unsigned long int> >("size",
								  sizes,
								  default_sizes,
								  "16",
								  "Size.  Pass multiple values to run with multiple sizes.");
	
	PARAM(1);
	PARAM(2);
	PARAM(3);
	PARAM(4);
	PARAM(5);


	float minv = -1.0;
	archlab_add_option<float>("min",
				  minv,
				  -1.0,
				  "mininum random value in tensors");

	float maxv = 1.0;
	archlab_add_option<float>("max",
				  maxv,
				  1.0,
				  "maxinum random value in tensors");
	

	bool tag_functions;
	archlab_add_option<bool >("tag-functions",
				  tag_functions,
				  true,
				  "true",
				  "Add tags for each function invoked");

	std::vector<uint64_t> seeds;
	std::vector<uint64_t> default_seeds;
	default_seeds.push_back(0xDEADBEEF);
	archlab_add_multi_option<std::vector<uint64_t> >("seed",
							 seeds,
							 default_seeds,
							 "0xDEADBEEF",		
							 "random seeds to run");
	archlab_parse_cmd_line(&argc, argv);

	theDataCollector->disable_prefetcher();

	if (std::find(functions.begin(), functions.end(), "ALL") != functions.end()) {
		functions.clear();
		for(auto & f : function_map::get()) {
			functions.push_back(f.first);
		}
	}

	if (canary_size!= 0) {
		functions.insert(functions.begin(),"_canary");
	}
	
	for(auto & function : functions) {
		auto t= function_map::get().find(function);
		if (t == function_map::get().end()) {
			std::cerr << "Unknown function: " << function <<"\n";
			exit(1);
		}
		std::cerr << "Gonna run " << function << "\n";
	}
	

	for(auto &mhz: mhz_s) {
		set_cpu_clock_frequency(mhz);
		for(auto & thread: threads ) {
			omp_set_num_threads(thread);
			for(auto & seed: seeds ) {
				for(auto & size:sizes) {
					for(auto & power: powers ) {
						PARAM_LOOP(1) {
							PARAM_LOOP(2) {
								PARAM_LOOP(3) {
									PARAM_LOOP(4) {
										PARAM_LOOP(5) {
											for(auto & function : functions) {
												tensor_t<ELEMENT_TYPE> dst(size,size);
												tensor_t<ELEMENT_TYPE> A(size,size);
												randomize(A, seed, minv, maxv);
										
												START_TRACE();
												std::cerr << "Running " << function << "\n";
												function_spec_t f_spec = function_map::get()[function];
												auto fut = reinterpret_cast<matexp_impl>(f_spec.second);
												std::cerr << ".";
												pristine_machine();					
												PARAM_MARK(1);
												PARAM_MARK(2);
												PARAM_MARK(3);
												PARAM_MARK(4);
												PARAM_MARK(5);
												{								
													theDataCollector->disable_prefetcher();
													theDataCollector->register_tag("function",function);
													theDataCollector->register_tag("seed",seed);
													theDataCollector->register_tag("cmdlineMHz", mhz);
													theDataCollector->register_tag("thread", thread);
											
													if (tag_functions) DUMP_START_ALL(function.c_str(), false);

													fut(dst, A,
													    power,
													    seed,
													    PARAM_PASS(1),
													    PARAM_PASS(2),
													    PARAM_PASS(3),
													    PARAM_PASS(4),
													    PARAM_PASS(5)
														);
											
													if (tag_functions )DUMP_STOP(function.c_str());
												}								
											}
											std::cerr << "\n";
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	archlab_write_stats();
	return 0;
}
