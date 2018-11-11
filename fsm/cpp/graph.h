#ifndef FSM_CORE_GRAPH_H
#define FSM_CORE_GRAPH_H
#include <map>
#include <vector>
#include <sstream>
#include "exceptions.h"
#include "state.h"

namespace fsm {

template <typename K, typename T>
class Graph {
 private:
  K root_;
  const K *curr_state_;
  std::vector<T> op_trace_;
  std::vector<K> state_trace_;
  std::map<K, fsm::State<K, T>> states_;

  void SetState(const K& id);
 public:

  explicit Graph(const State<K, T>& root) : root_(root.GetId()) {
    curr_state_ = &root_;
    AddState(root);
  }

  void AddState(const State<K, T>& state);
  void Reset();
  void Step(const T& t);
  bool TryStep(const T& t);
  bool HasStep(const T& t) const;

  bool IsAtRoot() const;
  bool IsAtTerminal() const;

  State<K, T>& GetState();
  const K& GetStateId() const;
  std::vector<T> GetOpTrace() const;
  std::vector<K> GetStateTrace() const;

  friend Graph& operator<<(Graph& output, const State<K, T> &state) {
    output.AddState(state);
    return output;
  }
};

template <typename K, typename T>
void Graph<K, T>::SetState(const K &id) {
  curr_state_ = &id;
  state_trace_.push_back(id);
}

template <typename K, typename T>
State<K, T>& Graph<K, T>::GetState() {
  return states_[*curr_state_];
}

template <typename K, typename T>
const K& Graph<K, T>::GetStateId() const {
  return *curr_state_;
}

template <typename K, typename T>
void Graph<K, T>::AddState(const fsm::State<K, T> &state) {
  auto it = states_.find(state.GetId());
  if (it != states_.end()) {
    std::stringstream ss;
    ss << "Duplicated state id_: \"" << state.GetId() << "\"" << std::endl;
    throw DuplicatedKeyException(ss.str());
  }
  states_[state.GetId()] = state;
}

template <typename K, typename T>
void Graph<K, T>::Reset() {
  state_trace_.clear();
  op_trace_.clear();
  SetState(root_);
}

template <typename K, typename T>
void Graph<K, T>::Step(const T &t) {
  op_trace_.push_back(t);
  SetState(states_[*curr_state_].Transit(t));
}

template <typename K, typename T>
bool Graph<K, T>::HasStep(const T &t) const {
  auto it = states_.find(*curr_state_);
  return it->second.HasTransition(t);
}

template <typename K, typename T>
bool Graph<K, T>::TryStep(const T &t) {
  if (!HasStep(t)) return false;
  Step(t);
  return true;
}

template <typename K, typename T>
bool Graph<K, T>::IsAtRoot() const {
  return curr_state_ == &root_;
}

template <typename K, typename T>
bool Graph<K, T>::IsAtTerminal() const {
  auto it = states_.find(*curr_state_);
  return it->second.IsTerminal();
}

template <typename K, typename T>
std::vector<T> Graph<K, T>::GetOpTrace() const {
  return op_trace_;
}

template <typename K, typename T>
std::vector<K> Graph<K, T>::GetStateTrace() const {
  return state_trace_;
}

}

#endif  // FSM_CORE_GRAPH_H
