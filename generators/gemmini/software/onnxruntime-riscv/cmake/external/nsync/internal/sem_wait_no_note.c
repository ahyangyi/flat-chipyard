/* Copyright 2016 Google Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License. */

#include "nsync_cpp.h"
#include "platform.h"
#include "compiler.h"
#include "cputype.h"
#include "nsync.h"
#include "dll.h"
#include "sem.h"
#include "wait_internal.h"
#include "common.h"
#include "atomic.h"

NSYNC_CPP_START_

/* Wait until one of:
     w->sem is non-zero----decrement it and return 0.
     abs_deadline expires---return ETIMEDOUT.
     Ignores cancel_note. */
int nsync_sem_wait_with_cancel_ (waiter *w, nsync_time abs_deadline, nsync_note cancel_note UNUSED) {
	return (nsync_mu_semaphore_p_with_deadline (&w->sem, abs_deadline));
}

NSYNC_CPP_END_
