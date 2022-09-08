# Copyright 2019 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================
"""A sample plugin to demonstrate dynamic loading."""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import json
import os

import numpy as np
import six
from tensorboard.plugins import base_plugin
from tensorboard.util import tensor_util
import werkzeug
from werkzeug import wrappers

from tensorboard_plugin_example import metadata


class ExamplePlugin(base_plugin.TBPlugin):
  plugin_name = metadata.PLUGIN_NAME

  def __init__(self, context):
    self._multiplexer = context.multiplexer

  def is_active(self):
    return bool(self._multiplexer.PluginRunToTagToContent(metadata.PLUGIN_NAME))

  def get_plugin_apps(self):
    return {
        "/index.js": self._serve_js,
        "/tags": self._serve_tags,
        "/greetings": self._serve_greetings,
    }

  def frontend_metadata(self):
    return super(ExamplePlugin, self).frontend_metadata()._replace(
        es_module_path="/index.js",
    )

  @wrappers.Request.application
  def _serve_js(self, request):
    del request  # unused
    filepath = os.path.join(os.path.dirname(__file__), "static", "index.js")
    with open(filepath) as infile:
      contents = infile.read()
    return werkzeug.Response(contents, content_type="application/javascript")

  @wrappers.Request.application
  def _serve_tags(self, request):
    del request  # unused
    mapping = self._multiplexer.PluginRunToTagToContent(metadata.PLUGIN_NAME)
    result = {run: {} for run in self._multiplexer.Runs()}
    for (run, tag_to_content) in six.iteritems(mapping):
      for tag in tag_to_content:
        summary_metadata = self._multiplexer.SummaryMetadata(run, tag)
        result[run][tag] = {
            u"description": summary_metadata.summary_description,
        }
    contents = json.dumps(result, sort_keys=True)
    return werkzeug.Response(contents, content_type="application/json")

  @wrappers.Request.application
  def _serve_greetings(self, request):
    run = request.args["run"]
    tag = request.args["tag"]
    data = [
        np.asscalar(tensor_util.make_ndarray(event.tensor_proto))
            .decode("utf-8")
        for event in self._multiplexer.Tensors(run, tag)
    ]
    contents = json.dumps(data, sort_keys=True)
    return werkzeug.Response(contents, content_type="application/json")
