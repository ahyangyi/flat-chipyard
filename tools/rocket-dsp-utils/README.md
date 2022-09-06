ROCKET-DSP-UTILS
===================

[![Test](https://github.com/chick/rocket-dsp-utils/actions/workflows/test.yml/badge.svg)](https://github.com/ucb-bar/dsptools/actions/workflows/test.yml)

This repository is part of a transition to move the rocket subdirectory from
[ucb-bar/dsptools]() to its own repository

----------

This README will be filled out later. At the moment it will only contain instructions to run it locally

Goals: Get the rocket sub-project of dsptools to run within the chipyard environment.
It is based on running using the chipyards rocket-chip commit

Steps
- Checkout [rocket-chip](https://github.com/chipsalliance/rocket-chip)
  - git checkout 3b3169cb04bd5a7be4ec0be04a4cbe1a794c540e
  - make the following changes to build.sbt so that jars will be produced
```shell
git diff build.sbt
diff --git a/build.sbt b/build.sbt
index 09d5f98bd..f2f54af58 100644
--- a/build.sbt
+++ b/build.sbt
@@ -65,14 +65,13 @@ lazy val chiselPluginLib = "edu.berkeley.cs" % "chisel3-plugin" % chiselVersion

 lazy val `api-config-chipsalliance` = (project in file("api-config-chipsalliance/build-rules/sbt"))
   .settings(commonSettings)
-  .settings(publishArtifact := false)
 lazy val hardfloat  = (project in file("hardfloat"))
   .sourceDependency(chiselRef, chiselLib)
   .settings(addCompilerPlugin(chiselPluginLib))
   .settings(commonSettings)
   .settings(publishArtifact := false)
 lazy val `rocket-macros` = (project in file("macros")).settings(commonSettings)
-  .settings(publishArtifact := false)
+  // .settings(publishArtifact := false)
 lazy val rocketchip = (project in file("."))
   .sourceDependency(chiselRef, chiselLib)
   .settings(addCompilerPlugin(chiselPluginLib))
```    
- sbt
  - rocket-macros / publishLocal
  - api-config-chipsalliance / publishLocal
  - publishLocal
- should be able to edit/compile/run things
  - I have not run all tests yet.
- PLEASE let me know if this does not work for you

Questions:
- Questionable code is marked with //TODO: CHIPYARD
- Using local publishing of everything was just to get jump started, can modify build approach after getting things working


This code is maintained by [Chick](https://github.com/chick)

