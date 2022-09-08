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

#include "platform.h"
#include "array.h"

void a_ensure_ (void *v, int delta, int sz) {
	typedef A_TYPE (void *) a_void_ptr;
	a_void_ptr *a = (a_void_ptr *) v;
	int omax = a->h_.max_;
	if (omax < 0) {
		omax = -omax;
	}
	if (a->h_.len_ + delta > omax) {
		int nmax = a->h_.len_ + delta;
		void *na;
		if (nmax < omax * 2) {
			nmax = omax * 2;
		}
		if (a->h_.max_ <= 0) {
			na = malloc (nmax * sz);
			memcpy (na, a->a_, omax*sz);
		} else {
			na = realloc (a->a_, nmax*sz);
		}
		memset (omax *sz + (char *)na, 0, (nmax - omax) * sz);
		a->a_ = (void **) na;
		a->h_.max_ = nmax;
	}
}
