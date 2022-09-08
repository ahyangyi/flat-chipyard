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

#include "headers.h"

NSYNC_CPP_START_

struct thd_args {
	void (*f) (void *);
	void *arg;
};

static void *body (void *v) {
	struct thd_args *args = (struct thd_args *) v;
	(*args->f) (args->arg);
	free (args);
	return (NULL);
}

void nsync_start_thread_ (void (*f) (void *), void *arg) {
	struct thd_args *args = (struct thd_args *) malloc (sizeof (*args));
	pthread_t t;
	args->f = f;
	args->arg = arg;
	pthread_create (&t, NULL, body, args);
	pthread_detach (t);
}

NSYNC_CPP_END_
