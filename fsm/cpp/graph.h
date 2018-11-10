#ifndef FSM_CORE_GRAPH_H
#define FSM_CORE_GRAPH_H
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include "exceptions.h"
#include "state.h"

namespace fsm {

template <typename Op>
class Graph {
 private:
  std::string root_;
  const std::string *curr_state_;
  std::vector<Op> op_trace_;
  std::vector<std::string> state_trace_;
  std::map<std::string, fsm::State<Op>> states_;

  void SetState(const std::string& state_id);
 public:

  explicit Graph(const State<Op>& root) : root_(root.GetId()) {
    curr_state_ = &root_;
    AddState(root);
  }

  void AddState(const State<Op>& state);
  void Reset();
  void Step(const Op& op);
  bool TryStep(const Op& op);
  bool HasStep(const Op& op) const;
  bool IsAtRoot() const;

  std::string GetState() const;
  std::vector<Op> GetOpTrace() const;
  std::vector<std::string> GetStateTrace() const;

  friend Graph& operator<<(Graph& output, const State<Op>& state) {
    output.AddState(state);
    return output;
  }
};

template <typename Op>
void Graph<Op>::SetState(const std::string &state_id) {
  curr_state_ = &state_id;
  state_trace_.push_back(state_id);
}

template <typename Op>
std::string Graph<Op>::GetState() const {
  return *curr_state_;
}

template <typename Op>
void Graph<Op>::AddState(const fsm::State<Op> &state) {
  auto it = states_.find(state.GetId());
  if (it != states_.end()) {
    std::stringstream ss;
    ss << "Duplicated state id_: \"" << state.GetId() << "\"" << std::endl;
    throw DuplicatedKeyException(ss.str());
  }
  states_[state.GetId()] = state;
}

template <typename Op>
void Graph<Op>::Reset() {
  state_trace_.clear();
  op_trace_.clear();
  SetState(root_);
}

template <typename Op>
void Graph<Op>::Step(const Op& op) {
  op_trace_.push_back(op);
  SetState(states_[*curr_state_].Transit(op));
}

template <typename Op>
bool Graph<Op>::HasStep(const Op &op) const {
  auto it = states_.find(*curr_state_);
  return it->second.HasOp(op);
}

template <typename Op>
bool Graph<Op>::TryStep(const Op& op) {
  if (!HasStep(op)) return false;
  Step(op);
  return true;
}

template <typename Op>
bool Graph<Op>::IsAtRoot() const {
  return curr_state_ == &root_;
}

template <typename Op>
std::vector<Op> Graph<Op>::GetOpTrace() const {
  return op_trace_;
}

template <typename Op>
std::vector<std::string> Graph<Op>::GetStateTrace() const {
  return state_trace_;
}

}

#endif  // FSM_CORE_GRAPH_H
