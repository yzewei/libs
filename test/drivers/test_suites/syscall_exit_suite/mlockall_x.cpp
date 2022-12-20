#include "../../event_class/event_class.h"

#ifdef __NR_mlockall

#include <sys/mman.h>

TEST(SyscallExit, mlockallX)
{
	auto evt_test = get_syscall_event_test(__NR_mlockall, EXIT_EVENT);

	evt_test->enable_capture();

	/*=============================== TRIGGER SYSCALL ===========================*/

	unsigned long mock_flags{0};
	assert_syscall_state(SYSCALL_FAILURE, "mlockall", syscall(__NR_mlockall, mock_flags));
	int64_t errno_value = -errno;

	/*=============================== TRIGGER SYSCALL ===========================*/

	evt_test->disable_capture();

	evt_test->assert_event_presence();

	if(HasFatalFailure())
	{
		return;
	}

	evt_test->parse_event();

	evt_test->assert_header();

	/*=============================== ASSERT PARAMETERS  ===========================*/

	/* Parameter 1: res (type: PT_ERRNO)*/
	evt_test->assert_numeric_param(1, (int64_t)errno_value);

	/* Parameter 2: flags (type: PT_UINT64) */
	evt_test->assert_numeric_param(2, mock_flags);

	/*=============================== ASSERT PARAMETERS  ===========================*/

	evt_test->assert_num_params_pushed(2);
}
#endif