#ifndef FSM_CORE_STATE_H
#define FSM_CORE_STATE_H
#include <map>
#include <string>
#include <sstream>
#include "exceptions.h"

namespace fsm {

template <typename K, typename T>
class State {
 private:
  std::map<T, K> tr_map_;
  K id_;
  bool is_terminal_ = false;
 public:
  State() {};
  State(const K &id, const bool is_terminal)
      : id_(id), is_terminal_(is_terminal) {};

  bool IsTerminal() const;
  void SetTerminal();

  bool HasTransition(const T &t) const;
  void AddTransition(const T &t, const K &id);

  const K& GetId() const;
  const K& Transit(const T &t) const;

};

template <typename K, typename T>
void State<K, T>::AddTransition(const T &t, const K &id) {
  auto it = tr_map_.find(t);
  if (it != tr_map_.end()) {
    std::stringstream ss;
    ss << "Duplicated transition:\"" << t << "\"" << std::endl;
    throw fsm::DuplicatedKeyException(ss.str());
  }
  tr_map_[t] = id;
}


template <typename K, typename T>
const K& State<K, T>::GetId() const {
  return id_;
}


template <typename K, typename T>
bool State<K, T>::IsTerminal() const{
  return is_terminal_;
}

template <typename K, typename T>
void State<K, T>::SetTerminal() {
  is_terminal_ = true;
}

template <typename K, typename T>
const K& State<K, T>::Transit(const T& t) const {
  auto it = tr_map_.find(t);
  if (it == tr_map_.end()) {
    std::stringstream ss;
    ss << "Transition key \"" << t << "\" not found." << std::endl;
    throw fsm::InvalidTransitionException(ss.str());
  }
  return it->second;
}

template <typename K, typename T>
bool State<K, T>::HasTransition(const T &t) const {
  auto it = tr_map_.find(t);
  return it != tr_map_.end();
}

}

#endif  // FSM_CORE_STATE_H
