void undefined_behaviour_with_double_checked_locking()
{
	if(!resource_ptr)
	{
		std::lock_guard<std::mutex> lk(resource_mutex)
		if(!resource_ptr)
		{
			resource_ptr.reset(new some_resource);
		}
	}
	resource_ptr->do_something();
}


// a better way such that initialization is done only once
std::once_flag resource_flag;
void init_resource()
{
	resource_ptr.reset(new some_resource);
}
void foo()
{
	std::call_once(resource_flag,init_resource);
	resource_ptr->do_something();
}