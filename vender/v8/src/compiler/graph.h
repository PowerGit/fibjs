// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_GRAPH_H_
#define V8_COMPILER_GRAPH_H_

#include "src/zone.h"
#include "src/zone-containers.h"

namespace v8 {
namespace internal {
namespace compiler {

// Forward declarations.
class GraphDecorator;
class Node;
class Operator;


// Marks are used during traversal of the graph to distinguish states of nodes.
// Each node has a mark which is a monotonically increasing integer, and a
// {NodeMarker} has a range of values that indicate states of a node.
typedef uint32_t Mark;


// NodeIds are identifying numbers for nodes that can be used to index auxiliary
// out-of-line data associated with each node.
typedef int32_t NodeId;


class Graph : public ZoneObject {
 public:
  explicit Graph(Zone* zone);

  // Base implementation used by all factory methods.
  Node* NewNode(const Operator* op, int input_count, Node** inputs,
                bool incomplete = false);

  // Factories for nodes with static input counts.
  Node* NewNode(const Operator* op) {
    return NewNode(op, 0, static_cast<Node**>(nullptr));
  }
  Node* NewNode(const Operator* op, Node* n1) { return NewNode(op, 1, &n1); }
  Node* NewNode(const Operator* op, Node* n1, Node* n2) {
    Node* nodes[] = {n1, n2};
    return NewNode(op, arraysize(nodes), nodes);
  }
  Node* NewNode(const Operator* op, Node* n1, Node* n2, Node* n3) {
    Node* nodes[] = {n1, n2, n3};
    return NewNode(op, arraysize(nodes), nodes);
  }
  Node* NewNode(const Operator* op, Node* n1, Node* n2, Node* n3, Node* n4) {
    Node* nodes[] = {n1, n2, n3, n4};
    return NewNode(op, arraysize(nodes), nodes);
  }
  Node* NewNode(const Operator* op, Node* n1, Node* n2, Node* n3, Node* n4,
                Node* n5) {
    Node* nodes[] = {n1, n2, n3, n4, n5};
    return NewNode(op, arraysize(nodes), nodes);
  }
  Node* NewNode(const Operator* op, Node* n1, Node* n2, Node* n3, Node* n4,
                Node* n5, Node* n6) {
    Node* nodes[] = {n1, n2, n3, n4, n5, n6};
    return NewNode(op, arraysize(nodes), nodes);
  }
  Node* NewNode(const Operator* op, Node* n1, Node* n2, Node* n3, Node* n4,
                Node* n5, Node* n6, Node* n7) {
    Node* nodes[] = {n1, n2, n3, n4, n5, n6, n7};
    return NewNode(op, arraysize(nodes), nodes);
  }
  Node* NewNode(const Operator* op, Node* n1, Node* n2, Node* n3, Node* n4,
                Node* n5, Node* n6, Node* n7, Node* n8) {
    Node* nodes[] = {n1, n2, n3, n4, n5, n6, n7, n8};
    return NewNode(op, arraysize(nodes), nodes);
  }
  Node* NewNode(const Operator* op, Node* n1, Node* n2, Node* n3, Node* n4,
                Node* n5, Node* n6, Node* n7, Node* n8, Node* n9) {
    Node* nodes[] = {n1, n2, n3, n4, n5, n6, n7, n8, n9};
    return NewNode(op, arraysize(nodes), nodes);
  }

  template <class Visitor>
  inline void VisitNodeInputsFromEnd(Visitor* visitor);

  Zone* zone() const { return zone_; }
  Node* start() const { return start_; }
  Node* end() const { return end_; }

  void SetStart(Node* start) { start_ = start; }
  void SetEnd(Node* end) { end_ = end; }

  int NodeCount() const { return next_node_id_; }

  void Decorate(Node* node, bool incomplete);
  void AddDecorator(GraphDecorator* decorator);
  void RemoveDecorator(GraphDecorator* decorator);

 private:
  friend class NodeMarkerBase;

  inline NodeId NextNodeId();

  Zone* const zone_;
  Node* start_;
  Node* end_;
  Mark mark_max_;
  NodeId next_node_id_;
  ZoneVector<GraphDecorator*> decorators_;

  DISALLOW_COPY_AND_ASSIGN(Graph);
};


// A graph decorator can be used to add behavior to the creation of nodes
// in a graph.
class GraphDecorator : public ZoneObject {
 public:
  virtual ~GraphDecorator() {}
  virtual void Decorate(Node* node, bool incomplete) = 0;
};

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_GRAPH_H_
