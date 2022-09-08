# Copyright 2017 The TensorFlow Authors. All Rights Reserved.
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

"""Rule for building the HTML binary using Closure Compiler."""

load("@bazel_skylib//lib:paths.bzl", "paths")

load("@io_bazel_rules_closure//closure:defs.bzl", "closure_js_aspect")
load("@io_bazel_rules_closure//closure/private:defs.bzl", "collect_js", "unfurl", "long_path")

def _tensorboard_html_binary(ctx):
  deps = unfurl(ctx.attr.deps, provider="webfiles")
  manifests = depset(order="postorder")
  files = depset()
  webpaths = depset()
  for dep in deps:
    manifests = depset(transitive=[manifests, dep.webfiles.manifests])
    webpaths = depset(transitive=[webpaths, dep.webfiles.webpaths])
    files = depset(transitive=[files, dep.data_runfiles.files])
  webpaths = depset([ctx.attr.output_path], transitive=[webpaths])
  closure_js_library=collect_js(
      unfurl(ctx.attr.deps, provider="closure_js_library"))

  # vulcanize
  jslibs = depset(ctx.files._jslibs, transitive=[closure_js_library.srcs])
  if ctx.file.path_regexs_for_noinline == None:
    ignore_regexs_file_set = depset()
    ignore_regexs_file_path = "NO_REGEXS"
  else:
    ignore_regexs_file_set = depset([ctx.file.path_regexs_for_noinline])
    ignore_regexs_file_path = ctx.file.path_regexs_for_noinline.path
  ctx.actions.run(
      inputs=depset(transitive=[
          manifests,
          files,
          ignore_regexs_file_set,
      ]).to_list(),
      tools=jslibs,
      outputs=[ctx.outputs.html],
      executable=ctx.executable._Vulcanize,
      arguments=([ctx.attr.compilation_level,
                  "true" if ctx.attr.compile else "false",
                  "true" if ctx.attr.testonly else "false",
                  ctx.attr.input_path,
                  ctx.attr.output_path,
                  ctx.outputs.html.path,
                  ignore_regexs_file_path] +
                 [f.path for f in jslibs.to_list()] +
                 [f.path for f in manifests.to_list()]),
      progress_message="Vulcanizing %s" % ctx.attr.input_path)

  # webfiles manifest
  manifest_srcs = [struct(path=ctx.outputs.html.path,
                          longpath=long_path(ctx, ctx.outputs.html),
                          webpath=ctx.attr.output_path)]
  manifest = ctx.actions.declare_file(paths.join(
      ctx.configuration.bin_dir.path, "%s.pbtxt" % ctx.label.name))
  ctx.actions.write(
      output=manifest,
      content=struct(
          label=str(ctx.label),
          src=manifest_srcs).to_proto())
  manifests = depset([manifest], transitive=[manifests])

  # webfiles server
  params = struct(
      label=str(ctx.label),
      bind="[::]:6006",
      manifest=[long_path(ctx, man) for man in manifests.to_list()],
      external_asset=[struct(webpath=k, path=v)
                      for k, v in ctx.attr.external_assets.items()])
  params_file = ctx.actions.declare_file(paths.join(
      ctx.configuration.bin_dir.path,
      "%s_server_params.pbtxt" % ctx.label.name))
  ctx.actions.write(output=params_file, content=params.to_proto())
  ctx.actions.write(
      is_executable=True,
      output=ctx.outputs.executable,
      content="#!/bin/sh\nexec %s %s" % (
          ctx.executable._WebfilesServer.short_path,
          long_path(ctx, params_file)))

  transitive_runfiles = depset()
  transitive_runfiles = depset(transitive=[
      transitive_runfiles,
      ctx.attr._WebfilesServer.data_runfiles.files,
  ])
  for dep in deps:
    transitive_runfiles = depset(transitive=[
        transitive_runfiles,
        dep.data_runfiles.files,
    ])
  return struct(
      files=depset([ctx.outputs.html]),
      webfiles=struct(
          manifest=manifest,
          manifests=manifests,
          webpaths=webpaths,
          dummy=ctx.outputs.html),
      runfiles=ctx.runfiles(
          files=ctx.files.data + [manifest,
                                  params_file,
                                  ctx.outputs.html,
                                  ctx.outputs.executable],
          transitive_files=transitive_runfiles))

tensorboard_html_binary = rule(
    implementation=_tensorboard_html_binary,
    executable=True,
    attrs={
        "compilation_level": attr.string(default="ADVANCED"),
        "input_path": attr.string(mandatory=True),
        "output_path": attr.string(mandatory=True),
        "compile": attr.bool(),
        "data": attr.label_list(allow_files=True),
        "deps": attr.label_list(aspects=[closure_js_aspect], mandatory=True),
        "external_assets": attr.string_dict(default={"/_/runfiles": "."}),
        "path_regexs_for_noinline": attr.label(allow_single_file=True),
        "_jslibs": attr.label(
            default=Label("//tensorboard/java/org/tensorflow/tensorboard/vulcanize:jslibs"),
            allow_files=True),
        "_Vulcanize": attr.label(
            default=Label("//tensorboard/java/org/tensorflow/tensorboard/vulcanize:Vulcanize"),
            executable=True,
            cfg="host"),
        "_WebfilesServer": attr.label(
            default=Label(
                "@io_bazel_rules_closure//java/io/bazel/rules/closure/webfiles/server:WebfilesServer"),
            executable=True,
            cfg="host"),
    },
    outputs={
        "html": "%{name}.html",
    })
