#include <iostream>
#include <thread>

const long int mem_alligment = 32;
long int allocations = 64;
long int allocation_size = 1 * 1024 * 1024;
const long int threads_count = 32;
const long int sleep_seconds = 2;

static void* alocu(size_t size, bool use_aligned_malloc)
{
	if (use_aligned_malloc)
		return _aligned_malloc(size, mem_alligment);
	else
		return malloc(size);
}

static void alofree(void * ptr, bool use_aligned_malloc)
{
	if (use_aligned_malloc)
		return _aligned_free(ptr);
	else
		return free(ptr);
}

int allocator_thread(int thread)
{
	const bool thread_use_aligned_malloc = true;

	char ** ptrs = (char**)malloc(allocations*sizeof(void*));
	
	std::this_thread::sleep_for(std::chrono::seconds(1));

	for (int i = 0; i < allocations; i++)
	{
		char* ptr = (char*)alocu(allocation_size, thread_use_aligned_malloc);

		if (ptr)
		{
			memset(ptr, 0xFF, allocation_size);
			ptrs[i] = ptr;
		} else {
			ptrs[i] = nullptr;
			std::cout << "Failed to allocate in thread " << thread <<", allocation " << i << std::endl;
		}
	}
	
	std::cout << "Allocated thread " << thread << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(sleep_seconds)); 

	for (int i = 0; i < allocations; i++)
	{
		if (ptrs[i])
		{
			alofree(ptrs[i], thread_use_aligned_malloc);
			ptrs[i] = nullptr;
		}
	}
	free(ptrs);
	std::cout << "Finished thread " << thread << std::endl;
	return 0;
}

int main()
{
	std::thread al_thrds[threads_count];

	std::cout << "Application will create N threads " << std::endl;
	std::cout << "Each thread will allocate memory K times R bytes " << std::endl;
	std::cout << "So total allocated memory will be N*K*R " << std::endl;
	std::cout << "And to test " << std::endl;
	std::cout << "4Gb = 32 threads x 2048 allocations x 65536 bytes " << std::endl;
	std::cout << "16Gb = 32 threads x 8192 allocations x 65536 bytes " << std::endl;
	std::cout << "tiny bytes 4Gb = 32 threads x 2097152 allocations x 64 bytes " << std::endl << std::endl;

	std::cout << "Enter size of single allocation " << std::endl;
	std::cin >> allocation_size;
	if (allocation_size <= 0)
	{
		allocation_size = 1024;
	}

	std::cout << "Enter allocations count " << std::endl;
	std::cin >> allocations;
	if (allocations <= 0)
	{
		allocations = 1024;
	}

	std::cout << std::endl << std::endl << "Single allocation " << allocation_size << std::endl;
	std::cout << "Threads count " << threads_count << std::endl;
	std::cout << "Allocations per thread " << allocations << std::endl;
	std::cout << "Also allocated memory can be aligned at " << mem_alligment << " and it can increase memory consumption" << std::endl;
	
	for (int i = 0; i < threads_count; i++) 
	{
		al_thrds[i] = std::thread( allocator_thread, i);
	}
	
	std::cout << "Launched all " << threads_count  << std::endl;

	for (int i = 0; i < threads_count; i++) 
	{
		al_thrds[i].join();
	}

	std::cout << "Exit " << std::endl;
	return 0;
}
