/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#include <assert.h>
#include <stdlib.h> /* malloc() */
#include <windows.h>

static LPCRITICAL_SECTION cs_ = NULL;

AUG_EXTERNC aug_status
aug_initlock_(void)
{
    /* Create singleton mutex object. */

    LPCRITICAL_SECTION cs = malloc(sizeof(*cs));
    if (!cs)
        return AUG_FAILURE;

	/* In low memory situations, InitializeCriticalSection() can raise a
       STATUS_NO_MEMORY exception. */

    InitializeCriticalSection(cs);

    /* Explicit MemoryBarrier() not required: MSDN states that a full memory
       barrier is generated by InterlockedCompareExchangePointer(). */

    if (InterlockedCompareExchangePointer((volatile PVOID*)&cs_, cs, NULL)) {

        /* Mutex already initialised. */

        DeleteCriticalSection(cs);
        free(cs);
    }
    return AUG_SUCCESS;
}

AUGCTX_API void
aug_lock(void)
{
	/* In low memory situations, EnterCriticalSection can raise a
       STATUS_INVALID_HANDLE exception. */

    assert(cs_);
    EnterCriticalSection(cs_);
}

AUGCTX_API void
aug_unlock(void)
{
    assert(cs_);
    LeaveCriticalSection(cs_);
}
