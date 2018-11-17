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

  bool finalized;

  // ### Internal helper methods ###
  void SetState(const K& id);
  void MaybeAddState(const K &id);
 public:

  // ### Graph contructors & destructors ###
  explicit Graph(const K &root_id);
  ~Graph();

  // ### Graph controller methods ###
  void Reset();
  void Step(const T& t);

  // ### Graph construction methods ###
  void Finalize();
  void AddEdge(const T &t, const K &to_id);
  void AddEdge(const T &t, const K &from_id, const K &to_id);
  void SetTerminal();
  void SetTerminal(const K &id);

  // ### Graph status query methods ###
  bool HasStep(const T& t) const;
  bool IsAtRoot() const;
  bool IsAtTerminal() const;
  bool IsFinalized() const;
  const K& GetStateId() const;
  std::vector<T> GetOpTrace() const;
  std::vector<K> GetStateTrace() const;
};

// ########################################
// ### Graph constructors & destructors ###
// ########################################
template <typename K, typename T>
Graph<K, T>::Graph(const K &root_id) : root_(root_id) {
    curr_state_ = &root_;
    MaybeAddState(root_id);
}

template <typename K, typename T>
Graph<K, T>::~Graph() {}

// ################################
// ### Graph controller methods ###
// ################################
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

// ### Internal helper methods ###
template <typename K, typename T>
void Graph<K, T>::MaybeAddState(const K &id) {
  if (states_.find(id) == states_.end())
    states_[id] = fsm::State<K,T>(id);
}

template <typename K, typename T>
void Graph<K, T>::SetState(const K &id) {
  curr_state_ = &id;
  state_trace_.push_back(id);
}

// ##################################
// ### Graph construction methods ###
// ##################################
template <typename K, typename T>
void Graph<K, T>::Finalize() {
  finalized = true;
}

template <typename K, typename T>
void Graph<K, T>::AddEdge(const T &t, const K &to_id) {
  AddEdge(t, *curr_state_, to_id);
}

template <typename K, typename T>
void Graph<K, T>::AddEdge(const T &t, const K &from_id, const K &to_id) {
  if (finalized) throw FrozenGraphException();
  MaybeAddState(from_id);
  MaybeAddState(to_id);
  states_[from_id].AddTransition(t, to_id);
}

template <typename K, typename T>
void Graph<K, T>::SetTerminal() {
  SetTerminal(*curr_state_);
}

template <typename K, typename T>
void Graph<K, T>::SetTerminal(const K &id) {
  if (finalized) throw FrozenGraphException();
  MaybeAddState(id);
  states_[id].SetTerminal();
}

// ##################################
// ### Graph status query methods ###
// ##################################
template <typename K, typename T>
bool Graph<K, T>::HasStep(const T &t) const {
  auto it = states_.find(*curr_state_);
  return it->second.HasTransition(t);
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
bool Graph<K, T>::IsFinalized() const {
  return finalized;
}

template <typename K, typename T>
const K& Graph<K, T>::GetStateId() const {
  return *curr_state_;
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
