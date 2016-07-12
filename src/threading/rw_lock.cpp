#include "rw_lock.h"

#include <Windows.h>

crlib::rw_lock::rw_lock()
{
	InitializeSRWLock(reinterpret_cast<PSRWLOCK>(&m_handle));
}

template<>
void crlib::rw_lock::acquire<crlib::RW_LOCK_MODE::EXCLUSLIVE>()
{
	AcquireSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&m_handle));
}

template<>
void crlib::rw_lock::acquire<crlib::RW_LOCK_MODE::SHARED>()
{
	AcquireSRWLockShared(reinterpret_cast<PSRWLOCK>(&m_handle));
}

template<>
void crlib::rw_lock::release<crlib::RW_LOCK_MODE::EXCLUSLIVE>()
{
	ReleaseSRWLockExclusive(reinterpret_cast<PSRWLOCK>(&m_handle));
}

template<>
void crlib::rw_lock::release<crlib::RW_LOCK_MODE::SHARED>()
{
	ReleaseSRWLockShared(reinterpret_cast<PSRWLOCK>(&m_handle));
}

template<>
void crlib::rw_lock::sleep<crlib::RW_LOCK_MODE::SHARED>(condition_variable& var)
{
	SleepConditionVariableSRW(reinterpret_cast<PCONDITION_VARIABLE>(var.handle()), reinterpret_cast<PSRWLOCK>(&m_handle), INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED);
}

template<>
void crlib::rw_lock::sleep<crlib::RW_LOCK_MODE::EXCLUSLIVE>(condition_variable& var)
{
	SleepConditionVariableSRW(reinterpret_cast<PCONDITION_VARIABLE>(var.handle()), reinterpret_cast<PSRWLOCK>(&m_handle), INFINITE, 0);
}

template<>
void crlib::rw_lock::sleep<crlib::RW_LOCK_MODE::SHARED>(condition_variable& var, std::function<bool()> predicate)
{
	while (!predicate())
	{
		SleepConditionVariableSRW(reinterpret_cast<PCONDITION_VARIABLE>(var.handle()), reinterpret_cast<PSRWLOCK>(&m_handle), INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED);
	}
}

template<>
void crlib::rw_lock::sleep<crlib::RW_LOCK_MODE::EXCLUSLIVE>(condition_variable& var, std::function<bool()> predicate)
{
	while (!predicate())
	{
		SleepConditionVariableSRW(reinterpret_cast<PCONDITION_VARIABLE>(var.handle()), reinterpret_cast<PSRWLOCK>(&m_handle), INFINITE, 0);
	}
}