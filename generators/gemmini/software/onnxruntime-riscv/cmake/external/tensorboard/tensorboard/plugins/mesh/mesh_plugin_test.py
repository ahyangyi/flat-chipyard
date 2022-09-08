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
"""Tests the Tensorboard mesh plugin."""
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import collections
import os
import shutil
import numpy as np
import tensorflow as tf

from werkzeug import test as werkzeug_test
from werkzeug import wrappers
from tensorboard.backend import application
from tensorboard.backend.event_processing import plugin_event_multiplexer as event_multiplexer
from tensorboard.plugins import base_plugin
from tensorboard.plugins.mesh import mesh_plugin
from tensorboard.plugins.mesh import summary
from tensorboard.plugins.mesh import plugin_data_pb2
from tensorboard.plugins.mesh import test_utils
from tensorboard.util import test_util as tensorboard_test_util

try:
  # python version >= 3.3
  from unittest import mock  # pylint: disable=g-import-not-at-top
except ImportError:
  import mock  # pylint: disable=g-import-not-at-top,unused-import


@tensorboard_test_util.run_v1_only('requires tf.Session')
class MeshPluginTest(tf.test.TestCase):
  """Tests for mesh plugin server."""

  def setUp(self):
    # We use numpy.random to generate meshes. We seed to avoid non-determinism
    # in this test.
    np.random.seed(17)

    # Log dir to save temp events into.
    self.log_dir = self.get_temp_dir()

    # Create mesh summary.
    tf.compat.v1.reset_default_graph()
    sess = tf.compat.v1.Session()
    point_cloud = test_utils.get_random_mesh(1000)
    point_cloud_vertices = tf.compat.v1.placeholder(tf.float32,
                                                    point_cloud.vertices.shape)

    mesh_no_color = test_utils.get_random_mesh(2000, add_faces=True)
    mesh_no_color_vertices = tf.compat.v1.placeholder(
        tf.float32, mesh_no_color.vertices.shape)
    mesh_no_color_faces = tf.compat.v1.placeholder(tf.int32,
                                                   mesh_no_color.faces.shape)

    mesh_color = test_utils.get_random_mesh(
        3000, add_faces=True, add_colors=True)
    mesh_color_vertices = tf.compat.v1.placeholder(tf.float32,
                                                   mesh_color.vertices.shape)
    mesh_color_faces = tf.compat.v1.placeholder(tf.int32,
                                                mesh_color.faces.shape)
    mesh_color_colors = tf.compat.v1.placeholder(tf.uint8,
                                                 mesh_color.colors.shape)
    self.data = [point_cloud, mesh_no_color, mesh_color]

    # In case when name is present and display_name is not, we will reuse name
    # as display_name. Summaries below intended to test both cases.
    self.names = ["point_cloud", "mesh_no_color", "mesh_color"]
    summary.op(
        self.names[0],
        point_cloud_vertices,
        description="just point cloud")
    summary.op(
        self.names[1],
        mesh_no_color_vertices,
        faces=mesh_no_color_faces,
        display_name="name_to_display_in_ui",
        description="beautiful mesh in grayscale")
    summary.op(
        self.names[2],
        mesh_color_vertices,
        faces=mesh_color_faces,
        colors=mesh_color_colors,
        description="mesh with random colors")

    merged_summary_op = tf.compat.v1.summary.merge_all()
    self.runs = ["bar"]
    self.steps = 20
    bar_directory = os.path.join(self.log_dir, self.runs[0])
    with tensorboard_test_util.FileWriterCache.get(bar_directory) as writer:
      writer.add_graph(sess.graph)
      for step in range(self.steps):
        writer.add_summary(
            sess.run(
                merged_summary_op,
                feed_dict={
                    point_cloud_vertices: point_cloud.vertices,
                    mesh_no_color_vertices: mesh_no_color.vertices,
                    mesh_no_color_faces: mesh_no_color.faces,
                    mesh_color_vertices: mesh_color.vertices,
                    mesh_color_faces: mesh_color.faces,
                    mesh_color_colors: mesh_color.colors,
                }),
            global_step=step)

    # Start a server that will receive requests.
    self.multiplexer = event_multiplexer.EventMultiplexer({
        "bar": bar_directory,
    })
    self.context = base_plugin.TBContext(
        logdir=self.log_dir, multiplexer=self.multiplexer)
    self.plugin = mesh_plugin.MeshPlugin(self.context)
    wsgi_app = application.TensorBoardWSGIApp(
        self.log_dir, [self.plugin],
        self.multiplexer,
        reload_interval=0,
        path_prefix="")
    self.server = werkzeug_test.Client(wsgi_app, wrappers.BaseResponse)
    self.multiplexer.Reload()
    self.routes = self.plugin.get_plugin_apps()

  def tearDown(self):
    shutil.rmtree(self.log_dir, ignore_errors=True)

  def testRoutes(self):
    """Tests that the /tags route offers the correct run to tag mapping."""
    self.assertIsInstance(self.routes["/tags"], collections.Callable)
    self.assertIsInstance(self.routes["/meshes"], collections.Callable)
    self.assertIsInstance(self.routes["/data"], collections.Callable)

  def testTagsRoute(self):
    """Tests that the /tags route offers the correct run to tag mapping."""
    response = self.server.get("/data/plugin/mesh/tags")
    self.assertEqual(200, response.status_code)
    tags = test_utils.deserialize_json_response(response.get_data())
    self.assertIn(self.runs[0], tags)
    for name in self.names:
      self.assertIn(name, tags[self.runs[0]])

  def testDataRoute(self):
    """Tests that the /data route returns correct data for meshes."""
    response = self.server.get(
        "/data/plugin/mesh/data?run=%s&tag=%s&sample=%d&content_type=%s" %
        (self.runs[0], self.names[0], 0, "VERTEX"))
    self.assertEqual(200, response.status_code)
    data = test_utils.deserialize_array_buffer_response(
        next(response.response), np.float32)
    vertices = np.tile(self.data[0].vertices.reshape(-1), self.steps)
    self.assertEqual(vertices.tolist(), data.tolist())

    response = self.server.get(
        "/data/plugin/mesh/data?run=%s&tag=%s&sample=%d&content_type=%s" %
        (self.runs[0], self.names[1], 0, "FACE"))
    self.assertEqual(200, response.status_code)
    data = test_utils.deserialize_array_buffer_response(
        next(response.response), np.int32)
    faces = np.tile(self.data[1].faces.reshape(-1), self.steps)
    self.assertEqual(faces.tolist(), data.tolist())

    response = self.server.get(
        "/data/plugin/mesh/data?run=%s&tag=%s&sample=%d&content_type=%s" %
        (self.runs[0], self.names[2], 0, "COLOR"))
    self.assertEqual(200, response.status_code)
    data = test_utils.deserialize_array_buffer_response(
        next(response.response), np.uint8)
    colors = np.tile(self.data[2].colors.reshape(-1), self.steps)
    self.assertListEqual(colors.tolist(), data.tolist())

  def testMetadataRoute(self):
    """Tests that the /meshes route returns correct metadata for meshes."""
    response = self.server.get(
        "/data/plugin/mesh/meshes?run=%s&tag=%s&sample=%d" %
        (self.runs[0], self.names[0], 0))
    self.assertEqual(200, response.status_code)
    metadata = test_utils.deserialize_json_response(response.get_data())
    self.assertEqual(len(metadata), self.steps)
    self.assertAllEqual(metadata[0]["content_type"],
                        plugin_data_pb2.MeshPluginData.VERTEX)
    self.assertAllEqual(metadata[0]["data_shape"], self.data[0].vertices.shape)

  def testsEventsAlwaysSortedByWallTime(self):
    """Tests that events always sorted by wall time."""
    response = self.server.get(
        "/data/plugin/mesh/meshes?run=%s&tag=%s&sample=%d" %
        (self.runs[0], self.names[1], 0))
    self.assertEqual(200, response.status_code)
    metadata = test_utils.deserialize_json_response(response.get_data())
    for i in range(1, self.steps):
      # Timestamp will be equal when two tensors of different content type
      #  belong to the same mesh.
      self.assertLessEqual(metadata[i - 1]["wall_time"],
                           metadata[i]["wall_time"])

  @mock.patch.object(
      event_multiplexer.EventMultiplexer,
      "PluginRunToTagToContent",
      return_value={"bar": {"foo": "".encode("utf-8")}},
  )
  def testMetadataComputedOnce(self, run_to_tag_mock):
    """Tests that metadata mapping computed once."""
    self.plugin.prepare_metadata()
    self.plugin.prepare_metadata()
    self.assertEqual(1, run_to_tag_mock.call_count)

  def testIsActive(self):
    self.assertTrue(self.plugin.is_active())

  @mock.patch.object(
      event_multiplexer.EventMultiplexer,
      "PluginRunToTagToContent",
      return_value={})
  def testIsInactive(self, get_random_mesh_stub):
    self.assertFalse(self.plugin.is_active())


if __name__ == "__main__":
  tf.test.main()
