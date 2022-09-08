/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
module tf.graph.loader {
  export type GraphAndHierarchy = {
    graph: SlimGraph,
    graphHierarchy: hierarchy.Hierarchy;
  };

  export function fetchAndConstructHierarchicalGraph(
      tracker: tf.graph.util.Tracker,
      remotePath: string|null,
      pbTxtFile: Blob|null,
      compatibilityProvider: op.CompatibilityProvider =
          new op.TpuCompatibilityProvider(),
      hierarchyParams: hierarchy.HierarchyParams =
          hierarchy.DefaultHierarchyParams,
  ): Promise<GraphAndHierarchy> {
    const dataTracker = util.getSubtaskTracker(tracker, 30, 'Data');
    const graphTracker = util.getSubtaskTracker(tracker, 20, 'Graph');
    const hierarchyTracker = util.getSubtaskTracker(
        tracker, 50, 'Namespace hierarchy');

    return parser.fetchAndParseGraphData(remotePath, pbTxtFile, dataTracker)
      .then(function(graph) {
        if (!graph.node) {
          throw new Error('The graph is empty. This can happen when ' +
            'TensorFlow could not trace any graph. Please refer to ' +
            'https://github.com/tensorflow/tensorboard/issues/1961 for more ' +
            'information.');
        }

        return build(graph, DefaultBuildParams, graphTracker);
      }, () => {
        throw new Error('Malformed GraphDef. This can sometimes be caused by ' +
          'a bad network connection or difficulty reconciling multiple ' +
          'GraphDefs; for the latter case, please refer to ' +
          'https://github.com/tensorflow/tensorboard/issues/1929.');
      })
      .then(async (graph) => {
        // Populate compatibile field of OpNode based on whitelist
        op.checkOpsForCompatibility(graph, compatibilityProvider);
        const graphHierarchy = await hierarchy.build(
            graph, hierarchyParams, hierarchyTracker);
        return {graph, graphHierarchy};
      })
      .catch((e) => {
        // Generic error catch, for errors that happened outside
        // asynchronous tasks.
        const msg = `Graph visualization failed.\n\n${e}`;

        tracker.reportError(msg, e);
        // Don't swallow the error.
        throw e;
      });
  }

}  // module tf.graph.loader
