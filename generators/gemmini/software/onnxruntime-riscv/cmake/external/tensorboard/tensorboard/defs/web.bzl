# Copyright 2017 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Same as web_library but supports TypeScript."""

load("@bazel_skylib//lib:paths.bzl", "paths")

load("//third_party:clutz.bzl",
     "DEPRECATED_CLUTZ_ATTRIBUTES",
     "DEPRECATED_CLUTZ_OUTPUTS",
     "deprecated_clutz_aspect",
     "deprecated_extract_dts_from_closure_libraries")

load("@io_bazel_rules_closure//closure:defs.bzl",
     "closure_js_aspect")

load("@io_bazel_rules_closure//closure/private:defs.bzl",
     "CLOSURE_LIBRARY_BASE_ATTR",
     "CLOSURE_WORKER_ATTR",
     "collect_js",
     "collect_runfiles",
     "difference",
     "long_path",
     "unfurl")

load(":web_testing.bzl", _tf_web_test = "tf_web_test")

tf_web_test = _tf_web_test

def _tf_web_library(ctx):
  if not ctx.attr.srcs:
    if ctx.attr.deps:
      fail("deps can not be set when srcs is not")
    if not ctx.attr.exports:
      fail("exports must be set if srcs is not")
  if ctx.attr.path:
    if not ctx.attr.path.startswith("/"):
      fail("webpath must start with /")
    if ctx.attr.path != "/" and ctx.attr.path.endswith("/"):
      fail("webpath must not end with / unless it is /")
    if "//" in ctx.attr.path:
      fail("webpath must not have //")
  elif ctx.attr.srcs:
    fail("path must be set when srcs is set")
  if "*" in ctx.attr.suppress and len(ctx.attr.suppress) != 1:
    fail("when \"*\" is suppressed no other items should be present")

  # process what came before
  deps = unfurl(ctx.attr.deps, provider="webfiles")
  webpaths = depset()
  ts_typings = depset(ctx.files._default_typings)
  ts_typings_paths = depset(
      [long_path(ctx, f) for f in ctx.files._default_typings])
  ts_typings_execroots = depset()
  for dep in deps:
    webpaths = depset(transitive=[webpaths, dep.webfiles.webpaths])
    if hasattr(dep.webfiles, "ts_typings"):
      ts_typings = depset(transitive=[ts_typings, dep.webfiles.ts_typings])
    if hasattr(dep.webfiles, "ts_typings_paths"):
      ts_typings_paths = depset(transitive=[
          ts_typings_paths,
          dep.webfiles.ts_typings_paths,
      ])
    if hasattr(dep.webfiles, "ts_typings_execroots"):
      ts_typings_execroots = depset(transitive=[
          ts_typings_execroots,
          dep.webfiles.ts_typings_execroots,
      ])

  # process what comes now
  manifest_srcs = []
  new_webpaths = []
  ts_inputs = depset()
  ts_outputs = []
  ts_files = ts_typings_paths.to_list()
  new_typings = []
  new_typings_paths = []
  new_typings_execroot = struct(inputs=[])
  execroot = struct(
      inputs=[(long_path(ctx, f), f.path) for f in ctx.files._default_typings],
      outputs=[],
      program=[ctx.executable._tsc.path, "-p"])
  web_srcs = []
  path = ctx.attr.path
  strip = _get_strip(ctx)
  for src in ctx.files.srcs:
    suffix = _get_path_relative_to_package(src)
    if strip:
      if not suffix.startswith(strip):
        fail("Relative src path not start with '%s': %s" % (strip, suffix))
      suffix = suffix[len(strip):]
    webpath = "%s/%s" % ("" if path == "/" else path, suffix)
    _add_webpath(ctx, src, webpath, webpaths, new_webpaths, manifest_srcs)
    if suffix.endswith(".d.ts"):
      web_srcs.append(src)
      entry = (webpath[1:], src.path)
      new_typings.append(src)
      new_typings_paths.append(entry[0])
      new_typings_execroot.inputs.append(entry)
      ts_inputs = depset([src], transitive=[ts_inputs])
      ts_files.append(entry[0])
      execroot.inputs.append(entry)
    elif suffix.endswith(".ts"):
      noext = suffix[:-3]
      js = ctx.actions.declare_file(paths.join(
          ctx.genfiles_dir.path, "%s.js" % noext))
      dts = ctx.actions.declare_file(paths.join(
          ctx.genfiles_dir.path, "%s.d.ts" % noext))
      webpath_js = webpath[:-3] + ".js"
      webpath_dts = webpath[:-3] + ".d.ts"
      _add_webpath(ctx, js, webpath_js, webpaths, new_webpaths, manifest_srcs)
      _add_webpath(ctx, dts, webpath_dts, webpaths, new_webpaths, manifest_srcs)
      ts_inputs = depset([src], transitive=[ts_inputs])
      ts_outputs.append(js)
      ts_outputs.append(dts)
      web_srcs.append(dts)
      web_srcs.append(js)
      ts_files.append(webpath[1:])
      execroot.inputs.append((webpath[1:], src.path))
      execroot.outputs.append((webpath_js[1:], js.path))
      execroot.outputs.append((webpath_dts[1:], dts.path))
      new_typings.append(dts)
      new_typings_paths.append(webpath_dts[1:])
      new_typings_execroot.inputs.append((webpath_dts[1:], dts.path))
    else:
      web_srcs.append(src)

  # get typings for closure code
  clutz_dts = deprecated_extract_dts_from_closure_libraries(ctx)
  if clutz_dts:
    entry = (long_path(ctx, clutz_dts), clutz_dts.path)
    ts_inputs = depset([clutz_dts], transitive=[ts_inputs])
    ts_files.append(entry[0])
    execroot.inputs.append(entry)

  # compile typescript
  if execroot.outputs:
    ts_config = _new_file(ctx, "-tsc.json")
    execroot.inputs.append(("tsconfig.json", ts_config.path))
    ctx.actions.write(
        output=ts_config,
        content=struct(
            compilerOptions=struct(
                baseUrl=".",
                declaration=True,
                inlineSourceMap=True,
                inlineSources=True,
                module="es6",
                moduleResolution="node",
                skipLibCheck=True,
                noResolve=True,
                target="es5",
            ),
            files=ts_files,
        ).to_json())
    er_config = _new_file(ctx, "-tsc-execroot.json")
    ctx.actions.write(output=er_config, content=execroot.to_json())
    ts_inputs = depset(
        [ts_config, er_config],
        transitive=[
            ts_inputs,
            # TODO(@wchargin): Because `_tsc` is passed as `tools`
            # below, collecting its runfiles should in principle be
            # unnecessary, but without this line the two deps of `_tsc`
            # (`tsc.js` and `@org_nodejs//:bin/node`) are in fact not
            # included, and so execrooter fails at runtime.
            collect_runfiles([ctx.attr._tsc]),
            ts_typings,
            ts_typings_execroots,
        ],
    )
    ctx.actions.run(
        tools=ctx.files._tsc,
        inputs=ts_inputs,
        outputs=ts_outputs,
        executable=ctx.executable._execrooter,
        arguments=(
            [er_config.path] +
            [f.path for f in ts_typings_execroots.to_list()]
        ),
        progress_message="Compiling %d TypeScript files %s" % (
            len(ts_files), ctx.label))

  # perform strict dependency checking
  manifest = _make_manifest(ctx, manifest_srcs)
  webpaths = depset(new_webpaths, transitive=[webpaths])
  dummy, manifests = _run_webfiles_validator(ctx, web_srcs, deps, manifest)
  web_srcs.append(dummy)

  # define development web server that only applies to this transitive closure
  if ctx.attr.srcs:
    devserver_manifests = manifests
    export_deps = []
  else:
    # If a rule exists purely to export other build rules, then it's
    # appropriate for the exported sources to be included in the
    # development web server.
    devserver_manifests = depset(order="postorder")
    export_deps = unfurl(ctx.attr.exports)
    for dep in export_deps:
      devserver_manifests += dep.webfiles.manifests
    devserver_manifests = manifests + devserver_manifests
  params = struct(
      label=str(ctx.label),
      bind="localhost:6006",
      manifest=[long_path(ctx, man) for man in devserver_manifests.to_list()],
      external_asset=[struct(webpath=k, path=v)
                      for k, v in ctx.attr.external_assets.items()])
  params_file = _new_file(ctx, "-params.pbtxt")
  ctx.actions.write(output=params_file, content=params.to_proto())
  ctx.actions.write(
      is_executable=True,
      output=ctx.outputs.executable,
      content="#!/bin/sh\nexec %s %s" % (
          ctx.executable._WebfilesServer.short_path,
          long_path(ctx, params_file)))

  if new_typings:
    er_config = _new_file(ctx, "-typings-execroot.json")
    ctx.actions.write(output=er_config, content=new_typings_execroot.to_json())
    ts_typings = depset(new_typings, transitive=[ts_typings])
    ts_typings_paths = depset(new_typings_paths, transitive=[ts_typings_paths])
    ts_typings_execroots = depset(
        [er_config],
        transitive=[ts_typings_execroots],
    )
  else:
    ts_typings = depset()
    ts_typings_paths = depset()
    ts_typings_execroots = depset()

  # Export data to parent rules. This uses the legacy, string-based
  # provider mechanism for compatibility with the base `web_library`
  # rule from rules_closure: because `tf_web_library`s may depend on
  # either other `tf_web_library`s or base `web_library`s, the
  # interfaces ~must be the same.
  #
  # buildozer: disable=rule-impl-return
  return struct(
      files=depset(web_srcs + [dummy]),
      exports=unfurl(ctx.attr.exports),
      webfiles=struct(
          manifest=manifest,
          manifests=manifests,
          webpaths=webpaths,
          dummy=dummy,
          ts_typings=ts_typings,
          ts_typings_paths=ts_typings_paths,
          ts_typings_execroots=ts_typings_execroots),
      closure_js_library=collect_js(
          unfurl(ctx.attr.deps, provider="closure_js_library"),
          ctx.files._closure_library_base),
      runfiles=ctx.runfiles(
          files=(ctx.files.srcs +
                 ctx.files.data +
                 ts_outputs +
                 ctx.files._closure_library_base + [
                     manifest,
                     params_file,
                     ctx.outputs.executable,
                     dummy]),
          transitive_files=depset(transitive=[
              collect_runfiles([ctx.attr._WebfilesServer]),
              collect_runfiles(deps),
              collect_runfiles(export_deps),
              collect_runfiles(ctx.attr.data),
          ])))

def _make_manifest(ctx, src_list):
  manifest = _new_file(ctx, "-webfiles.pbtxt")
  ctx.actions.write(
      output=manifest,
      content=struct(
          label=str(ctx.label),
          src=src_list).to_proto())
  return manifest

def _run_webfiles_validator(ctx, srcs, deps, manifest):
  dummy = _new_file(ctx, "-webfiles.ignoreme")
  manifests = depset(order="postorder")
  for dep in deps:
    manifests = depset(transitive=[manifests, dep.webfiles.manifests])
  if srcs:
    args = ["WebfilesValidator",
            "--dummy", dummy.path,
            "--target", manifest.path]
    if hasattr(ctx, "attr") and hasattr(ctx.attr, "suppress"):
      for category in ctx.attr.suppress:
        args.append("--suppress")
        args.append(category)
    inputs = []  # list of depsets
    inputs.append(depset([manifest] + srcs))
    direct_manifests = depset()
    for dep in deps:
      inputs.append(depset([dep.webfiles.dummy]))
      inputs.append(dep.files)
      direct_manifests = depset(
          [dep.webfiles.manifest],
          transitive=[direct_manifests],
      )
      inputs.append(depset([dep.webfiles.manifest]))
      args.append("--direct_dep")
      args.append(dep.webfiles.manifest.path)
    for man in difference(manifests, direct_manifests):
      inputs.append(depset([man]))
      args.append("--transitive_dep")
      args.append(man.path)
    argfile = _new_file(ctx, "-webfiles-checker-args.txt")
    ctx.actions.write(output=argfile, content="\n".join(args))
    inputs.append(depset([argfile]))
    ctx.actions.run(
        inputs=depset(transitive=inputs),
        outputs=[dummy],
        executable=(getattr(ctx.executable, "_ClosureWorker", None) or
                    getattr(ctx.executable, "_ClosureWorkerAspect", None)),
        arguments=["@@" + argfile.path],
        mnemonic="Closure",
        execution_requirements={"supports-workers": "1"},
        progress_message="Checking webfiles %s" % ctx.label)
  else:
    ctx.actions.write(output=dummy, content="BOO!")
  manifests = depset([manifest], transitive=[manifests])
  return dummy, manifests

def _new_file(ctx, suffix):
  return ctx.actions.declare_file(paths.join(
      ctx.bin_dir.path, "%s%s" % (ctx.label.name, suffix)))

def _add_webpath(ctx, src, webpath, webpaths, new_webpaths, manifest_srcs):
  if webpath in new_webpaths:
    _fail(ctx, "multiple srcs within %s define the webpath %s " % (
        ctx.label, webpath))
  if webpath in webpaths.to_list():
    _fail(ctx, "webpath %s was defined by %s when already defined by deps" % (
        webpath, ctx.label))
  new_webpaths.append(webpath)
  manifest_srcs.append(struct(
      path=src.path,
      longpath=long_path(ctx, src),
      webpath=webpath))

def _fail(ctx, message):
  if ctx.attr.suppress == ["*"]:
    print(message)
  else:
    fail(message)

def _get_path_relative_to_package(artifact):
  """Returns file path relative to the package that declared it."""
  path = artifact.path
  for prefix in (artifact.root.path,
                 artifact.owner.workspace_root if artifact.owner else '',
                 artifact.owner.package if artifact.owner else ''):
    if prefix:
      prefix = prefix + "/"
      if not path.startswith(prefix):
        fail("Path %s doesn't start with %s" % (path, prefix))
      path = path[len(prefix):]
  return path

def _get_strip(ctx):
  strip = ctx.attr.strip_prefix
  if strip:
    if strip.startswith("/"):
      _fail(ctx, "strip_prefix should not end with /")
      strip = strip[1:]
    if strip.endswith("/"):
      _fail(ctx, "strip_prefix should not end with /")
    else:
      strip += "/"
  return strip

tf_web_library = rule(
    implementation=_tf_web_library,
    executable=True,
    attrs=dict(DEPRECATED_CLUTZ_ATTRIBUTES.items() + {
        "path": attr.string(),
        "srcs": attr.label_list(allow_files=True),
        "deps": attr.label_list(
            aspects=[
                deprecated_clutz_aspect,
                closure_js_aspect,
            ]),
        "exports": attr.label_list(),
        "data": attr.label_list(allow_files=True),
        "suppress": attr.string_list(),
        "strip_prefix": attr.string(),
        "external_assets": attr.string_dict(default={"/_/runfiles": "."}),
        "clutz_entry_points": attr.string_list(),
        "_execrooter": attr.label(
            default=Label("//tensorboard/scripts:execrooter"),
            executable=True,
            cfg="host"),
        "_tsc": attr.label(
            default=Label("@com_microsoft_typescript//:tsc"),
            allow_files=True,
            executable=True,
            cfg="host"),
        "_default_typings": attr.label(
            default=Label("//tensorboard:tf_web_library_default_typings"),
            allow_files=True),
        "_WebfilesServer": attr.label(
            default=Label("@io_bazel_rules_closure//java/io/bazel/rules/closure/webfiles/server:WebfilesServer"),
            executable=True,
            cfg="host"),
        "_ClosureWorker": CLOSURE_WORKER_ATTR,
        "_closure_library_base": CLOSURE_LIBRARY_BASE_ATTR,
    }.items()),
    outputs=DEPRECATED_CLUTZ_OUTPUTS)
