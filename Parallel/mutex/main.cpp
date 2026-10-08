#include <iostream>
#include <mutex>
#include <thread>

std::mutex cat_bowl_mutex;
int cat_food_count = 0;

void feed_cat(const char* cat_name, int food_count)
{
	for (int food_index = 0; food_index < food_count; ++food_index)
	{
		std::lock_guard<std::mutex> bowl_lock(cat_bowl_mutex);
		++cat_food_count;
		std::cout << cat_name << " ate one snack. Total: " << cat_food_count << '\n';
	}
}

int main()
{
	std::thread luna_thread(feed_cat, "Luna", 5);
	std::thread simba_thread(feed_cat, "Simba", 5);

	luna_thread.join();
	simba_thread.join();

	std::cout << "All snacks eaten: " << cat_food_count << '\n';
	return 0;
}
